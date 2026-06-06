// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved

#include "Buildings/PGKChestSpawnPoint.h"
#include "Buildings/PGKChest.h"
#include "Buildings/PGKChestLootTable.h"
#include "Core/Inventory/PGKInventoryComponent.h"
#include "Core/Inventory/PGKItemData.h"

#if WITH_EDITORONLY_DATA
#include "Components/BillboardComponent.h"
#endif

APGKChestSpawnPoint::APGKChestSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;

#if WITH_EDITORONLY_DATA
	EditorSprite = CreateDefaultSubobject<UBillboardComponent>(TEXT("EditorSprite"));
	SetRootComponent(EditorSprite);
#endif
}

APGKChest* APGKChestSpawnPoint::TrySpawnChest(float Progress, TSubclassOf<APGKChest> ChestClass, UPGKChestLootTable* DefaultLootTable)
{
	if (!ChestClass) return nullptr;

	// Roll against spawn chance
	if (FMath::FRand() > SpawnChance) return nullptr;

	UPGKChestLootTable* Loot = LootTableOverride ? LootTableOverride : DefaultLootTable;
	if (!Loot) return nullptr;

	// Spawn the chest at this marker's transform
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APGKChest* Chest = GetWorld()->SpawnActor<APGKChest>(ChestClass, GetActorTransform(), Params);
	if (!Chest) return nullptr;

	// Mark as saveable world object and enable auto-delete when emptied
	Chest->Tags.Add(FName("PlayerPlaced"));
	Chest->bDestroyWhenEmpty = true;

	// Fill inventory with rolled loot
	UPGKInventoryComponent* Inv = Chest->GetInventoryComponent();
	if (Inv)
	{
		for (const FPGKLootEntry& Entry : Loot->RollLoot(Progress))
		{
			if (Entry.Item)
			{
				Inv->Server_AddItem(Entry.Item, Entry.MinQuantity);
			}
		}
	}

	return Chest;
}