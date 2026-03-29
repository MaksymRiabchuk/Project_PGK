// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PGKBuildingComponent.h"
#include "Core/Inventory/PGKInventoryComponent.h"
#include "Types/PGKBuildingData.h"
#include "PGKHologramPreview.h"
#include "Character/PGKCharacter.h"
#include "Camera/CameraComponent.h"

UPGKBuildingComponent::UPGKBuildingComponent()
{
    // Нам потрібен Tick, щоб голограма літала за камерою
    PrimaryComponentTick.bCanEverTick = true; 
}

void UPGKBuildingComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UPGKBuildingComponent::StartBuilding(UPGKBuildingData* BuildingData)
{
    if (!BuildingData || !BuildingData->HologramClass) return;
    
    StopBuilding();

    CurrentBuildingData = BuildingData;
    
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    CurrentHologram = GetWorld()->SpawnActor<APGKHologramPreview>(CurrentBuildingData->HologramClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
}

void UPGKBuildingComponent::StopBuilding()
{
    if (CurrentHologram)
    {
        CurrentHologram->Destroy();
        CurrentHologram = nullptr;
    }
    CurrentBuildingData = nullptr;
}

void UPGKBuildingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!CurrentHologram || !CurrentBuildingData) return;

    APGKCharacter* Character = Cast<APGKCharacter>(GetOwner());
    if (!Character) return;

    UCameraComponent* Camera = Character->GetFirstPersonCameraComponent();
    FVector StartLoc = Camera->GetComponentLocation();
    FVector EndLoc = StartLoc + (Camera->GetForwardVector() * BuildRange);

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Character);
    QueryParams.AddIgnoredActor(CurrentHologram);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_Visibility, QueryParams);

    if (bHit)
    {
        // 1. СЕТКА (GRID SNAPPING)
        float GridSize = 50.0f; // Розмір клітинки сітки (можна винести в змінну)
        
        FVector SnappedLocation;
        SnappedLocation.X = FMath::GridSnap(HitResult.ImpactPoint.X, GridSize);
        SnappedLocation.Y = FMath::GridSnap(HitResult.ImpactPoint.Y, GridSize);
        
        // Z ми також снапимо, щоб об'єкти на різних поверхах стояли рівно
        SnappedLocation.Z = FMath::GridSnap(HitResult.ImpactPoint.Z, GridSize);

        // 2. ВИПРАВЛЕННЯ ВИСОТИ (Z-OFFSET)
        // Додаємо офсет, щоб підняти півот над землею
        SnappedLocation.Z += CurrentBuildingData->ZOffset;

        // 3. ФІКСОВАНА РОТАЦІЯ (Тільки Yaw)
        // Об'єкт завжди стоїть вертикально, поворот залежить тільки від погляду гравця
        FRotator TargetRotation = FRotator(0.0f, Character->GetActorRotation().Yaw, 0.0f);

        // Застосовуємо трансформацію до голограми
        CurrentHologram->SetActorLocationAndRotation(SnappedLocation, TargetRotation);

        // 4. ПЕРЕВІРКА ВАЛІДНОСТІ
        // Не дозволяємо будувати на стінах (тільки на горизонтальних поверхнях)
        bool bIsSurfaceHorizontal = HitResult.ImpactNormal.Z > 0.7f;
        
        // Оновлюємо стан (червоний/зелений)
        bool bCanBuild = bIsSurfaceHorizontal && (CurrentHologram->IsPlacementValid());
        CurrentHologram->UpdateHologramState(bCanBuild);
    }
    else
    {
        // Якщо дивимося в небо - не показуємо або робимо червоною на відстані
        CurrentHologram->UpdateHologramState(false);
    }
}

void UPGKBuildingComponent::TryConstruct()
{
    if (!CurrentHologram || !CurrentBuildingData) return;

    if (!CurrentHologram->IsPlacementValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot build here! Location is blocked."));
        return;
    }
    Server_ConstructBuilding(CurrentBuildingData, CurrentHologram->GetActorLocation(), CurrentHologram->GetActorRotation());
    StopBuilding();
}

void UPGKBuildingComponent::Server_ConstructBuilding_Implementation(UPGKBuildingData* BuildingData, FVector Location, FRotator Rotation)
{
    if (!BuildingData || !BuildingData->ConstructedClass) return;

    APGKCharacter* Character = Cast<APGKCharacter>(GetOwner());
    if (!Character) return;

    UPGKInventoryComponent* Inventory = Character->GetInventoryComponent();
    if (!Inventory) return;

    if (!Inventory->HasRecipeItems(BuildingData->Recipe))
    {
        UE_LOG(LogTemp, Warning, TEXT("Server: Player %s tried to build without resources!"), *Character->GetName());
        return; 
    }
    Inventory->ConsumeRecipeItems(BuildingData->Recipe);
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    GetWorld()->SpawnActor<AActor>(BuildingData->ConstructedClass, Location, Rotation, SpawnParams);
}