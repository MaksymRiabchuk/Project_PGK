// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/Types/PGKSaveTypes.h"
#include "Buildings/PGKChestLootTable.h"
#include "PGKGameMode.generated.h"

class APGKChest;

/**
 *  Simple GameMode for a first person game
 */
UCLASS(abstract)
class APGKGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APGKGameMode();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	void RespawnPlayer(AController* Controller);

	// Collects world + player state and writes to disk. Safe to call on server only.
	UFUNCTION(BlueprintCallable, Category = "Save")
	void SaveGame();

	// Blueprint child (BP_GameMode) must set this to BP_Chest
	UPROPERTY(EditDefaultsOnly, Category = "World Chests")
	TSubclassOf<APGKChest> ChestClass;

	// Fallback loot table used when a spawn point has no override
	UPROPERTY(EditDefaultsOnly, Category = "World Chests")
	UPGKChestLootTable* DefaultLootTable = nullptr;

private:
	void LoadSavedGame();
	void ApplyPlayerSaveData(APlayerController* PC, const FPGKPlayerSaveData& Data);
	void SpawnWorldChests();

	// Returns PlayerName, or "Player_N" if name is empty. Used as the save key.
	FString GetPlayerSaveID(APlayerController* PC) const;

	// Populated during LoadSavedGame(); entries removed as each player applies their data.
	TMap<FString, FPGKPlayerSaveData> LoadedPlayersData;
};



