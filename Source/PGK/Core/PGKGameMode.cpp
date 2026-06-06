// Copyright Epic Games, Inc. All Rights Reserved.

#include "PGKGameMode.h"

#include "EngineUtils.h"
#include "PGKGameStateBase.h"
#include "PGKSaveGame.h"
#include "PGKTimeComponent.h"
#include "Buildings/PGKChest.h"
#include "Buildings/PGKChestSpawnPoint.h"
#include "Character/PGKCharacter.h"
#include "Character/PGKPlayerController.h"
#include "Character/PGKPlayerState.h"
#include "Core/Interfaces/PGKSaveableInterface.h"
#include "Core/Inventory/PGKInventoryComponent.h"
#include "Core/Inventory/PGKItemData.h"
#include "Kismet/GameplayStatics.h"
#include "Network/MainGameInstance.h"

APGKGameMode::APGKGameMode()
{
	GameStateClass  = APGKGameStateBase::StaticClass();
	PlayerStateClass = APGKPlayerState::StaticClass();
}

void APGKGameMode::BeginPlay()
{
	Super::BeginPlay();

	UMainGameInstance* GI = GetGameInstance<UMainGameInstance>();
	if (GI && GI->bShouldLoadSave)
	{
		GI->bShouldLoadSave = false;
		LoadSavedGame();
	}
	else
	{
		SpawnWorldChests();
	}
}

void APGKGameMode::SpawnWorldChests()
{
	if (!ChestClass || !DefaultLootTable) return;

	APGKGameStateBase* GS = GetGameState<APGKGameStateBase>();
	const float Progress = GS
		? FMath::Clamp(float(1.0 - GS->GlobalCO2 / 99.2), 0.f, 1.f)
		: 0.f;

	int32 Spawned = 0;
	for (TActorIterator<APGKChestSpawnPoint> It(GetWorld()); It; ++It)
	{
		if (It->TrySpawnChest(Progress, ChestClass, DefaultLootTable))
		{
			++Spawned;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("World chests spawned: %d"), Spawned);
}

void APGKGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	RestartPlayer(NewPlayer);

	if (bHasPendingPlayerLoad)
	{
		bHasPendingPlayerLoad = false;
		ApplyPlayerSaveData(NewPlayer, PendingPlayerData);
	}

	UE_LOG(LogTemp, Warning, TEXT("Player Joined"));
}

void APGKGameMode::RespawnPlayer(AController* Controller)
{
	if (!Controller) return;

	if (APawn* PawnToDestroy = Controller->GetPawn())
	{
		PawnToDestroy->Destroy();
	}

	RestartPlayer(Controller);

	if (APGKPlayerController* PC = Cast<APGKPlayerController>(Controller))
	{
		PC->ResetUI();
		FInputModeGameOnly InputModeData;
		PC->SetInputMode(InputModeData);
		PC->SetShowMouseCursor(false);
	}
}

// ---------------------------------------------------------------------------
// Save
// ---------------------------------------------------------------------------

void APGKGameMode::SaveGame()
{
	UPGKSaveGame* SaveData = Cast<UPGKSaveGame>(
		UGameplayStatics::CreateSaveGameObject(UPGKSaveGame::StaticClass()));
	if (!SaveData) return;

	// --- World state ---
	if (APGKGameStateBase* GS = GetGameState<APGKGameStateBase>())
	{
		SaveData->WorldData.GlobalTemperature = GS->GlobalTemperature;
		SaveData->WorldData.GlobalCO2         = GS->GlobalCO2;
		SaveData->WorldData.GlobalO2          = GS->GlobalO2;
		SaveData->WorldData.WaterLevel        = GS->WaterLevel;

		if (GS->TimeComponent)
		{
			SaveData->WorldData.DateTime = GS->TimeComponent->GetDateTime();
		}
	}

	// --- Player state (first player only for now) ---
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (!PC) continue;

		APGKCharacter* Character = Cast<APGKCharacter>(PC->GetPawn());
		if (!Character) continue;

		SaveData->PlayerData.Location = Character->GetActorLocation();
		SaveData->PlayerData.Rotation = Character->GetActorRotation();

		if (APGKPlayerState* PS = Character->GetPlayerState<APGKPlayerState>())
		{
			SaveData->PlayerData.WaterLevel  = PS->waterLevel;
			SaveData->PlayerData.HealthLevel = PS->healthLevel;
			SaveData->PlayerData.OxygenLevel = PS->oxygenLevel;
		}

		if (UPGKInventoryComponent* Inv = Character->GetInventoryComponent())
		{
			for (const FPGKInventorySlot& Slot : Inv->InventorySlots)
			{
				FPGKSavedInventorySlot& Saved = SaveData->PlayerData.Inventory.AddDefaulted_GetRef();
				Saved.ItemData = TSoftObjectPtr<UPGKItemData>(Slot.ItemData);
				Saved.Quantity = Slot.Quantity;
			}
		}

		UE_LOG(LogTemp, Log, TEXT("Saved player: Location=%s, Inventory slots=%d"),
			*SaveData->PlayerData.Location.ToString(), SaveData->PlayerData.Inventory.Num());
		break; // single-player save
	}

	// --- Placed actors ---
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->ActorHasTag(FName("PlayerPlaced"))) continue;
		if (!Actor->Implements<UPGKSaveableInterface>()) continue;

		SaveData->PlacedActors.Add(IPGKSaveableInterface::Execute_GetActorSaveData(Actor));
	}

	UGameplayStatics::SaveGameToSlot(SaveData, UPGKSaveGame::SaveSlotName, UPGKSaveGame::SaveUserIndex);
	UE_LOG(LogTemp, Log, TEXT("Game saved — placed actors: %d"), SaveData->PlacedActors.Num());
}

