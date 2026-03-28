#include "PGKPickupItem.h"
#include "Character/PGKCharacter.h"
#include "Inventory/PGKInventoryComponent.h"

APGKPickupItem::APGKPickupItem()
{
	
}

FText APGKPickupItem::GetInteractText_Implementation()
{
	if (ItemData)
	{
		UE_LOG(LogTemp, Error, TEXT("PickUpItemData!"));		
		return FText::Format(FText::FromString(TEXT("Press E to pickup {0}")), ItemData->ItemName);
	}
	UE_LOG(LogTemp, Error, TEXT("PickUp!"));
	return FText::FromString(TEXT("Press E to pickup"));
}

void APGKPickupItem::Interact_Implementation(APGKCharacter* InteractorCharacter)
{
	if (InteractorCharacter && ItemData)
	{
		UPGKInventoryComponent* InvComp = InteractorCharacter->FindComponentByClass<UPGKInventoryComponent>();
        
		if (InvComp)
		{
			InvComp->Server_AddItem(ItemData, Amount);
			Destroy(); 
		}
	}
}


// Called when the game starts or when spawned
void APGKPickupItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APGKPickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}