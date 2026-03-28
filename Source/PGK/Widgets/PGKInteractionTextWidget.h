// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGKInteractionTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class PGK_API UPGKInteractionTextWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void UpdateText(const FText& NewText);
};
