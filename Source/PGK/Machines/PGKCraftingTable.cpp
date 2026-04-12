// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved
#include "Machines/PGKCraftingTable.h"
#include "Character/PGKCharacter.h"

FText APGKCraftingTable::GetInteractText_Implementation()
{
	return FText::FromString(TEXT("Press E to open Crafting Table"));
}

APGKCraftingTable::APGKCraftingTable()
{
	PrimaryActorTick.bCanEverTick = true;
}


void APGKCraftingTable::Interact_Implementation(APGKCharacter* InteractorCharacter)
{
	if (InteractorCharacter)
	{
		APGKPlayerController* PC = Cast<APGKPlayerController>(InteractorCharacter->GetController());
		if (PC)
		{
			Client_OpenMachineUI(PC);			
		}
	}
}

void APGKCraftingTable::BeginPlay()
{
	Super::BeginPlay();

}

void APGKCraftingTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}