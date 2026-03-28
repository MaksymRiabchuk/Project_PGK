// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/PGKPickupItem.h"
#include "Core/Interfaces/PGKInteractableInterface.h"
#include "GameFramework/Actor.h"
#include "PGKIronOre.generated.h"

UCLASS()
class PGK_API APGKIronOre : public APGKPickupItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APGKIronOre();

	virtual void Interact_Implementation(class APGKCharacter* InteractorCharacter) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
