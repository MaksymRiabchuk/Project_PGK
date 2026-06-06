// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PGKChestLootTable.generated.h"

class UPGKItemData;

// One entry in a loot table.
// Weight is relative — an entry with Weight=2 is twice as likely as Weight=1.
// MinProgress filters out entries until the player has reached that progress level (0–1).
USTRUCT(BlueprintType)
struct FPGKLootEntry
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot")
	UPGKItemData* Item = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot", meta = (ClampMin = "1"))
	int32 MinQuantity = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot", meta = (ClampMin = "1"))
	int32 MaxQuantity = 1;

	// Relative spawn weight among eligible entries
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot", meta = (ClampMin = "0.01"))
	float Weight = 1.f;

	// Entry is only eligible when player progress >= this value (0 = always available)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float MinProgress = 0.f;
};

// DataAsset assigned to each ChestSpawnPoint (or used as the world default).
// RollLoot() returns a list of (Item, Quantity) pairs ready to be added to a chest.
UCLASS(BlueprintType)
class PGK_API UPGKChestLootTable : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot Table")
	TArray<FPGKLootEntry> Entries;

	// How many distinct item stacks to place in the chest
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot Table", meta = (ClampMin = "1"))
	int32 MinItemsToSpawn = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot Table", meta = (ClampMin = "1"))
	int32 MaxItemsToSpawn = 3;

	// Returns rolled loot filtered by Progress (0–1).
	// Each element is a (Item, Quantity) pair.
	UFUNCTION(BlueprintCallable, Category = "Loot Table")
	TArray<FPGKLootEntry> RollLoot(float Progress) const;
};