// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PGKHologramPreview.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
APGKHologramPreview::APGKHologramPreview()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	HologramMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HologramMesh"));
	HologramMesh->SetupAttachment(RootComponent);
	HologramMesh->SetCollisionProfileName(FName("NoCollision"));
	
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f)); 
	CollisionComponent->SetBoxExtent(FVector(50.0f, 50.0f, 10.0f));
	CollisionComponent->SetCollisionProfileName(FName("OverlapAllDynamic"));
	CollisionComponent->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void APGKHologramPreview::BeginPlay()
{
	Super::BeginPlay();
	if (BaseHologramMaterial && HologramMesh)
	{
		int32 NumMaterials = HologramMesh->GetNumMaterials();
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(BaseHologramMaterial, this);
		if (DynamicMaterialInstance)
		{
			for (int32 i = 0; i < NumMaterials; ++i)
			{
				HologramMesh->SetMaterial(i, DynamicMaterialInstance);
			}
			UpdateHologramState(true);
		}
	}
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APGKHologramPreview::OnOverlapBegin);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &APGKHologramPreview::OnOverlapEnd);	
}

// Called every frame
void APGKHologramPreview::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APGKHologramPreview::UpdateHologramState(bool bCanPlace)
{
    bCurrentPlacementValid = bCanPlace;

    if (DynamicMaterialInstance)
    {
        FLinearColor ColorToSet = bCanPlace ? ValidColor : InvalidColor;
    	//Name of vector in the Material
        DynamicMaterialInstance->SetVectorParameterValue(FName("Color"), ColorToSet);
    }
}

void APGKHologramPreview::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && OtherActor != GetOwner())
    {
        OverlappingBlockingActors++;
        UpdateHologramState(false);
    }
}

void APGKHologramPreview::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor != this && OtherActor != GetOwner())
    {
        OverlappingBlockingActors--;
        
        if (OverlappingBlockingActors <= 0)
        {
            OverlappingBlockingActors = 0;
            UpdateHologramState(true);
        }
    }
}