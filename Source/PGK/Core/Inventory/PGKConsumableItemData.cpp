
#include "Core/Inventory/PGKConsumableItemData.h"

#include "Character/PGKCharacter.h"
#include "Character/PGKPlayerState.h"

void UPGKConsumableItemData::UseItem(APGKCharacter* Character)
{
	if (!Character) return;
	if (ItemType == EItemType::Food)
	{
		APGKPlayerState* playerState = Cast<APGKPlayerState>(Character->GetPlayerState());
		if (playerState)
		{
			playerState->AddHealth(RestoreValue);
			UE_LOG(LogTemp, Warning, TEXT("Ate food! Restored %f"), RestoreValue);
		}
	}
	else if (ItemType == EItemType::Water)
	{
		APGKPlayerState* playerState = Cast<APGKPlayerState>(Character->GetPlayerState());
		if (playerState)
		{
			playerState->AddWater(RestoreValue);
			UE_LOG(LogTemp, Warning, TEXT("Drank water! Restored %f"), RestoreValue);
		}
	}
	else if (ItemType == EItemType::Oxygen)
	{
		APGKPlayerState* playerState = Cast<APGKPlayerState>(Character->GetPlayerState());
		if (playerState)
		{
			playerState->AddOxygen(RestoreValue);
			UE_LOG(LogTemp, Warning, TEXT("Oxygen Restored %f"), RestoreValue);
		}
	}
}
