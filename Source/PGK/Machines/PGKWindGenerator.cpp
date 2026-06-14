// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved


#include "Machines/PGKWindGenerator.h"
#include "Character/PGKCharacter.h"

// Sets default values
APGKWindGenerator::APGKWindGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APGKWindGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APGKWindGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
FText APGKWindGenerator::GetInteractText_Implementation()
{
	return FText::FromString(TEXT("Press E to open Wind Generator"));
}
