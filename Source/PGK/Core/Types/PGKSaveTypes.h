// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "PGKCoreTypes.h"
#include "PGKSaveTypes.generated.h"

class UPGKItemData;

// Single inventory slot stored as a soft asset reference so the path
// survives serialization without pulling the full UObject into the save file.
USTRUCT(BlueprintType)
struct FPGKSavedInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Save")
	TSoftObjectPtr<UPGKItemData> ItemData;

	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Save")
	int32 Quantity = 0;
};

// Everything about the player that needs to survive a save/load cycle.
USTRUCT(BlueprintType)
struct FPGKPlayerSaveData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Save")
	FVector Location = FVector::ZeroVector;

	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Save")
	FRotator Rotation = FRotator::ZeroRotator;

	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Save")
	float WaterLevel = 100.f;

	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Save")
	float HealthLevel = 100.f;

	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Save")
	float OxygenLevel = 100.f;

	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Save")
	TArray<FPGKSavedInventorySlot> Inventory;
};

// Global environment variables and in-game time.
USTRUCT(BlueprintType)
struct FPGKWorldSaveData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Save")
	double GlobalTemperature = 368.5;

	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Save")
	double GlobalCO2 = 99.2;

	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Save")
	double GlobalO2 = 0.8;

	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Save")
	double WaterLevel = 100.0;

	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Save")
	FGameDateTime DateTime;
};

// One placed actor (building, machine, chest) in the world.
// ActorClass is a soft reference so it doesn't force-load the BP on parse.
USTRUCT(BlueprintType)
struct FPGKActorSaveData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Save")
	TSoftClassPtr<AActor> ActorClass;

	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Save")
	FTransform Transform;

	// Only populated for actors that carry an inventory (e.g. APGKChest).
	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Save")
	TArray<FPGKSavedInventorySlot> Inventory;
};