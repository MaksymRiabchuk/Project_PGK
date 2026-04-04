// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGKBaseUserWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PGK_API UPGKBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION(BlueprintCallable, Category = "UI")
	virtual void CloseWidget();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI Settings")
	bool bDestroyOnClose = true;
};
