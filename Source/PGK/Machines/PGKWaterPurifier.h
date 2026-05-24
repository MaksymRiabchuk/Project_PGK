// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "PGKMachineBase.h"
#include "PGKWaterPurifier.generated.h"

UCLASS()
class PGK_API APGKWaterPurifier : public APGKMachineBase
{
	GENERATED_BODY()

public:
	APGKWaterPurifier();

	virtual FText GetInteractText_Implementation() override;
	virtual void Interact_Implementation(class APGKCharacter* InteractorCharacter) override;

	virtual FPGKActorSaveData GetActorSaveData_Implementation() override;
	virtual void ApplyActorSaveData_Implementation(const FPGKActorSaveData& SaveData) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPGKInventoryComponent* InventoryComponent;

	UFUNCTION(BlueprintPure, Category = "Components|Inventory")
	FORCEINLINE class UPGKInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	// Set this to the water bottle DataAsset in BP_WaterPurifier
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Production")
	class UPGKItemData* WaterBottleItem;

	// How often (seconds) one water bottle is produced
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Production")
	float ProductionInterval = 7.f;

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle ProductionTimerHandle;

	UFUNCTION()
	void ProduceWater();
};