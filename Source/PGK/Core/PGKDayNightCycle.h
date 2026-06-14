// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PGKDayNightCycle.generated.h"

class UDirectionalLightComponent;
class UPGKTimeComponent;

UCLASS()
class PGK_API APGKDayNightCycle : public AActor
{
	GENERATED_BODY()

public:
	APGKDayNightCycle();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Night Cycle")
	AActor* WeatherSystem;

	UPROPERTY(EditAnywhere, Category = "Day Night Cycle")
	float DayLightIntensity = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Day Night Cycle")
	float NightLightIntensity = 0.0f;

private:
	UPROPERTY()
	UPGKTimeComponent* TimeComp;

	UPROPERTY()
	UDirectionalLightComponent* SunLightComp;

	float SunYaw = 0.0f;

	void UpdateSun(float TimeOfDay);
};
