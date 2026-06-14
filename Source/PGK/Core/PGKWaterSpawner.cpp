#include "PGKWaterSpawner.h"
#include "PGKWaterAnchor.h"
#include "Components/BoxComponent.h"

APGKWaterSpawner::APGKWaterSpawner()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickInterval = 1.0f;

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

    // Spawn one replicated anchor at the water level.
    // All plastic attaches to it — only this single actor's position
    // replicates when water level changes, not each individual piece of plastic.
    WaterAnchor = GetWorld()->SpawnActor<APGKWaterAnchor>(
        FVector(0.f, 0.f, WaterZLevel), FRotator::ZeroRotator);

    for (int32 i = 0; i < AmountToSpawn; ++i)
    {
        float RandomX = FMath::RandRange(BoxOrigin.X - BoxExtent.X, BoxOrigin.X + BoxExtent.X);
        float RandomY = FMath::RandRange(BoxOrigin.Y - BoxExtent.Y, BoxOrigin.Y + BoxExtent.Y);

        FVector SpawnLocation(RandomX, RandomY, WaterZLevel);
        FRotator RandomRotation(0.f, FMath::RandRange(0.f, 360.f), 0.f);

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnLocation, RandomRotation, SpawnParams);

        if (SpawnedActor && WaterAnchor)
        {
            // KeepWorldTransform: plastic stays at its XY position but is now
            // parented to the anchor — when anchor Z changes, all plastic follows.
            SpawnedActor->AttachToActor(WaterAnchor, FAttachmentTransformRules::KeepWorldTransform);
        }
    }
}

void APGKWaterSpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!HasAuthority() || !WaterAnchor || !TargetWaterPlane) return;

    float NewZ = TargetWaterPlane->GetActorLocation().Z;
    FVector AnchorLoc = WaterAnchor->GetActorLocation();

    if (!FMath::IsNearlyEqual(AnchorLoc.Z, NewZ, 0.1f))
    {
        AnchorLoc.Z = NewZ;
        WaterAnchor->SetActorLocation(AnchorLoc);
    }
}