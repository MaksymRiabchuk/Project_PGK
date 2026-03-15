#include "PGKTimeHandler.h"
#include "Math/UnrealMathUtility.h"

APGKTimeHandler::APGKTimeHandler()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APGKTimeHandler::BeginPlay()
{
	Super::BeginPlay();
	UpdateSeason();
}

void APGKTimeHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DayLengthInMinutes <= 0.0f) return;

	float HoursPerSecond = 24.0f / (DayLengthInMinutes * 60.0f);

	CurrentTimeOfDay += (HoursPerSecond * DeltaTime);

	if (CurrentTimeOfDay >= 24.0f)
	{
		CurrentTimeOfDay -= 24.0f;
		AdvanceDay();
	}
}

void APGKTimeHandler::AdvanceDay()
{
	CurrentDay++;

	if (CurrentDay > 7)
	{
		CurrentDay = 1;
		CurrentMonth++;

		if (CurrentMonth > 6)
		{
			CurrentMonth = 1;
			CurrentYear++;
		}
	}

	UpdateSeason();
}

void APGKTimeHandler::UpdateSeason()
{
	int32 YearlyDayIndex = ((CurrentMonth - 1) * 7) + (CurrentDay - 1);
	int32 SeasonIndex = FMath::FloorToInt(YearlyDayIndex / 10.5f);
	CurrentSeason = static_cast<EGameSeason>(SeasonIndex);
}

int32 APGKTimeHandler::GetCurrentHour() const
{
	return FMath::FloorToInt(CurrentTimeOfDay);
}

int32 APGKTimeHandler::GetCurrentMinute() const
{
	float FractionalHour = CurrentTimeOfDay - GetCurrentHour();
	return FMath::FloorToInt(FractionalHour * 60.0f);
}

int32 APGKTimeHandler::GetCurrentDay() const
{
	return CurrentDay;
}

int32 APGKTimeHandler::GetCurrentMonth() const
{
	return CurrentMonth;
}

int32 APGKTimeHandler::GetCurrentYear() const
{
	return CurrentYear;
}

EGameSeason APGKTimeHandler::GetCurrentSeason() const
{
	return CurrentSeason;
}