// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PGKHologramPreview.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UMaterialInstanceDynamic;

UCLASS()
class PGK_API APGKHologramPreview : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APGKHologramPreview();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* HologramMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hologram | Materials")
	UMaterialInterface* BaseHologramMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hologram | Colors")
	FLinearColor ValidColor = FLinearColor::Green;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hologram | Colors")
	FLinearColor InvalidColor = FLinearColor::Red;

	void UpdateHologramState(bool bCanPlace);

	FORCEINLINE bool IsPlacementValid() const { return bCurrentPlacementValid; }

private:
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterialInstance;

	bool bCurrentPlacementValid = true;

	int32 OverlappingBlockingActors = 0;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex);
};
