#include "PGKWaterSpawner.h"
#include "Components/BoxComponent.h"

APGKWaterSpawner::APGKWaterSpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
    RootComponent = SpawnArea;
    SpawnArea->SetCollisionProfileName(FName("NoCollision"));
    SpawnArea->SetBoxExtent(FVector(2000.f, 2000.f, 100.f)); 
}

void APGKWaterSpawner::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        SpawnOnWater();
    }
}

void APGKWaterSpawner::SpawnOnWater()
{
    if (!TargetWaterPlane || !ClassToSpawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("WaterSpawner: Water Plane or ClassToSpawn is not set!"));
        return;
    }

    FVector BoxOrigin = SpawnArea->GetComponentLocation();
    FVector BoxExtent = SpawnArea->GetUnscaledBoxExtent();

    float WaterZLevel = TargetWaterPlane->GetActorLocation().Z;

    for (int32 i = 0; i < AmountToSpawn; ++i)
    {
        float RandomX = FMath::RandRange(BoxOrigin.X - BoxExtent.X, BoxOrigin.X + BoxExtent.X);
        float RandomY = FMath::RandRange(BoxOrigin.Y - BoxExtent.Y, BoxOrigin.Y + BoxExtent.Y);

        FVector SpawnLocation(RandomX, RandomY, WaterZLevel);
        FRotator RandomRotation(0.f, FMath::RandRange(0.f, 360.f), 0.f);

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnLocation, RandomRotation, SpawnParams);

        if (SpawnedActor)
        {
            SpawnedActor->AttachToActor(TargetWaterPlane, FAttachmentTransformRules::KeepWorldTransform);
        }
    }
}

void APGKWaterSpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}