// ---------------------------------------------------------------------------
// Load
// ---------------------------------------------------------------------------

void APGKGameMode::LoadSavedGame()
{
	UPGKSaveGame* SaveData = Cast<UPGKSaveGame>(
		UGameplayStatics::LoadGameFromSlot(UPGKSaveGame::SaveSlotName, UPGKSaveGame::SaveUserIndex));
	if (!SaveData) return;

	// --- World state ---
	if (APGKGameStateBase* GS = GetGameState<APGKGameStateBase>())
	{
		GS->GlobalTemperature = SaveData->WorldData.GlobalTemperature;
		GS->GlobalCO2         = SaveData->WorldData.GlobalCO2;
		GS->GlobalO2          = SaveData->WorldData.GlobalO2;
		GS->WaterLevel        = SaveData->WorldData.WaterLevel;

		if (GS->TimeComponent)
		{
			GS->TimeComponent->SetDateTime(SaveData->WorldData.DateTime);
		}
	}

	// --- Placed actors ---
	for (const FPGKActorSaveData& ActorData : SaveData->PlacedActors)
	{
		UClass* ActorClass = ActorData.ActorClass.LoadSynchronous();
		if (!ActorClass) continue;

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AActor* Spawned = GetWorld()->SpawnActor<AActor>(ActorClass, ActorData.Transform, Params);
		if (!Spawned) continue;

		Spawned->Tags.Add(FName("PlayerPlaced"));
		IPGKSaveableInterface::Execute_ApplyActorSaveData(Spawned, ActorData);
	}

	// On a listen server PostLogin + RestartPlayer fire BEFORE BeginPlay, so the
	// player pawn already exists by the time we get here. Try to apply immediately.
	// On a dedicated server PostLogin comes later — fall back to the pending flag.
	bool bApplied = false;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (!PC || !PC->GetPawn()) continue;

		ApplyPlayerSaveData(PC, SaveData->PlayerData);
		bApplied = true;
		break;
	}

	if (!bApplied)
	{
		PendingPlayerData     = SaveData->PlayerData;
		bHasPendingPlayerLoad = true;
	}

	UE_LOG(LogTemp, Log, TEXT("Game loaded — placed actors: %d, player applied: %s"),
		SaveData->PlacedActors.Num(), bApplied ? TEXT("immediate") : TEXT("pending"));
}

void APGKGameMode::ApplyPlayerSaveData(APlayerController* PC, const FPGKPlayerSaveData& Data)
{
	APGKCharacter* Character = Cast<APGKCharacter>(PC->GetPawn());
	if (!Character) return;

	Character->SetActorLocation(Data.Location);
	PC->SetControlRotation(Data.Rotation);

	if (APGKPlayerState* PS = Character->GetPlayerState<APGKPlayerState>())
	{
		PS->waterLevel  = Data.WaterLevel;
		PS->healthLevel = Data.HealthLevel;
		PS->oxygenLevel = Data.OxygenLevel;
	}

	if (UPGKInventoryComponent* Inv = Character->GetInventoryComponent())
	{
		Inv->InventorySlots.SetNum(Inv->MaxInventorySize);

		for (int32 i = 0; i < Data.Inventory.Num() && i < Inv->InventorySlots.Num(); ++i)
		{
			UPGKItemData* LoadedItem          = Data.Inventory[i].ItemData.LoadSynchronous();
			Inv->InventorySlots[i].ItemData   = LoadedItem;
			Inv->InventorySlots[i].Quantity    = LoadedItem ? Data.Inventory[i].Quantity : 0;
		}

		Inv->RequestInventoryUpdate();
	}
}