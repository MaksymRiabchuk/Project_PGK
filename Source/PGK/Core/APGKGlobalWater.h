
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APGKGlobalWater.generated.h"

UCLASS()
class PGK_API APGKGlobalWater : public AActor
{
	GENERATED_BODY()
	
public:	
	APGKGlobalWater();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Water")
	class UStaticMeshComponent* WaterMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Water")
	class UPostProcessComponent* UnderwaterPostProcess;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Water")
	class UBoxComponent* UnderwaterVolume;

	UPROPERTY(EditAnywhere, Category = "Water Settings")
	float DecreaseRate = -1.0f;

	FTimerHandle WaterTimerHandle;
	void DecreaseWaterLevel();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY()
	ACharacter* LocalPlayerInWater = nullptr;
};
