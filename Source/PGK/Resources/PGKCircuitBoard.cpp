// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved


#include "Resources/PGKCircuitBoard.h"

APGKCircuitBoard::APGKCircuitBoard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APGKCircuitBoard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APGKCircuitBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}