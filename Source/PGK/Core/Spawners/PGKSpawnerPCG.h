#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PGKSpawnerPCG.generated.h"

class UBoxComponent;

UCLASS()
class PGK_API ASpawnerPCG : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnerPCG();

	UPROPERTY(VisibleAnywhere, Category = "PCG Settings")
	UBoxComponent* SpawnArea;

	// --- NEW: Array of Meshes instead of just one ---
	UPROPERTY(EditAnywhere, Category = "PCG Settings|Meshes")
	TArray<UStaticMesh*> MeshesToSpawn;

	UPROPERTY(EditAnywhere, Category = "PCG Settings|Rules")
	float Spacing = 200.0f;

	UPROPERTY(EditAnywhere, Category = "PCG Settings|Rules")
	float HeightThreshold = 500.0f; 

	// --- NEW: Density / Spawn Chance (0.0 to 1.0) ---
	UPROPERTY(EditAnywhere, Category = "PCG Settings|Rules", meta=(ClampMin="0.0", ClampMax="1.0"))
	float SpawnProbability = 0.5f; 

	// --- NEW: Random Transform Settings ---
	UPROPERTY(EditAnywhere, Category = "PCG Settings|Transform")
	bool bRandomizeRotation = true;

	UPROPERTY(EditAnywhere, Category = "PCG Settings|Transform")
	float MinScale = 0.8f;

	UPROPERTY(EditAnywhere, Category = "PCG Settings|Transform")
	float MaxScale = 1.2f;

protected:
	virtual void BeginPlay() override;

	void GenerateLevel();
};