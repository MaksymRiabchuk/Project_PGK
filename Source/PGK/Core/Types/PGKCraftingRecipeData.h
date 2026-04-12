// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PGKCraftingRecipeData.generated.h"

class UPGKItemData;
struct FPGKItemAmount;
/**
 * 
 */
UCLASS(BlueprintType)
class PGK_API UPGKCraftingRecipeData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FPGKItemAmount> RequiredIngredients;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UPGKItemData* OutputItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 OutputAmount;
};
