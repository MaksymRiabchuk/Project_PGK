// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved

#include "Core/PGKDayNightCycle.h"
#include "Core/PGKTimeComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "GameFramework/GameStateBase.h"

APGKDayNightCycle::APGKDayNightCycle()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = false;
}

void APGKDayNightCycle::BeginPlay()
{
	Super::BeginPlay();

	if (AGameStateBase* GS = GetWorld()->GetGameState())
	{
		TimeComp = GS->FindComponentByClass<UPGKTimeComponent>();
	}

	if (WeatherSystem)
	{
		SunLightComp = WeatherSystem->FindComponentByClass<UDirectionalLightComponent>();
		if (SunLightComp)
		{
			SunYaw = SunLightComp->GetComponentRotation().Yaw;
		}
	}

	if (TimeComp && SunLightComp)
	{
		UpdateSun(TimeComp->GetDateTime().TimeOfDay);
	}
}

void APGKDayNightCycle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TimeComp && SunLightComp)
	{
		UpdateSun(TimeComp->GetDateTime().TimeOfDay);
	}
}

void APGKDayNightCycle::UpdateSun(float TimeOfDay)
{
	const float SunPitch = 90.0f - (TimeOfDay / 24.0f) * 360.0f;

	SunLightComp->SetWorldRotation(FRotator(SunPitch, SunYaw, 0.0f));

	const float SinValue = FMath::Sin(FMath::DegreesToRadians(-SunPitch));
	const float Intensity = FMath::Lerp(NightLightIntensity, DayLightIntensity, FMath::Max(0.0f, SinValue));
	SunLightComp->SetIntensity(Intensity);
}
