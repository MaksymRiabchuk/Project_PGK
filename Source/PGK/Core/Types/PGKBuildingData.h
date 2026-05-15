// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PGKItemAmount.h"
#include "Engine/DataAsset.h"
#include "PGKBuildingData.generated.h"
class UPGKItemData;
class AActor;

/**
 * 
 */
UCLASS(BlueprintType)
class PGK_API UPGKBuildingData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Info")
	FText BuildingName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Info")
	FText BuildingDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Info")
	class UTexture2D* BuildingIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Info")
	TArray<FPGKItemAmount> Recipe;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Info")
	float ZOffset = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Classes")
	TSubclassOf<AActor> HologramClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building Classes")
	TSubclassOf<AActor> ConstructedClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Placement Rules")
	bool bRequiresWater = false;
	
};
