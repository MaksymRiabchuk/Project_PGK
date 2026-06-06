// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved

#include "Buildings/PGKChestLootTable.h"

TArray<FPGKLootEntry> UPGKChestLootTable::RollLoot(float Progress) const
{
	// Filter entries available at the current progress level
	TArray<const FPGKLootEntry*> Eligible;
	float TotalWeight = 0.f;

	for (const FPGKLootEntry& Entry : Entries)
	{
		if (Entry.Item && Entry.Weight > 0.f && Progress >= Entry.MinProgress)
		{
			Eligible.Add(&Entry);
			TotalWeight += Entry.Weight;
		}
	}

	if (Eligible.IsEmpty() || TotalWeight <= 0.f)
		return {};

	const int32 NumItems = FMath::RandRange(MinItemsToSpawn, MaxItemsToSpawn);

	TArray<FPGKLootEntry> Result;
	Result.Reserve(NumItems);

	for (int32 i = 0; i < NumItems; ++i)
	{
		// Weighted random pick
		float Roll = FMath::FRandRange(0.f, TotalWeight);
		float Accumulated = 0.f;

		for (const FPGKLootEntry* Entry : Eligible)
		{
			Accumulated += Entry->Weight;
			if (Roll <= Accumulated)
			{
				FPGKLootEntry Picked = *Entry;
				Picked.MinQuantity = FMath::RandRange(Entry->MinQuantity, Entry->MaxQuantity);
				Picked.MaxQuantity = Picked.MinQuantity;
				Result.Add(Picked);
				break;
			}
		}
	}

	return Result;
}