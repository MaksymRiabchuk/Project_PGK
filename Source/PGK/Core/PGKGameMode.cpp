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

	FString SaveID = GetPlayerSaveID(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("Player Joined: %s (pending saves: %d)"), *SaveID, LoadedPlayersData.Num());

	FPGKPlayerSaveData* Found = LoadedPlayersData.Find(SaveID);

	// Fallback: last remaining entry covers newly-joining remote players
	if (!Found && LoadedPlayersData.Num() > 0)
	{
		auto MapIt = LoadedPlayersData.CreateIterator();
		ApplyPlayerSaveData(NewPlayer, MapIt.Value());
		MapIt.RemoveCurrent();
		return;
	}

	if (Found)
	{
		ApplyPlayerSaveData(NewPlayer, *Found);
		LoadedPlayersData.Remove(SaveID);
	}
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

	// --- All connected players ---
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (!PC) continue;

		APGKCharacter* Character = Cast<APGKCharacter>(PC->GetPawn());
		if (!Character) continue;

		FPGKPlayerSaveData& PData = SaveData->AllPlayersData.AddDefaulted_GetRef();
		PData.PlayerSaveID = GetPlayerSaveID(PC);
		PData.Location     = Character->GetActorLocation();
		PData.Rotation     = Character->GetActorRotation();

		if (APGKPlayerState* PS = Character->GetPlayerState<APGKPlayerState>())
		{
			PData.WaterLevel  = PS->waterLevel;
			PData.HealthLevel = PS->healthLevel;
			PData.OxygenLevel = PS->oxygenLevel;
		}

		if (UPGKInventoryComponent* Inv = Character->GetInventoryComponent())
		{
			for (const FPGKInventorySlot& Slot : Inv->InventorySlots)
			{
				FPGKSavedInventorySlot& Saved = PData.Inventory.AddDefaulted_GetRef();
				Saved.ItemData = TSoftObjectPtr<UPGKItemData>(Slot.ItemData);
				Saved.Quantity = Slot.Quantity;
			}
		}

		UE_LOG(LogTemp, Log, TEXT("Saved player '%s': Location=%s, Inventory=%d slots"),
			*PData.PlayerSaveID, *PData.Location.ToString(), PData.Inventory.Num());
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
	UE_LOG(LogTemp, Log, TEXT("Game saved — players: %d, placed actors: %d"),
		SaveData->AllPlayersData.Num(), SaveData->PlacedActors.Num());
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

	// Build per-player lookup map; entries are consumed as each player logs in.
	LoadedPlayersData.Empty();
	for (const FPGKPlayerSaveData& PData : SaveData->AllPlayersData)
	{
		FString Key = PData.PlayerSaveID.IsEmpty()
			? FString::Printf(TEXT("__idx_%d"), LoadedPlayersData.Num())
			: PData.PlayerSaveID;
		LoadedPlayersData.Add(Key, PData);
		UE_LOG(LogTemp, Log, TEXT("Load: found save entry for '%s'"), *Key);
	}

	// On a listen server the host pawn already exists at this point — apply immediately.
	// Remote clients apply their data in PostLogin.
	int32 PlayerIndex = 0;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (!PC || !PC->GetPawn()) { ++PlayerIndex; continue; }

		FString SaveID = GetPlayerSaveID(PC);
		UE_LOG(LogTemp, Log, TEXT("Load: host player ID = '%s'"), *SaveID);

		FPGKPlayerSaveData* Found = LoadedPlayersData.Find(SaveID);

		// Fallback: if no name match, pick the positionally matching entry
		if (!Found)
		{
			FString IndexKey = FString::Printf(TEXT("__idx_%d"), PlayerIndex);
			Found = LoadedPlayersData.Find(IndexKey);
			if (Found) SaveID = IndexKey;
		}

		// Last resort for single-player: use whatever entry is left
		if (!Found && LoadedPlayersData.Num() > 0)
		{
			auto MapIt = LoadedPlayersData.CreateIterator();
			ApplyPlayerSaveData(PC, MapIt.Value());
			MapIt.RemoveCurrent();
			++PlayerIndex;
			continue;
		}

		if (Found)
		{
			ApplyPlayerSaveData(PC, *Found);
			LoadedPlayersData.Remove(SaveID);
		}

		++PlayerIndex;
	}

	UE_LOG(LogTemp, Log, TEXT("Game loaded — placed actors: %d, players in save: %d"),
		SaveData->PlacedActors.Num(), SaveData->AllPlayersData.Num());
}

FString APGKGameMode::GetPlayerSaveID(APlayerController* PC) const
{
	// Player names include a session-specific hex suffix in UE (e.g. WIN-ABCD-3F7E),
	// so they change every launch. Use stable positional index instead.
	int32 Index = 0;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (It->Get() == PC) break;
		++Index;
	}
	return FString::Printf(TEXT("Player_%d"), Index);
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