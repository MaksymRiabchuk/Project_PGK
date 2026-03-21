#include "PGKInventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPGKInventoryComponent::UPGKInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true); 
}

void UPGKInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UPGKInventoryComponent, InventorySlots);
}

void UPGKInventoryComponent::Server_AddItem_Implementation(UPGKItemData* ItemToAdd, int32 Amount)
{
    if (!ItemToAdd || Amount <= 0) return;

    int32 RemainingAmount = Amount;

    for (int32 i = 0; i < InventorySlots.Num(); ++i)
    {
        if (InventorySlots[i].ItemData == ItemToAdd)
        {
            int32 SpaceLeftInSlot = ItemToAdd->MaxStackSize - InventorySlots[i].Quantity;
            if (SpaceLeftInSlot > 0)
            {
                int32 AmountToPut = FMath::Min(SpaceLeftInSlot, RemainingAmount);
                InventorySlots[i].Quantity += AmountToPut;
                RemainingAmount -= AmountToPut;
                if (RemainingAmount <= 0)
                {
                    break; 
                }
            }
        }
    }
    
    while (RemainingAmount > 0 && InventorySlots.Num() < MaxInventorySize)
    {
        int32 AmountForNewSlot = FMath::Min(RemainingAmount, ItemToAdd->MaxStackSize);

        FPGKInventorySlot NewSlot;
        NewSlot.ItemData = ItemToAdd;
        NewSlot.Quantity = AmountForNewSlot;
        
        InventorySlots.Add(NewSlot);
        RemainingAmount -= AmountForNewSlot;
    }

    if (RemainingAmount > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryFull! %d items were not equipped."), RemainingAmount);
    }

    CheckOverweightDebuff();
}

void UPGKInventoryComponent::CheckOverweightDebuff()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return;

    UCharacterMovementComponent* MovementComp = OwnerCharacter->GetCharacterMovement();
    if (!MovementComp) return;

    float FillPercentage = (float)InventorySlots.Num() / (float)MaxInventorySize;

    if (FillPercentage >= 0.8f)
    {
        MovementComp->MaxWalkSpeed = 300.0f; 
    }
    else
    {
        MovementComp->MaxWalkSpeed = 600.0f; 
    }
}