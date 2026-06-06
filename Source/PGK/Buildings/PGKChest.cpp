 // Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved


#include "Buildings/PGKChest.h"
#include "Character/PGKCharacter.h"
#include "Core/Inventory/PGKInventoryComponent.h"
#include "Core/Inventory/PGKItemData.h"

// Sets default values
APGKChest::APGKChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	InventoryComponent = CreateDefaultSubobject<UPGKInventoryComponent>(TEXT("PGK_Inventory"));
	InventoryComponent->SetIsReplicated(true);

}

void APGKChest::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && InventoryComponent)
	{
		InventoryComponent->OnInventoryUpdated.AddDynamic(this, &APGKChest::CheckIfEmptyAndDestroy);
	}
}

void APGKChest::CheckIfEmptyAndDestroy()
{
	if (!bDestroyWhenEmpty || !HasAuthority()) return;

	for (const FPGKInventorySlot& Slot : InventoryComponent->InventorySlots)
	{
		if (Slot.ItemData) return;
	}

	Destroy();
}

// Called every frame
void APGKChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
FText APGKChest::GetInteractText_Implementation()
{
	return FText::FromString(TEXT("Press E to pickup"));
}

FPGKActorSaveData APGKChest::GetActorSaveData_Implementation()
{
	FPGKActorSaveData Data = Super::GetActorSaveData_Implementation();

	if (InventoryComponent)
	{
		for (const FPGKInventorySlot& Slot : InventoryComponent->InventorySlots)
		{
			FPGKSavedInventorySlot& SavedSlot = Data.Inventory.AddDefaulted_GetRef();
			SavedSlot.ItemData = TSoftObjectPtr<UPGKItemData>(Slot.ItemData);
			SavedSlot.Quantity  = Slot.Quantity;
		}
	}

	return Data;
}

void APGKChest::ApplyActorSaveData_Implementation(const FPGKActorSaveData& SaveData)
{
	SetActorTransform(SaveData.Transform);

	if (!InventoryComponent || SaveData.Inventory.IsEmpty()) return;

	InventoryComponent->InventorySlots.SetNum(InventoryComponent->MaxInventorySize);

	for (int32 i = 0; i < SaveData.Inventory.Num() && i < InventoryComponent->InventorySlots.Num(); ++i)
	{
		const FPGKSavedInventorySlot& Saved = SaveData.Inventory[i];
		UPGKItemData* LoadedItem = Saved.ItemData.LoadSynchronous();

		InventoryComponent->InventorySlots[i].ItemData = LoadedItem;
		InventoryComponent->InventorySlots[i].Quantity  = LoadedItem ? Saved.Quantity : 0;
	}
}

void APGKChest::Interact_Implementation(APGKCharacter* InteractorCharacter)
{
	if (InteractorCharacter)
	{
		APGKPlayerController* PC = Cast<APGKPlayerController>(InteractorCharacter->GetController());
		if (PC)
		{
			Client_OpenMachineUI_Implementation(PC);			
		}
	}
}
