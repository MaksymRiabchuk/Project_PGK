#pragma once

#include "CoreMinimal.h"
#include "PGKCoreTypes.generated.h"

UENUM(BlueprintType)
enum class EGameSeason : uint8
{
	Winter UMETA(DisplayName = "Winter"),
	Spring UMETA(DisplayName = "Spring"),
	Summer UMETA(DisplayName = "Summer"),
	Autumn UMETA(DisplayName = "Autumn")
};

USTRUCT(BlueprintType)
struct FGameDateTime
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	int32 Year = 2026;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	int32 Month = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	int32 Day = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float TimeOfDay = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time")
	EGameSeason Season = EGameSeason::Winter;

	int32 GetHour() const { return FMath::FloorToInt(TimeOfDay); }
	int32 GetMinute() const { return FMath::FloorToInt((TimeOfDay - GetHour()) * 60.0f); }
	
	bool operator==(const FGameDateTime& Other) const
	{
		return Year == Other.Year && Month == Other.Month && Day == Other.Day 
			&& FMath::IsNearlyEqual(TimeOfDay, Other.TimeOfDay);
	}
};