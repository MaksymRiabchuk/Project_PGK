// Fill out your copyright notice in the Description page of Project Settings.


#include "Resources/PGKIronOre.h"

// Sets default values
APGKIronOre::APGKIronOre()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APGKIronOre::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APGKIronOre::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APGKIronOre::Interact_Implementation(APGKCharacter* InteractorCharacter)
{
	if (InteractorCharacter)
	{
		Destroy();
	}
}

