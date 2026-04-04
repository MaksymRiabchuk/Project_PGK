// Fill out your copyright notice in the Description page of Project Settings.


#include "Machines/PGKWaterPurifier.h"
#include "Character/PGKCharacter.h"

// Sets default values
APGKWaterPurifier::APGKWaterPurifier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APGKWaterPurifier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APGKWaterPurifier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
FText APGKWaterPurifier::GetInteractText_Implementation()
{
	return FText::FromString(TEXT("Press E to pickup"));
}

void APGKWaterPurifier::Interact_Implementation(APGKCharacter* InteractorCharacter)
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
