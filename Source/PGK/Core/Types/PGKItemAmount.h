#pragma once

#include "CoreMinimal.h"
#include "Core/Inventory/PGKItemData.h"
#include "PGKItemAmount.generated.h"

USTRUCT(BlueprintType)
struct FPGKItemAmount
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UPGKItemData* ItemData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Amount;
};