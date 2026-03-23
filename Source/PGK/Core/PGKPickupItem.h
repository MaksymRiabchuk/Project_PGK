// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PGKInteractableInterface.h"
#include "PGKPickupItem.generated.h"

class UPGKItemData;

UCLASS()
class PGK_API APGKPickupItem : public AActor, public IPGKInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APGKPickupItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UPGKItemData* ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 Amount = 1;

	virtual FString GetInteractText_Implementation() override;
	virtual void Interact_Implementation(class APGKCharacter* InteractorCharacter) override;

};
