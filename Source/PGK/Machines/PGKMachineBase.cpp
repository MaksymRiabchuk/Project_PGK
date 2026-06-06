// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved


#include "Machines/PGKMachineBase.h"
#include "Character/PGKCharacter.h"
#include "Core/Inventory/PGKInventoryComponent.h"
#include "Widgets/PGKMachineUserWidget.h"

APGKMachineBase::APGKMachineBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void APGKMachineBase::Interact_Implementation(APGKCharacter* InteractorCharacter)
{
	if (!InteractorCharacter) return;

	UPGKInventoryComponent* MyInventory = FindComponentByClass<UPGKInventoryComponent>();

	if (APGKPlayerController* PC = Cast<APGKPlayerController>(InteractorCharacter->GetController()))
	{
		SetOwner(PC);  // Required so the Client RPC routes to this player's connection
		Client_OpenMachineUI(PC);
	}
}

void APGKMachineBase::Client_OpenMachineUI_Implementation(APGKPlayerController* InteractorController)
{
	if (!InteractorController || !MachineWidgetClass) return;

	UPGKMachineUserWidget* MachineWidget = CreateWidget<UPGKMachineUserWidget>(InteractorController, MachineWidgetClass);
	if (MachineWidget)
	{
		MachineWidget->OwningMachine = this;
		MachineWidget->AddToViewport();
		MachineWidget->OnMachineInitialized();
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(MachineWidget->TakeWidget());
		InteractorController->SetInputMode(InputMode);
		InteractorController->SetShowMouseCursor(true);
	}
}

FPGKActorSaveData APGKMachineBase::GetActorSaveData_Implementation()
{
	FPGKActorSaveData Data;
	Data.ActorClass = GetClass();
	Data.Transform  = GetActorTransform();
	return Data;
}

void APGKMachineBase::ApplyActorSaveData_Implementation(const FPGKActorSaveData& SaveData)
{
	SetActorTransform(SaveData.Transform);
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

