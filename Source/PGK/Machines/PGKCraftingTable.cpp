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



void APGKCraftingTable::BeginPlay()
{
	Super::BeginPlay();

}

void APGKCraftingTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}