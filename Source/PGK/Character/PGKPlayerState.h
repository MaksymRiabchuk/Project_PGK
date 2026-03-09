#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PGKPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PGK_API APGKPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	public:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Basic stats")
	float waterLevel=100.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Basic stats")
	float maxWaterLevel = 100.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Basic stats")
	float healthLevel=100.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Basic stats")
	float maxHealthLevel=100.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Basic stats")
	float oxygenLevel=50.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Basic stats")
	float maxOxygenLevel=100.f;
};
