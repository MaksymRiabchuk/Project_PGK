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
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Basic stats")
	float waterLevel = 100.f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Basic stats")
	float maxWaterLevel = 100.f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Basic stats")
	float healthLevel = 100.f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Basic stats")
	float maxHealthLevel = 100.f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Basic stats")
	float oxygenLevel = 100.f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Basic stats")
	float maxOxygenLevel = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Basic stats|Rates")
	float WaterDecreaseRate = 0.5f; 

	UPROPERTY(EditDefaultsOnly, Category = "Basic stats|Rates")
	float HealthDecreaseRate = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "Basic stats|Rates")
	float OxygenDecreaseRate = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Basic stats|Rates")
	float OxygenRestoreRate = 15.0f;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Basic stats|Actions")
	void AddWater( float Amount);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Basic stats|Actions")
	void AddHealth(float Amount);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Basic stats|Actions")
	void AddOxygen( float Amount);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Basic stats|Actions")
	void SetOxygenDecreasing(bool bIsDecreasing);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Basic stats|Actions")
	void ResetPlayerStats();

	void EnterBase();
    
	void LeaveBase();

	APGKPlayerState();
	

protected:
	FTimerHandle PlayerStatsTimerHandle;
	
	bool bIsOxygenDecreasing = true;

	void UpdatePlayerStats();

	int32 BaseOverlapCount = 0;
};
