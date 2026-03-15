#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PGKTimeComponent.h"
#include "PGKGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGlobalTemperatureChangedSignature, double, NewTemp);
/**
 * 
 */
UCLASS()
class PGK_API APGKGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

	public:
	APGKGameStateBase();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data", ReplicatedUsing=OnRep_GlobalTemperature)
	double GlobalTemperature = 368.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data", Replicated)
	double GlobalCO2 = 99.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data", Replicated)
	double GlobalO2 = 0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data", Replicated)
	double WaterLevel = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPGKTimeComponent> TimeComponent;

	UPROPERTY(BlueprintAssignable, Category = "Game Data|Events")
	FOnGlobalTemperatureChangedSignature OnGlobalTemperatureChangedDelegate;
	
	UFUNCTION()
	void OnRep_GlobalTemperature();
	void SetGlobalTemp(double NewGlobalTemp);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
