#include "PGKTimeComponent.h"
#include "Net/UnrealNetwork.h"

UPGKTimeComponent::UPGKTimeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UPGKTimeComponent::BeginPlay()
{
	Super::BeginPlay();
	UpdateSeason();
}

void UPGKTimeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bPaused || DayLengthInMinutes <= 0.0f)
	{
		return;
	}

	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	const float HoursPerSecond = 24.0f / (DayLengthInMinutes * 60.0f);
	DateTime.TimeOfDay += HoursPerSecond * DeltaTime;

	if (DateTime.TimeOfDay >= 24.0f)
	{
		DateTime.TimeOfDay -= 24.0f;
		AdvanceDay();
	}

	OnTimeChanged.Broadcast(DateTime);
}

void UPGKTimeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPGKTimeComponent, DateTime);
}

void UPGKTimeComponent::OnRep_DateTime()
{
	OnTimeChanged.Broadcast(DateTime);
}

void UPGKTimeComponent::AdvanceDay()
{
	DateTime.Day++;

	if (DateTime.Day > DaysPerWeek)
	{
		DateTime.Day = 1;
		DateTime.Month++;

		if (DateTime.Month > MonthsPerYear)
		{
			DateTime.Month = 1;
			DateTime.Year++;
		}
	}

	UpdateSeason();
	OnDayChanged.Broadcast(DateTime.Day);
}

void UPGKTimeComponent::UpdateSeason()
{
	const int32 YearlyDayIndex = ((DateTime.Month - 1) * DaysPerWeek) + (DateTime.Day - 1);
	const int32 SeasonIndex = FMath::Clamp(FMath::FloorToInt(YearlyDayIndex / DaysPerSeason), 0, 3);
	
	const EGameSeason NewSeason = static_cast<EGameSeason>(SeasonIndex);
	if (DateTime.Season != NewSeason)
	{
		DateTime.Season = NewSeason;
		OnSeasonChanged.Broadcast(DateTime.Season);
	}
}

void UPGKTimeComponent::SetDateTime(const FGameDateTime& NewDateTime)
{
	DateTime = NewDateTime;
	UpdateSeason();
	OnTimeChanged.Broadcast(DateTime);
}

void UPGKTimeComponent::AdvanceTime(float Hours)
{
	DateTime.TimeOfDay += Hours;
	
	while (DateTime.TimeOfDay >= 24.0f)
	{
		DateTime.TimeOfDay -= 24.0f;
		AdvanceDay();
	}
	while (DateTime.TimeOfDay < 0.0f)
	{
		DateTime.TimeOfDay += 24.0f;
	}
	
	OnTimeChanged.Broadcast(DateTime);
}