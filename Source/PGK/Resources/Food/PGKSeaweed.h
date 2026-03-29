// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/PGKPickupItem.h"
#include "GameFramework/Actor.h"
#include "PGKSeaweed.generated.h"

UCLASS()
class PGK_API APGKSeaweed : public APGKPickupItem
{	
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APGKSeaweed();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
