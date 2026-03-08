// Copyright Epic Games, Inc. All Rights Reserved.

#include "PGKGameMode.h"

#include "PGKGameStateBase.h"

APGKGameMode::APGKGameMode()
{
	GameStateClass = APGKGameStateBase::StaticClass();
}

void APGKGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	RestartPlayer(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("Player Joined"));
	
}
