// Copyright Epic Games, Inc. All Rights Reserved.


#include "PGKPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "PGK.h"
#include "Character/PGKCameraManager.h"
#include "Widgets/Input/SVirtualJoystick.h"

APGKPlayerController::APGKPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = APGKCameraManager::StaticClass();
}

void APGKPlayerController::BeginPlay()
{
	Super::BeginPlay();
 
	
	// only spawn touch controls on local player controllers
	if (ShouldUseTouchControls() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);
		if (MobileControlsWidget)
		{
			MobileControlsWidget->AddToPlayerScreen(0);
		} else {
			UE_LOG(LogPGK, Error, TEXT("Could not spawn mobile controls widget."));
		}
	}

	FInputModeGameOnly InputModeData;
	SetInputMode(InputModeData);
	bShowMouseCursor = false;

	if (IsLocalPlayerController())
	{
		if (HUDWidgetClass)
		{
			HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
            
			if (HUDWidgetInstance)
			{
				HUDWidgetInstance->AddToViewport();
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("HUDWidgetClass not specified!"));
		}
		if (WBP_Time)
		{
			WBP_TimeInstance = CreateWidget<UUserWidget>(this, WBP_Time);
            
			if (WBP_TimeInstance)
			{
				WBP_TimeInstance->AddToViewport();
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("WBP_TimeInstanceClass not specified!"));
		}
	}
	UE_LOG(LogPGK, Display, TEXT("Added HUD"));
	
}

void APGKPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
	
}

bool APGKPlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}
