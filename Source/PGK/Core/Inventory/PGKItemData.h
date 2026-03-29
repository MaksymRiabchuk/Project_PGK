#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PGKItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Resource UMETA(DisplayName = "Resource"),
	Tool     UMETA(DisplayName = "Equipment"),
	Food     UMETA(DisplayName = "Food"),
	Water     UMETA(DisplayName = "Water"),
	Oxygen     UMETA(DisplayName = "Oxygen"),
};

UCLASS(BlueprintType)
class PGK_API UPGKItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Info")
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Info")
	class UTexture2D* ItemIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Info")
	EItemType ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Info", meta = (ClampMin = "1"))
	int32 MaxStackSize = 1;
};