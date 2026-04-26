// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PGKSaveSettings.generated.h"

/**
 * 
 */
UCLASS()
class PGK_API UPGKSaveSettings : public USaveGame
{
	GENERATED_BODY()
public:
	UPGKSaveSettings();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio")
	float MusicVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio")
	float SFXVolume;
};
