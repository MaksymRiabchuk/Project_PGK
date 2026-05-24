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

void UPGKInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    if (InventorySlots.Num() != MaxInventorySize)
    {
        InventorySlots.SetNum(MaxInventorySize);
    }
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
                InventorySlots[i].ItemData = nullptr;
                InventorySlots[i].Quantity = 0;
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

    if (GetOwner()->HasAuthority())
    {
        APawn* OwnerPawn = Cast<APawn>(GetOwner());
        if (!OwnerPawn || OwnerPawn->IsLocallyControlled())
        {
            RequestInventoryUpdate();
        }
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
    
    if (RemainingAmount > 0)
    {
        for (int32 i = 0; i < InventorySlots.Num(); ++i)
        {
            if (InventorySlots[i].ItemData == nullptr) 
            {
                int32 AmountForNewSlot = FMath::Min(RemainingAmount, ItemToAdd->MaxStackSize);
                
                InventorySlots[i].ItemData = ItemToAdd;
                InventorySlots[i].Quantity = AmountForNewSlot;
                
                RemainingAmount -= AmountForNewSlot;
                if (RemainingAmount <= 0)
                {
                    break; 
                }
            }
        }
    }

    if (RemainingAmount > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryFull! %d items were not equipped."), RemainingAmount);
    }
    
    if (GetOwner()->HasAuthority())
    {
        APawn* OwnerPawn = Cast<APawn>(GetOwner());
        if (!OwnerPawn || OwnerPawn->IsLocallyControlled())
        {
            RequestInventoryUpdate();
        }
    }
    CheckOverweightDebuff();
}

void UPGKInventoryComponent::Server_RemoveItemFromSlot_Implementation(int32 SlotIndex, int32 AmountToRemove)
{
    if (!InventorySlots.IsValidIndex(SlotIndex) || AmountToRemove <= 0) return;
    
    if (!InventorySlots[SlotIndex].ItemData) return;

    int32 CurrentQuantity = InventorySlots[SlotIndex].Quantity;
    int32 ActualAmountToRemove = FMath::Min(AmountToRemove, CurrentQuantity);

    InventorySlots[SlotIndex].Quantity -= ActualAmountToRemove;

    if (InventorySlots[SlotIndex].Quantity <= 0)
    {
        InventorySlots[SlotIndex].ItemData = nullptr;
        InventorySlots[SlotIndex].Quantity = 0;
    }

    if (GetOwner()->HasAuthority() && Cast<APawn>(GetOwner())->IsLocallyControlled())
    {
        RequestInventoryUpdate();
    }   
    CheckOverweightDebuff();
}

void UPGKInventoryComponent::CheckOverweightDebuff()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return;

    UCharacterMovementComponent* MovementComp = OwnerCharacter->GetCharacterMovement();
    if (!MovementComp) return;

    int32 OccupiedSlots = 0;
    for (const FPGKInventorySlot& Slot : InventorySlots)
    {
        if (Slot.ItemData != nullptr)
        {
            OccupiedSlots++;
        }
    }

    float FillPercentage = (float)OccupiedSlots / (float)MaxInventorySize;

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
                    InventorySlots[i].ItemData = nullptr;
                    InventorySlots[i].Quantity = 0;
                }

                if (RemainingToRemove <= 0)
                {
                    break;
                }
            }
        }
    }
    if (GetOwner()->HasAuthority())
    {
        APawn* OwnerPawn = Cast<APawn>(GetOwner());
        if (!OwnerPawn || OwnerPawn->IsLocallyControlled())
        {
            RequestInventoryUpdate();
        }
    }
    CheckOverweightDebuff(); 
}

void UPGKInventoryComponent::OnRep_InventorySlots()
{
    OnInventoryUpdated.Broadcast();
}


void UPGKInventoryComponent::Server_TransferItem_Implementation(UPGKInventoryComponent* TargetInventory, int32 SourceSlotIndex, int32 AmountToTransfer)
{
    if (!TargetInventory || TargetInventory == this) return;
    if (AmountToTransfer <= 0) return;
    if (!InventorySlots.IsValidIndex(SourceSlotIndex)) return;
    
    FPGKInventorySlot& SourceSlot = InventorySlots[SourceSlotIndex]; 
    
    if (!SourceSlot.ItemData || SourceSlot.Quantity <= 0) return;

    int32 ActualAmountToMove = FMath::Min(AmountToTransfer, SourceSlot.Quantity);
    int32 AmountRemainingToMove = ActualAmountToMove;

    for (int32 i = 0; i < TargetInventory->InventorySlots.Num(); ++i)
    {
        if (TargetInventory->InventorySlots[i].ItemData == SourceSlot.ItemData)
        {
            int32 SpaceLeftInSlot = SourceSlot.ItemData->MaxStackSize - TargetInventory->InventorySlots[i].Quantity;
            if (SpaceLeftInSlot > 0)
            {
                int32 AmountToPut = FMath::Min(SpaceLeftInSlot, AmountRemainingToMove);
                TargetInventory->InventorySlots[i].Quantity += AmountToPut;
                AmountRemainingToMove -= AmountToPut;

                if (AmountRemainingToMove <= 0) break;
            }
        }
    }

    if (AmountRemainingToMove > 0)
    {
        for (int32 i = 0; i < TargetInventory->InventorySlots.Num(); ++i)
        {
            if (TargetInventory->InventorySlots[i].ItemData == nullptr) 
            {
                int32 AmountForNewSlot = FMath::Min(AmountRemainingToMove, SourceSlot.ItemData->MaxStackSize);
                
                TargetInventory->InventorySlots[i].ItemData = SourceSlot.ItemData;
                TargetInventory->InventorySlots[i].Quantity = AmountForNewSlot;

                AmountRemainingToMove -= AmountForNewSlot;
                
                if (AmountRemainingToMove <= 0) break; 
            }
        }
    }
    
    int32 AmountSuccessfullyMoved = ActualAmountToMove - AmountRemainingToMove;

    if (AmountSuccessfullyMoved > 0)
    {
        SourceSlot.Quantity -= AmountSuccessfullyMoved;

        if (SourceSlot.Quantity <= 0)
        {
            SourceSlot.ItemData = nullptr;
            SourceSlot.Quantity = 0;
        }
        if (GetOwner()->HasAuthority())
        {
            UE_LOG(LogTemp, Log, TEXT("................. %s Combined Parameters (%d Spawned )................."), TEXT("GPU Script"), AmountSuccessfullyMoved);
            this->RequestInventoryUpdate();
            TargetInventory->RequestInventoryUpdate();

            this->CheckOverweightDebuff();
            TargetInventory->CheckOverweightDebuff();
        }
    }
}

void UPGKInventoryComponent::RequestInventoryUpdate()
{
    if (BroadcastTimerHandle.IsValid()) return;

    GetWorld()->GetTimerManager().SetTimer(
        BroadcastTimerHandle,
        [this]()
        {
            BroadcastTimerHandle.Invalidate();
            OnInventoryUpdated.Broadcast();
        },
        0.05f,
        false
    );
}