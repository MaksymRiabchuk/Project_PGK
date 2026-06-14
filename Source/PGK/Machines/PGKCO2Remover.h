// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PGKMachineBase.h"
#include "Core/Interfaces/PGKInteractableInterface.h"
#include "GameFramework/Actor.h"
#include "PGKCO2Remover.generated.h"

UCLASS()
class PGK_API APGKCO2Remover : public APGKMachineBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APGKCO2Remover();
	virtual FText GetInteractText_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CO2 Remover")
	double CO2RemoveAmount = 0.0001;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CO2 Remover")
	float RemoveInterval = 10.f;

private:
	FTimerHandle CO2TimerHandle;

	void RemoveCO2Tick();

};
