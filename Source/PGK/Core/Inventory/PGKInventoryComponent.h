#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PGKItemData.h"
#include "PGKInventoryComponent.generated.h"

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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PGK_API UPGKInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPGKInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
	TArray<FPGKInventorySlot> InventorySlots;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxInventorySize = 20;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
	void Server_AddItem(UPGKItemData* ItemToAdd, int32 Amount);

protected:
	void CheckOverweightDebuff();
};
