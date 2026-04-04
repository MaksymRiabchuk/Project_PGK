// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved


#include "Widgets/PGKBaseUserWidget.h"
#include "Input/Reply.h"
#include "GameFramework/PlayerController.h"

FReply UPGKBaseUserWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		CloseWidget();
		return FReply::Handled(); 
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UPGKBaseUserWidget::CloseWidget()
{
	if (bDestroyOnClose)
	{
		RemoveFromParent();
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}	
	if (APlayerController* PC = GetOwningPlayer())
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(false);
	}
}