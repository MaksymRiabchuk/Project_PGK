// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PGKChestSpawnPoint.generated.h"

class APGKChest;
class UPGKChestLootTable;

UCLASS()
class PGK_API APGKChestSpawnPoint : public AActor
{
	GENERATED_BODY()

public:
	APGKChestSpawnPoint();

	// Probability (0–1) that a chest spawns at this point on a new game
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float SpawnChance = 0.7f;

	// If set, overrides the world default loot table for this point
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	UPGKChestLootTable* LootTableOverride = nullptr;

	// Called by GameMode on new game. Returns the spawned chest or nullptr if roll failed.
	APGKChest* TrySpawnChest(float Progress, TSubclassOf<APGKChest> ChestClass, UPGKChestLootTable* DefaultLootTable);

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	class UBillboardComponent* EditorSprite;
#endif
};