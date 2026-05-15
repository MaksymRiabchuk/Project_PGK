
#include "Core/PGKBuildingComponent.h"

#include "PGK.h"
#include "Core/Inventory/PGKInventoryComponent.h"
#include "Types/PGKBuildingData.h"
#include "PGKHologramPreview.h"
#include "Character/PGKCharacter.h"
#include "Camera/CameraComponent.h"

UPGKBuildingComponent::UPGKBuildingComponent()
{
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

    if (APGKCharacter* Character = Cast<APGKCharacter>(GetOwner()))
    {
        float InitialYaw = Character->GetActorRotation().Yaw;
        CurrentHologramYaw = FMath::GridSnap(InitialYaw, 45.0f);
    }
    
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
        float GridSize = 50.0f;
        
        FVector SnappedLocation;
        SnappedLocation.X = FMath::GridSnap(HitResult.ImpactPoint.X, GridSize);
        SnappedLocation.Y = FMath::GridSnap(HitResult.ImpactPoint.Y, GridSize);
        SnappedLocation.Z = FMath::GridSnap(HitResult.ImpactPoint.Z, GridSize);
        SnappedLocation.Z += CurrentBuildingData->ZOffset;

        FRotator TargetRotation = FRotator(0.0f, CurrentHologramYaw, 0.0f);
        CurrentHologram->SetActorLocationAndRotation(SnappedLocation, TargetRotation);

        bool bIsSurfaceHorizontal = HitResult.ImpactNormal.Z > 0.7f;
        bool bNoOverlaps = !CurrentHologram->HasAnyOverlaps();
        bool bIsHittingWater = HitResult.GetActor() && HitResult.GetActor()->ActorHasTag(TEXT("Water"));

        if (bHit && HitResult.GetActor())
        {
            FString ActorName = HitResult.GetActor()->GetName();
            bool bHasTag = HitResult.GetActor()->ActorHasTag(TEXT("Water"));
    
            UE_LOG(LogTemp, Warning, TEXT("Промінь влучив у: %s | Має тег Water: %s"), *ActorName, bHasTag ? TEXT("ТАК") : TEXT("НІ"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Промінь взагалі нікуди не влучив!"));
        }

        bool bPlacementRuleMet;

        if (CurrentBuildingData->bRequiresWater)
        {
            bPlacementRuleMet = bIsSurfaceHorizontal && bIsHittingWater;
        }
        else
        {
            bPlacementRuleMet = bIsSurfaceHorizontal && !bIsHittingWater;
        }
        bool bCanBuild = bPlacementRuleMet && bNoOverlaps;
        CurrentHologram->UpdateHologramState(bCanBuild);
    }
    else
    {
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

    if (!Inventory->HasRequiredItems(BuildingData->Recipe))
    {
        UE_LOG(LogTemp, Warning, TEXT("Server: Player %s tried to build without resources!"), *Character->GetName());
        return; 
    }
    Inventory->ConsumeRequiredItems(BuildingData->Recipe);
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    GetWorld()->SpawnActor<AActor>(BuildingData->ConstructedClass, Location, Rotation, SpawnParams);
}

void UPGKBuildingComponent::RotateHologram(float Direction)
{
    if (Direction == 0.0f || !CurrentHologram) return;

    float Sign = FMath::Sign(Direction);
    CurrentHologramYaw += Sign * 45.0f;

    CurrentHologramYaw = FMath::Fmod(CurrentHologramYaw, 360.0f);
    if (CurrentHologramYaw < 0.0f)
    {
        CurrentHologramYaw += 360.0f;
    }
}