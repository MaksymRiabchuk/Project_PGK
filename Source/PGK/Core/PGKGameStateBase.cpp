#include "Core/PGKGameStateBase.h"

#include "Net/UnrealNetwork.h"

APGKGameStateBase::APGKGameStateBase()
{
	TimeComponent = CreateDefaultSubobject<UPGKTimeComponent>(TEXT("TimeComponent"));
}

void APGKGameStateBase::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APGKGameStateBase, GlobalCO2);
	DOREPLIFETIME(APGKGameStateBase, GlobalO2);
	DOREPLIFETIME(APGKGameStateBase, GlobalTemperature);
	DOREPLIFETIME(APGKGameStateBase, WaterLevel);
}

void APGKGameStateBase::OnRep_GlobalTemperature()
{
	OnGlobalTemperatureChangedDelegate.Broadcast(GlobalTemperature);
}

void APGKGameStateBase::OnRep_GlobalCO2()
{
	OnGlobalCO2ChangedDelegate.Broadcast(GlobalCO2);
}

void APGKGameStateBase::SetGlobalTemp(double NewGlobalTemp)
{
	if (HasAuthority())
	{
		GlobalTemperature = NewGlobalTemp;
		OnRep_GlobalTemperature(); 
	}
}

void APGKGameStateBase::SetGlobalCO2(double NewGlobalCO2)
{
	if (HasAuthority())
	{
		GlobalCO2 = NewGlobalCO2;
		OnRep_GlobalCO2(); 
	}
}