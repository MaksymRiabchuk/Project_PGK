// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Interfaces/PGKInteractableInterface.h"
#include "GameFramework/Actor.h"
#include "PGKCO2Remover.generated.h"

UCLASS()
class PGK_API APGKCO2Remover : public AActor, public IPGKInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APGKCO2Remover();
	virtual FText GetInteractText_Implementation() override;
	virtual void Interact_Implementation(class APGKCharacter* InteractorCharacter) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
