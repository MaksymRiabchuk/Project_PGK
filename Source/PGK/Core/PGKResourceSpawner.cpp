#include "Core/PGKResourceSpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h" 

APGKResourceSpawner::APGKResourceSpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    SpawnZone = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnZone"));
    RootComponent = SpawnZone;
    
    SpawnZone->SetCollisionProfileName(FName("NoCollision"));
    SpawnZone->SetBoxExtent(FVector(1000.f, 1000.f, 500.f)); 
}

void APGKResourceSpawner::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        SpawnResources();
    }
}

void APGKResourceSpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void APGKResourceSpawner::SpawnResources()
{
    if (!ResourceClassToSpawn || !SpawnZone) return;

    int32 AmountToSpawn = FMath::RandRange(MinResources, MaxResources);

    FVector BoxOrigin = SpawnZone->GetComponentLocation();
    FVector BoxExtent = SpawnZone->GetUnscaledBoxExtent();

    for (int32 i = 0; i < AmountToSpawn; ++i)
    {
        float RandomX = FMath::RandRange(BoxOrigin.X - BoxExtent.X, BoxOrigin.X + BoxExtent.X);
        float RandomY = FMath::RandRange(BoxOrigin.Y - BoxExtent.Y, BoxOrigin.Y + BoxExtent.Y);

        FVector TraceStart(RandomX, RandomY, BoxOrigin.Z + BoxExtent.Z);
        FVector TraceEnd(RandomX, RandomY, BoxOrigin.Z - BoxExtent.Z);

        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this);

        bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

        if (bHit)
        {
            FVector SpawnLocation = HitResult.ImpactPoint;
            SpawnLocation.Z += ZOffset;
            FRotator RandomRotation(0.f, FMath::RandRange(0.f, 360.f), 0.f);

            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
            
            GetWorld()->SpawnActor<AActor>(ResourceClassToSpawn, SpawnLocation, RandomRotation, SpawnParams);
        }
    }
}