#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/PGKCoreTypes.h"
#include "PGKTimeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeChangedSignature, const FGameDateTime&, CurrentTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDayChangedSignature, int32, NewDay);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSeasonChangedSignature, EGameSeason, NewSeason);

UCLASS(ClassGroup=(PGK), meta=(BlueprintSpawnableComponent))
class PGK_API UPGKTimeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPGKTimeComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time System|Settings", meta = (ClampMin = "0.1"))
	float DayLengthInMinutes = 24.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time System|Settings")
	bool bPaused = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_DateTime, Category = "Time System|State")
	FGameDateTime DateTime;

	UPROPERTY(BlueprintAssignable, Category = "Time System|Events")
	FOnTimeChangedSignature OnTimeChanged;

	UPROPERTY(BlueprintAssignable, Category = "Time System|Events")
	FOnDayChangedSignature OnDayChanged;

	UPROPERTY(BlueprintAssignable, Category = "Time System|Events")
	FOnSeasonChangedSignature OnSeasonChanged;

	UFUNCTION(BlueprintPure, Category = "Time System")
	int32 GetCurrentHour() const { return DateTime.GetHour(); }

	UFUNCTION(BlueprintPure, Category = "Time System")
	int32 GetCurrentMinute() const { return DateTime.GetMinute(); }

	UFUNCTION(BlueprintPure, Category = "Time System")
	FORCEINLINE FGameDateTime& GetDateTime() { return DateTime; }

	UFUNCTION(BlueprintPure, Category = "Time System")
	FORCEINLINE EGameSeason GetCurrentSeason() const { return DateTime.Season; }

	UFUNCTION(BlueprintPure, Category = "Time System")
	float GetNormalizedTimeOfDay() const { return DateTime.TimeOfDay / 24.0f; }

	UFUNCTION(BlueprintCallable, Category = "Time System")
	void SetDateTime(const FGameDateTime& NewDateTime);

	UFUNCTION(BlueprintCallable, Category = "Time System")
	void SetPaused(bool bNewPaused) { bPaused = bNewPaused; }

	UFUNCTION(BlueprintCallable, Category = "Time System")
	void AdvanceTime(float Hours);

protected:
	UFUNCTION()
	void OnRep_DateTime();

private:
	void AdvanceDay();
	void UpdateSeason();

	static constexpr int32 DaysPerWeek = 7;
	static constexpr int32 MonthsPerYear = 6;
	static constexpr float DaysPerSeason = 10.5f;
};