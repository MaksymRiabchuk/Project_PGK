// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved

#include "Machines/PGKWaterPurifier.h"
#include "Character/PGKCharacter.h"
#include "Character/PGKPlayerController.h"
#include "Core/Inventory/PGKInventoryComponent.h"
#include "Core/Inventory/PGKItemData.h"
#include "Core/Types/PGKSaveTypes.h"

APGKWaterPurifier::APGKWaterPurifier()
{
	PrimaryActorTick.bCanEverTick = false;

	InventoryComponent = CreateDefaultSubobject<UPGKInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->MaxInventorySize = 5;
	InventoryComponent->SetIsReplicated(true);
}

void APGKWaterPurifier::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && WaterBottleItem && SideProductItem)
	{
		GetWorldTimerManager().SetTimer(
			ProductionTimerHandle,
			this,
			&APGKWaterPurifier::ProduceWater,
			ProductionInterval,
			true);
		GetWorldTimerManager().SetTimer(
			SideProductionTimerHandle,
			this,
			&APGKWaterPurifier::ProduceSideProduct,
			SideProductionInterval,
			true);
	}
}

void APGKWaterPurifier::ProduceWater()
{
	if (WaterBottleItem && InventoryComponent)
	{
		InventoryComponent->Server_AddItem(WaterBottleItem, 1);
		UE_LOG(LogTemp, Warning, TEXT("Water Bottle Produced!"));
	}
}

void APGKWaterPurifier::ProduceSideProduct()
{
	if (SideProductItem && InventoryComponent)
	{
		InventoryComponent->Server_AddItem(SideProductItem, 1);
		UE_LOG(LogTemp, Warning, TEXT("Side Product Produced!"));
	}
}

FText APGKWaterPurifier::GetInteractText_Implementation()
{
	return FText::FromString(TEXT("Press E to open Water Purifier"));
}

void APGKWaterPurifier::Interact_Implementation(APGKCharacter* InteractorCharacter)
{
	if (!InteractorCharacter) return;

	if (APGKPlayerController* PC = Cast<APGKPlayerController>(InteractorCharacter->GetController()))
	{
		Client_OpenMachineUI(PC);
	}
}

FPGKActorSaveData APGKWaterPurifier::GetActorSaveData_Implementation()
{
	FPGKActorSaveData Data = Super::GetActorSaveData_Implementation();

	if (InventoryComponent)
	{
		for (const FPGKInventorySlot& Slot : InventoryComponent->InventorySlots)
		{
			FPGKSavedInventorySlot& Saved = Data.Inventory.AddDefaulted_GetRef();
			Saved.ItemData = TSoftObjectPtr<UPGKItemData>(Slot.ItemData);
			Saved.Quantity = Slot.Quantity;
		}
	}

	return Data;
}

void APGKWaterPurifier::ApplyActorSaveData_Implementation(const FPGKActorSaveData& SaveData)
{
	SetActorTransform(SaveData.Transform);

	if (!InventoryComponent || SaveData.Inventory.IsEmpty()) return;

	InventoryComponent->InventorySlots.SetNum(InventoryComponent->MaxInventorySize);

	for (int32 i = 0; i < SaveData.Inventory.Num() && i < InventoryComponent->InventorySlots.Num(); ++i)
	{
		UPGKItemData* LoadedItem = SaveData.Inventory[i].ItemData.LoadSynchronous();
		InventoryComponent->InventorySlots[i].ItemData = LoadedItem;
		InventoryComponent->InventorySlots[i].Quantity  = LoadedItem ? SaveData.Inventory[i].Quantity : 0;
	}
}