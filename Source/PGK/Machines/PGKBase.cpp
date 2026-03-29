// Fill out your copyright notice in the Description page of Project Settings.
#include "Machines/PGKBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Character/PGKCharacter.h"
#include "Character/PGKPlayerState.h"

// Sets default values
APGKBase::APGKBase()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f)); 
	CollisionComponent->SetSphereRadius(250);
	CollisionComponent->SetCollisionProfileName(FName("OverlapAllDynamic"));
	CollisionComponent->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void APGKBase::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APGKBase::OnOverlapBegin);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &APGKBase::OnOverlapEnd);
}

// Called every frame
void APGKBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APGKBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("Entered Base!"));
	if (!HasAuthority()) return;
	
	if (APGKCharacter* Character = Cast<APGKCharacter>(OtherActor))
	{
		if (APGKPlayerState* PS = Character->GetPlayerState<APGKPlayerState>())
		{
			PS->EnterBase();
		}
	}
}

void APGKBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!HasAuthority()) return;

	if (APGKCharacter* Character = Cast<APGKCharacter>(OtherActor))
	{
		if (APGKPlayerState* PS = Character->GetPlayerState<APGKPlayerState>())
		{
			PS->LeaveBase();
		}
	}
}