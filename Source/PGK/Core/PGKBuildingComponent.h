// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PGKBuildingComponent.generated.h"
class UPGKBuildingData;
class APGKHologramPreview;
class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PGK_API UPGKBuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPGKBuildingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting")
	TArray<UPGKBuildingData*> AvailableBuildings;
	
	UFUNCTION(BlueprintCallable, Category = "Building")
	void StartBuilding(UPGKBuildingData* BuildingData);

	UFUNCTION(BlueprintCallable, Category = "Building")
	void StopBuilding();

	UFUNCTION(BlueprintCallable, Category = "Building")
	void TryConstruct();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	float BuildRange = 600.0f;

	UFUNCTION(BlueprintCallable, Category = "Building")
	void RotateHologram(float Direction);
private:
	UPROPERTY()
	UPGKBuildingData* CurrentBuildingData;

	UPROPERTY()
	APGKHologramPreview* CurrentHologram;

	UFUNCTION(Server, Reliable)
	void Server_ConstructBuilding(UPGKBuildingData* BuildingData, FVector Location, FRotator Rotation);

	float CurrentHologramYaw = 0.0f;
};
