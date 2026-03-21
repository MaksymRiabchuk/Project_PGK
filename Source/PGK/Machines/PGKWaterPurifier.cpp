// Fill out your copyright notice in the Description page of Project Settings.


#include "Machines/PGKWaterPurifier.h"

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
FString APGKWaterPurifier::GetInteractText_Implementation()
{
	return TEXT("Press E to open");
}

void APGKWaterPurifier::Interact_Implementation(APGKCharacter* InteractorCharacter)
{
	if (InteractorCharacter)
	{
		Destroy();
	}
}
