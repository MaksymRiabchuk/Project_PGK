// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Core/PGKPickupItem.h"
#include "PGKAluminumOre.generated.h"

/**
 * 
 */
UCLASS()
class PGK_API APGKAluminumOre : public APGKPickupItem
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	APGKAluminumOre();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
