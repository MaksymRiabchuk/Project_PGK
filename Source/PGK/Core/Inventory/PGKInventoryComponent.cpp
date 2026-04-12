#include "PGKInventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Character/PGKCharacter.h"
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

void UPGKInventoryComponent::Server_ConsumeItem_Implementation(UPGKConsumableItemData* ItemToConsume, int32 Amount)
{
    if (!ItemToConsume || Amount <= 0) return;

    APGKCharacter* OwnerCharacter = Cast<APGKCharacter>(GetOwner());
    if (!OwnerCharacter) return;

    int32 TotalAvailable = 0;
    for (const FPGKInventorySlot& Slot : InventorySlots)
    {
        if (Slot.ItemData == ItemToConsume)
        {
            TotalAvailable += Slot.Quantity;
        }
    }

    if (TotalAvailable < Amount)
    {
        UE_LOG(LogTemp, Warning, TEXT("Server: Player tried to consume more items than they have!"));
        return; 
    }

    int32 RemainingToRemove = Amount;
    
    for (int32 i = InventorySlots.Num() - 1; i >= 0; --i)
    {
        if (InventorySlots[i].ItemData == ItemToConsume)
        {
            if (InventorySlots[i].Quantity >= RemainingToRemove)
            {
                InventorySlots[i].Quantity -= RemainingToRemove;
                RemainingToRemove = 0;
            }
            else
            {
                RemainingToRemove -= InventorySlots[i].Quantity;
                InventorySlots[i].Quantity = 0;
            }

            if (InventorySlots[i].Quantity <= 0)
            {
                InventorySlots.RemoveAt(i);
            }

            if (RemainingToRemove <= 0)
            {
                break;
            }
        }
    }

    for (int32 i = 0; i < Amount; ++i)
    {
        ItemToConsume->UseItem(OwnerCharacter);
    }

    if (GetOwner()->HasAuthority() && Cast<APawn>(GetOwner())->IsLocallyControlled())
    {
        OnRep_InventorySlots();
    }
    CheckOverweightDebuff();
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
    
    if (GetOwner()->HasAuthority() && Cast<APawn>(GetOwner())->IsLocallyControlled())
    {
        OnRep_InventorySlots();
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

bool UPGKInventoryComponent::HasRequiredItems(const TArray<FPGKItemAmount>& RequiredItems) const
{
    for (const FPGKItemAmount& Req : RequiredItems)
    {
        if (!Req.ItemData || Req.Amount <= 0) continue;
        int32 TotalFound = 0;
        for (const FPGKInventorySlot& Slot : InventorySlots)
        {
            if (Slot.ItemData == Req.ItemData)
            {
                TotalFound += Slot.Quantity;
            }
        }
        if (TotalFound < Req.Amount)
        {
            return false;
        }
    }
    return true; 
}

void UPGKInventoryComponent::ConsumeRequiredItems(const TArray<FPGKItemAmount>& RequiredItems)
{
    if (!HasRequiredItems(RequiredItems)) return;
    for (const FPGKItemAmount& Req : RequiredItems)
    {
        if (!Req.ItemData || Req.Amount <= 0) continue;
        int32 RemainingToRemove = Req.Amount;
        for (int32 i = InventorySlots.Num() - 1; i >= 0; --i)
        {
            if (InventorySlots[i].ItemData == Req.ItemData)
            {
                if (InventorySlots[i].Quantity >= RemainingToRemove)
                {
                    InventorySlots[i].Quantity -= RemainingToRemove;
                    RemainingToRemove = 0;
                }
                else
                {
                    RemainingToRemove -= InventorySlots[i].Quantity;
                    InventorySlots[i].Quantity = 0;
                }
                if (InventorySlots[i].Quantity <= 0)
                {
                    InventorySlots.RemoveAt(i);
                }

                if (RemainingToRemove <= 0)
                {
                    break;
                }
            }
        }
    }
    if (GetOwner()->HasAuthority() && Cast<APawn>(GetOwner())->IsLocallyControlled())
    {
        OnRep_InventorySlots();
    }
    CheckOverweightDebuff(); 
}

void UPGKInventoryComponent::OnRep_InventorySlots()
{
    OnInventoryUpdated.Broadcast();
}