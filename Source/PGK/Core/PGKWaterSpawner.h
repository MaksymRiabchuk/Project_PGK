#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PGKWaterSpawner.generated.h"

class APGKWaterAnchor;

UCLASS()
class PGK_API APGKWaterSpawner : public AActor
{
	GENERATED_BODY()

public:
	APGKWaterSpawner();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawnArea;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	AActor* TargetWaterPlane;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<AActor> ClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (ClampMin = "1"))
	int32 AmountToSpawn = 10;

private:
	void SpawnOnWater();

	UPROPERTY()
	APGKWaterAnchor* WaterAnchor;
};