// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Core/PGKPickupItem.h"
#include "PGKCircuitBoard.generated.h"

/**
 * 
 */
UCLASS()
class PGK_API APGKCircuitBoard : public APGKPickupItem
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	APGKCircuitBoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
