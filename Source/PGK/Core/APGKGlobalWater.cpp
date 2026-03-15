#include "Core/APGKGlobalWater.h"

#include "Character/PGKPlayerState.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

APGKGlobalWater::APGKGlobalWater()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicatingMovement(true);

	WaterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaterMesh"));
	RootComponent = WaterMesh;

	UnderwaterVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("UnderwaterVolume"));
	UnderwaterVolume->SetupAttachment(RootComponent);
	UnderwaterVolume->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    
	UnderwaterVolume->SetBoxExtent(FVector(50000.f, 50000.f, 50000.f));
	UnderwaterVolume->SetRelativeLocation(FVector(0.f, 0.f, -50160.f));

	UnderwaterPostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("UnderwaterPostProcess"));
	UnderwaterPostProcess->SetupAttachment(UnderwaterVolume);
	UnderwaterPostProcess->bUnbound = false; 
	UnderwaterPostProcess->BlendWeight = 0.0f;
}
	
void APGKGlobalWater::BeginPlay()
{
	Super::BeginPlay();
	if (UnderwaterVolume)
	{
		UnderwaterVolume->OnComponentBeginOverlap.AddDynamic(this, &APGKGlobalWater::OnOverlapBegin);
		UnderwaterVolume->OnComponentEndOverlap.AddDynamic(this, &APGKGlobalWater::OnOverlapEnd);
	}

	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(WaterTimerHandle, this, &APGKGlobalWater::DecreaseWaterLevel, 0.1f, true);
	}
	
}

void APGKGlobalWater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APGKGlobalWater::DecreaseWaterLevel()
{
	FVector NewLocation = GetActorLocation();
	NewLocation.Z -= DecreaseRate;
	SetActorLocation(NewLocation);
}
void APGKGlobalWater::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacter* OverlappedCharacter = Cast<ACharacter>(OtherActor))
	{
		if (OverlappedCharacter->IsLocallyControlled())
		{
			UnderwaterPostProcess->BlendWeight = 1.0f;
		}

		if (HasAuthority())
		{
			if (APGKPlayerState* PS = OverlappedCharacter->GetPlayerState<APGKPlayerState>())
			{
				PS->SetOxygenDecreasing(true);
			}
		}
	}
}

void APGKGlobalWater::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ACharacter* OverlappedCharacter = Cast<ACharacter>(OtherActor))
	{
		if (OverlappedCharacter->IsLocallyControlled())
		{
			UnderwaterPostProcess->BlendWeight = 0.0f;
		}

		if (HasAuthority())
		{
			if (APGKPlayerState* PS = OverlappedCharacter->GetPlayerState<APGKPlayerState>())
			{
				PS->SetOxygenDecreasing(false);
			}
		}
	}
}