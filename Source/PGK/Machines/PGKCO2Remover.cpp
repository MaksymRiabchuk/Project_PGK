// Fill out your copyright notice in the Description page of Project Settings.


#include "Machines/PGKCO2Remover.h"
#include "Character/PGKCharacter.h"
#include "Core/PGKGameStateBase.h"

// Sets default values
APGKCO2Remover::APGKCO2Remover()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}

// Called when the game starts or when spawned
void APGKCO2Remover::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			CO2TimerHandle,
			this,
			&APGKCO2Remover::RemoveCO2Tick,
			RemoveInterval,
			true   
		);
	}
}

// Called every frame
void APGKCO2Remover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FText APGKCO2Remover::GetInteractText_Implementation()
{
	return FText::FromString(TEXT("Press E to open"));
}

void APGKCO2Remover::Interact_Implementation(APGKCharacter* InteractorCharacter)
{
	if (InteractorCharacter)
	{
		APGKPlayerController* PC = Cast<APGKPlayerController>(InteractorCharacter->GetController());
		if (PC)
		{
			Client_OpenMachineUI(PC);			
		}
	}
}

void APGKCO2Remover::RemoveCO2Tick()
{
	if (!HasAuthority()) return;
	APGKGameStateBase* GS = GetWorld() ? GetWorld()->GetGameState<APGKGameStateBase>() : nullptr;
	if (!GS) return;
	const double NewCO2 = FMath::Max(0.0, GS->GlobalCO2 - CO2RemoveAmount);
	GS->SetGlobalCO2(NewCO2);
}
