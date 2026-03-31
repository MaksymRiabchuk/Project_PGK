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

	InitializeUI();
	
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

void APGKPlayerController::ShowInventoryWidget()
{
	if (WBP_InventoryInstance && !WBP_InventoryInstance->IsVisible())
	{
		WBP_InventoryInstance->SetVisibility(ESlateVisibility::Visible);
		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(WBP_InventoryInstance->TakeWidget());
		SetInputMode(InputModeData);
		bShowMouseCursor = true;
	}
}

void APGKPlayerController::HideInventoryWidget()
{
	if (WBP_InventoryInstance && WBP_InventoryInstance->IsVisible())
	{
		WBP_InventoryInstance->SetVisibility(ESlateVisibility::Hidden);
		FInputModeGameOnly InputModeData;
		SetInputMode(InputModeData);
		bShowMouseCursor = false;
	}
}

void APGKPlayerController::InitializeUI()
{
    if (!IsLocalPlayerController()) return;

    // spawn the mobile controls widget
    if (ShouldUseTouchControls())
    {
       MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);
       if (MobileControlsWidget)
       {
          MobileControlsWidget->AddToPlayerScreen(0);
       } 
       else 
       {
          UE_LOG(LogPGK, Error, TEXT("Could not spawn mobile controls widget."));
       }
    }

    if (HUDWidgetClass)
    {
       HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
       if (HUDWidgetInstance) HUDWidgetInstance->AddToViewport();
    }

    if (WBP_TimeClass)
    {
       WBP_TimeInstance = CreateWidget<UUserWidget>(this, WBP_TimeClass);
       if (WBP_TimeInstance) WBP_TimeInstance->AddToViewport();
    }

    if (WBP_InteractionTextClass)
    {
       WBP_InteractionTextInstance = CreateWidget<UPGKInteractionTextWidget>(this, WBP_InteractionTextClass);
       if (WBP_InteractionTextInstance) WBP_InteractionTextInstance->AddToViewport();
    }

    if (WBP_InventoryClass)
    {
       WBP_InventoryInstance = CreateWidget<UUserWidget>(this, WBP_InventoryClass);
       if (WBP_InventoryInstance)
       {
          WBP_InventoryInstance->AddToViewport();
          WBP_InventoryInstance->SetVisibility(ESlateVisibility::Hidden);
       }
    }
}

void APGKPlayerController::ResetUI()
{
    if (MobileControlsWidget)
    {
        MobileControlsWidget->RemoveFromParent();
        MobileControlsWidget = nullptr;
    }
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }
    if (WBP_TimeInstance)
    {
        WBP_TimeInstance->RemoveFromParent();
        WBP_TimeInstance = nullptr;
    }
    if (WBP_InteractionTextInstance)
    {
        WBP_InteractionTextInstance->RemoveFromParent();
        WBP_InteractionTextInstance = nullptr;
    }
    if (WBP_InventoryInstance)
    {
        WBP_InventoryInstance->RemoveFromParent();
        WBP_InventoryInstance = nullptr;
    }

    InitializeUI();
}