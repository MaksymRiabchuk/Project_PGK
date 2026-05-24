// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/Types/PGKSaveTypes.h"
#include "PGKGameMode.generated.h"

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

private:
	void LoadSavedGame();
	void ApplyPlayerSaveData(APlayerController* PC, const FPGKPlayerSaveData& Data);

	bool bHasPendingPlayerLoad = false;
	FPGKPlayerSaveData PendingPlayerData;
};



