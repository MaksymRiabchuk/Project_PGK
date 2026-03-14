#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PGKTimeHandler.generated.h"

UENUM(BlueprintType)
enum class EGameSeason : uint8
{
	Winter UMETA(DisplayName = "Winter"),
	Spring UMETA(DisplayName = "Spring"),
	Summer UMETA(DisplayName = "Summer"),
	Autumn UMETA(DisplayName = "Autumn")
};

UCLASS()
class PGK_API APGKTimeHandler : public AActor
{
	GENERATED_BODY()

public:
	APGKTimeHandler();

protected:
	virtual void BeginPlay() override;
	void AdvanceDay();
	void UpdateSeason();

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time System|Time")
	float DayLengthInMinutes = 24.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time System|Time")
	float CurrentTimeOfDay = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time System|Calendar")
	int32 CurrentDay = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time System|Calendar")
	int32 CurrentMonth = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time System|Calendar")
	int32 CurrentYear = 2026;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time System|Calendar")
	EGameSeason CurrentSeason = EGameSeason::Winter;

	UFUNCTION(BlueprintPure, Category = "Time System")
	int32 GetCurrentHour() const;

	UFUNCTION(BlueprintPure, Category = "Time System")
	int32 GetCurrentMinute() const;

	UFUNCTION(BlueprintPure, Category = "Time System")
	int32 GetCurrentDay() const;

	UFUNCTION(BlueprintPure, Category = "Time System")
	int32 GetCurrentMonth() const;

	UFUNCTION(BlueprintPure, Category = "Time System")
	int32 GetCurrentYear() const;

	UFUNCTION(BlueprintPure, Category = "Time System")
	EGameSeason GetCurrentSeason() const;
};