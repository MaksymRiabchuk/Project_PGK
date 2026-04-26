 // Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved


#include "Buildings/PGKChest.h"
#include "Character/PGKCharacter.h"

// Sets default values
APGKChest::APGKChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APGKChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APGKChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
FText APGKChest::GetInteractText_Implementation()
{
	return FText::FromString(TEXT("Press E to pickup"));
}

void APGKChest::Interact_Implementation(APGKCharacter* InteractorCharacter)
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
