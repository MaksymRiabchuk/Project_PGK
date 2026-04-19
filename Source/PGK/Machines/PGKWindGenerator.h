// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Machines/PGKMachineBase.h"
#include "PGKWindGenerator.generated.h"

/**
 * 
 */
UCLASS()
class PGK_API APGKWindGenerator : public APGKMachineBase
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	APGKWindGenerator();

	virtual FText GetInteractText_Implementation() override;
	virtual void Interact_Implementation(class APGKCharacter* InteractorCharacter) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
