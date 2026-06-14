// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved


#include "PGKMachineUserWidget.h"
#include "Machines/PGKMachineBase.h"

void UPGKMachineUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (OwningMachine)
	{
		OwningMachine->OnDestroyed.AddDynamic(this, &UPGKMachineUserWidget::OnOwningMachineDestroyed);
	}
}

void UPGKMachineUserWidget::NativeDestruct()
{
	if (OwningMachine)
	{
		OwningMachine->OnDestroyed.RemoveDynamic(this, &UPGKMachineUserWidget::OnOwningMachineDestroyed);
	}

	Super::NativeDestruct();
}

void UPGKMachineUserWidget::OnOwningMachineDestroyed(AActor* DestroyedActor)
{
	CloseWidget();
}
