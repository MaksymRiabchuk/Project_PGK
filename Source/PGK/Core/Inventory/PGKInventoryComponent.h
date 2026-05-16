#pragma once

#include "CoreMinimal.h"
#include "PGKConsumableItemData.h"
#include "Components/ActorComponent.h"
#include "Core/Types/PGKBuildingData.h"
#include "PGKItemData.h"
#include "PGKInventoryComponent.generated.h"

struct FPGKItemAmount;

USTRUCT(BlueprintType)
struct FPGKInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UPGKItemData* ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Quantity;

	FPGKInventorySlot()
	{
		ItemData = nullptr;
		Quantity = 0;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PGK_API UPGKInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPGKInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_InventorySlots, BlueprintReadOnly, Category = "Inventory")
	TArray<FPGKInventorySlot> InventorySlots;

	UFUNCTION()
	void OnRep_InventorySlots();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxInventorySize = 30;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
	void Server_AddItem(UPGKItemData* ItemToAdd, int32 Amount);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
	void Server_RemoveItemFromSlot(int32 SlotIndex, int32 AmountToRemove);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
	void Server_ConsumeItem(UPGKConsumableItemData* ItemToAdd, int32 Amount);

	UPROPERTY(BlueprintAssignable, Category = "Inventory | Events")
	FOnInventoryUpdated OnInventoryUpdated;

	UFUNCTION(BlueprintCallable, Category = "Inventory | Items")
	bool HasRequiredItems(const TArray<FPGKItemAmount>& RequiredItems) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory | Items")
	void ConsumeRequiredItems(const TArray<FPGKItemAmount>& RequiredItems);

protected:
	void CheckOverweightDebuff();
};
