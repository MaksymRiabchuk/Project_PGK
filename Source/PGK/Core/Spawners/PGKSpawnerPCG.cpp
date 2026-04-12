#include "PGKSpawnerPCG.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

ASpawnerPCG::ASpawnerPCG()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = SpawnArea;
	SpawnArea->SetBoxExtent(FVector(1000.0f, 1000.0f, 5000.0f));
	SpawnArea->SetCollisionProfileName(TEXT("NoCollision"));
}

void ASpawnerPCG::BeginPlay()
{
	Super::BeginPlay();
	GenerateLevel();
}

void ASpawnerPCG::GenerateLevel()
{
	// Safety check: Don't do anything if the array is empty
	if (MeshesToSpawn.IsEmpty()) return;

	FVector Origin = SpawnArea->GetComponentLocation();
	FVector Extent = SpawnArea->GetScaledBoxExtent();

	float MinX = Origin.X - Extent.X;
	float MaxX = Origin.X + Extent.X;
	float MinY = Origin.Y - Extent.Y;
	float MaxY = Origin.Y + Extent.Y;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	for (float x = MinX; x <= MaxX; x += Spacing)
	{
		for (float y = MinY; y <= MaxY; y += Spacing)
		{
			FVector Start(x, y, Origin.Z + Extent.Z);
			FVector End(x, y, Origin.Z - Extent.Z);

			FHitResult Hit;
			bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, Params);

			if (bHit)
			{
				float LandscapeHeight = Hit.Location.Z;

				if (LandscapeHeight > HeightThreshold)
				{
					// 1. DENSITY CHECK: Roll a random number between 0.0 and 1.0
					// If the random number is greater than our probability, skip this spot
					if (FMath::FRand() > SpawnProbability)
					{
						continue; 
					}

					// 2. RANDOM ROTATION
					FRotator SpawnRotation = FRotator::ZeroRotator;
					if (bRandomizeRotation)
					{
						// Randomize only the Z axis (Yaw) so trees/rocks stand upright but face different ways
						SpawnRotation.Yaw = FMath::RandRange(0.0f, 360.0f);
					}

					AStaticMeshActor* SpawnedActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Hit.Location, SpawnRotation);
					
					if (SpawnedActor)
					{
						// 3. RANDOM MESH SELECTION
						// Pick a random index from 0 to the end of our array
						int32 RandomIndex = FMath::RandRange(0, MeshesToSpawn.Num() - 1);
						UStaticMesh* SelectedMesh = MeshesToSpawn[RandomIndex];

						// If the selected slot isn't empty, apply it
						if (SelectedMesh)
						{
							SpawnedActor->GetStaticMeshComponent()->SetStaticMesh(SelectedMesh);
						}
						
						SpawnedActor->SetMobility(EComponentMobility::Static);

						// 4. RANDOM SCALE
						// Generate a random float between MinScale and MaxScale and apply it uniformly
						float RandomScale = FMath::RandRange(MinScale, MaxScale);
						SpawnedActor->SetActorScale3D(FVector(RandomScale));
					}
				}
			}
		}
	}
}