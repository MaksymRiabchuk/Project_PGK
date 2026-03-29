#include "Character/PGKPlayerState.h"

#include "PGK.h"
#include "PGKPlayerController.h"
#include "Core/PGKGameMode.h"
#include "Net/UnrealNetwork.h"

void APGKPlayerState::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(PlayerStatsTimerHandle, this, &APGKPlayerState::UpdatePlayerStats, 0.1f,
		                                       true);
	}
}

APGKPlayerState::APGKPlayerState()
{
	SetNetUpdateFrequency(10.0f);
}

void APGKPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APGKPlayerState, waterLevel);
	DOREPLIFETIME(APGKPlayerState, healthLevel);
	DOREPLIFETIME(APGKPlayerState, oxygenLevel);
}

void APGKPlayerState::UpdatePlayerStats()
{
	//PlayerStatsTimerRefreshRate
	float DeltaTime = 0.1f;

	waterLevel = FMath::Clamp(waterLevel - (WaterDecreaseRate * DeltaTime), 0.f, maxWaterLevel);
	healthLevel = FMath::Clamp(healthLevel - (HealthDecreaseRate * DeltaTime), 0.f, maxHealthLevel);

	if (bIsOxygenDecreasing)
	{
		oxygenLevel = FMath::Clamp(oxygenLevel - (OxygenDecreaseRate * DeltaTime), 0.f, maxOxygenLevel);
	}
	else
	{
		if (oxygenLevel < maxOxygenLevel)
		{
			oxygenLevel = FMath::Clamp(oxygenLevel + (OxygenRestoreRate * DeltaTime), 0.f, maxOxygenLevel);
		}
	}

	if (healthLevel <= 0 || waterLevel <= 0 || oxygenLevel <= 0)
	{
		if (APGKGameMode* gameMode = Cast<APGKGameMode>(GetWorld()->GetAuthGameMode()))
		{
			if (APGKPlayerController* playerController = Cast<APGKPlayerController>(GetOwner()))
			{
				ResetPlayerStats();
				gameMode->RespawnPlayer(playerController);
			}
		}
	}
}

void APGKPlayerState::AddHealth(float Amount)
{
	if (HasAuthority())
	{
		healthLevel = FMath::Clamp(healthLevel + Amount, 0.f, maxHealthLevel);
	}
}

void APGKPlayerState::AddWater(float Amount)
{
	if (HasAuthority())
	{
		waterLevel = FMath::Clamp(waterLevel + Amount, 0.f, maxWaterLevel);
	}
}

void APGKPlayerState::AddOxygen(float Amount)
{
	if (HasAuthority())
	{
		oxygenLevel = FMath::Clamp(oxygenLevel + Amount, 0.f, maxOxygenLevel);
	}
}

void APGKPlayerState::SetOxygenDecreasing(bool bIsDecreasing)
{
	if (HasAuthority())
	{
		bIsOxygenDecreasing = bIsDecreasing;
	}
}

void APGKPlayerState::ResetPlayerStats()
{
	if (HasAuthority())
	{
		waterLevel = maxWaterLevel;
		healthLevel = maxHealthLevel;
		oxygenLevel = maxOxygenLevel;
	}
}
