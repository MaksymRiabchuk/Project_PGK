#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PGKMenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PGK_API APGKMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
};
