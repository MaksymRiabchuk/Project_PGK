// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/PGKBaseUserWidget.h"
#include "PGKMachineUserWidget.generated.h"

class APGKMachineBase;
/**
 * 
 */
UCLASS()
class PGK_API UPGKMachineUserWidget : public UPGKBaseUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Machine")
	APGKMachineBase* OwningMachine;

	UFUNCTION(BlueprintImplementableEvent, Category = "Machine")
	void OnMachineInitialized();	
};
