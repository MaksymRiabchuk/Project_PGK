// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Types/PGKSaveTypes.h"
#include "PGKSaveGame.generated.h"

UCLASS()
class PGK_API UPGKSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPGKSaveGame();

	// One entry per connected player, keyed by PlayerSaveID inside each struct.
	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Save|Player")
	TArray<FPGKPlayerSaveData> AllPlayersData;

	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Save|World")
	FPGKWorldSaveData WorldData;

	// All buildings, machines, and chests placed by the player.
	// Actors are tagged "PlayerPlaced" at spawn so they can be found on save.
	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Save|World")
	TArray<FPGKActorSaveData> PlacedActors;

	static const FString SaveSlotName;
	static const int32   SaveUserIndex;
};