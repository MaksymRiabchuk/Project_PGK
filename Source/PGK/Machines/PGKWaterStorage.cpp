// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved


#include "Machines/PGKWaterStorage.h"
#include "Character/PGKCharacter.h"

// Sets default values
APGKWaterStorage::APGKWaterStorage()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APGKWaterStorage::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APGKWaterStorage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
FText APGKWaterStorage::GetInteractText_Implementation()
{
	return FText::FromString(TEXT("Press E to open Water Storage"));
}

void APGKWaterStorage::Interact_Implementation(APGKCharacter* InteractorCharacter)
{
	if (InteractorCharacter)
	{
		APGKPlayerController* PC = Cast<APGKPlayerController>(InteractorCharacter->GetController());
		if (PC)
		{
			Client_OpenMachineUI_Implementation(PC);			
		}
	}
}
