#include "Core/PGKGameStateBase.h"

#include "Net/UnrealNetwork.h"

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

void APGKGameStateBase::SetGlobalTemp(double NewGlobalTemp)
{
	if (HasAuthority())
	{
		GlobalTemperature = NewGlobalTemp;
		OnRep_GlobalTemperature(); 
	}
}