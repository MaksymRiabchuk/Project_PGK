#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PGKWaterAnchor.generated.h"

UCLASS()
class PGK_API APGKWaterAnchor : public AActor
{
	GENERATED_BODY()

public:
	APGKWaterAnchor()
	{
		bReplicates = true;
		SetReplicatingMovement(true);
		PrimaryActorTick.bCanEverTick = false;
	}
};