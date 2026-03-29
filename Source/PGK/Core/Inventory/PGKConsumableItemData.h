// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Inventory/PGKItemData.h"
#include "PGKConsumableItemData.generated.h"

/**
 * 
 */
UCLASS()
class PGK_API UPGKConsumableItemData : public UPGKItemData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Consumable Info")
	float RestoreValue = 25.0f;
	UFUNCTION(BlueprintCallable, Category = "Consumable")
	virtual void UseItem(class APGKCharacter* Character);
};
