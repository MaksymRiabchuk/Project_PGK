// Copyright Epic Games, Inc. All Rights Reserved.


#include "PGKPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "PGK.h"
#include "Character/PGKCameraManager.h"
#include "Widgets/PGKInteractionTextWidget.h"
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
		if (WBP_TimeClass)
		{
			WBP_TimeInstance = CreateWidget<UUserWidget>(this, WBP_TimeClass);
			if (WBP_TimeInstance)
			{
				WBP_TimeInstance->AddToViewport();
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("WBP_TimeInstanceClass not specified!"));
		}
		if (WBP_InteractionTextClass)
		{
			WBP_InteractionTextInstance = CreateWidget<UPGKInteractionTextWidget>(this, WBP_InteractionTextClass);
			if (WBP_InteractionTextInstance)
			{
				WBP_InteractionTextInstance->AddToViewport();
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("WBP_InteractionTextClass not specified!"));
		}
	}
	
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

void APGKPlayerController::ShowInteractionWidget(const FText& InteractText)
{
	if (!WBP_InteractionTextInstance && WBP_InteractionTextClass)
	{
		WBP_InteractionTextInstance = CreateWidget<UPGKInteractionTextWidget>(this, WBP_InteractionTextClass);
		if (WBP_InteractionTextInstance)
		{
			WBP_InteractionTextInstance->AddToViewport();
		}
	}

	if (WBP_InteractionTextInstance)
	{
		WBP_InteractionTextInstance->SetVisibility(ESlateVisibility::Visible);
		
		WBP_InteractionTextInstance->UpdateText(InteractText);
		
	}
}

void APGKPlayerController::HideInteractionWidget()
{
	if (WBP_InteractionTextInstance && WBP_InteractionTextInstance->IsVisible())
	{
		WBP_InteractionTextInstance->SetVisibility(ESlateVisibility::Hidden);
	}
}