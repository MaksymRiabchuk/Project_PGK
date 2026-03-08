// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PGKGameMode.generated.h"

/**
 *  Simple GameMode for a first person game
 */
UCLASS(abstract)
class APGKGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APGKGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
};



