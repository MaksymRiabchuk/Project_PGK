// Copyright Epic Games, Inc. All Rights Reserved.

#include "PGKGameMode.h"

#include "PGKGameStateBase.h"
#include "Character/PGKPlayerController.h"
#include "Character/PGKPlayerState.h"

APGKGameMode::APGKGameMode()
{
	GameStateClass = APGKGameStateBase::StaticClass();
	PlayerStateClass = APGKPlayerState::StaticClass();
}

void APGKGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	RestartPlayer(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("Player Joined"));
}

void APGKGameMode::RespawnPlayer(AController* Controller)
{
	if (Controller)
	{
		if (APawn* PawnToDestroy = Controller->GetPawn())
		{
			PawnToDestroy->Destroy();
		}

		RestartPlayer(Controller);
		APGKPlayerController * PC = Cast<APGKPlayerController>(Controller);
		if (PC)
		{
			PC->ResetUI();
			FInputModeGameOnly InputModeData;
			PC->SetInputMode(InputModeData);
			PC->SetShowMouseCursor(false);
		}
		
	}
}
