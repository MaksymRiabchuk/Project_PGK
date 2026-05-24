// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Core/Types/PGKSaveTypes.h"
#include "PGKSaveableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPGKSaveableInterface : public UInterface
{
	GENERATED_BODY()
};

// Implemented by any placed actor whose state must survive a save/load cycle.
// APGKMachineBase provides a default implementation (transform + class).
// Actors with extra state (e.g. APGKChest) override both functions.
class PGK_API IPGKSaveableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	FPGKActorSaveData GetActorSaveData();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void ApplyActorSaveData(const FPGKActorSaveData& SaveData);
};