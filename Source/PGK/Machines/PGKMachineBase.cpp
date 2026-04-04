// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved


#include "Machines/PGKMachineBase.h"
#include "Character/PGKCharacter.h"
#include "Core/Inventory/PGKInventoryComponent.h"

APGKMachineBase::APGKMachineBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

// ВИКОНУЄТЬСЯ НА СЕРВЕРІ
void APGKMachineBase::Interact_Implementation(APGKCharacter* InteractorCharacter)
{
	if (!InteractorCharacter) return;

	UPGKInventoryComponent* MyInventory = FindComponentByClass<UPGKInventoryComponent>();

	if (APGKPlayerController* PC = Cast<APGKPlayerController>(InteractorCharacter->GetController()))
	{
		Client_OpenMachineUI(PC);
	}
}

void APGKMachineBase::Client_OpenMachineUI_Implementation(APGKPlayerController* InteractorController)
{
	if (!InteractorController || !MachineWidgetClass) return;

	UUserWidget* MachineWidget = CreateWidget<UUserWidget>(InteractorController, MachineWidgetClass);
	if (MachineWidget)
	{
		MachineWidget->AddToViewport();
        
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(MachineWidget->TakeWidget());
		InteractorController->SetInputMode(InputMode);
		InteractorController->SetShowMouseCursor(true);
	}
}

// Called when the game starts or when spawned
void APGKMachineBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APGKMachineBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

