// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Character/PGKPlayerController.h"
#include "Core/Interfaces/PGKInteractableInterface.h"
#include "GameFramework/Actor.h"
#include "PGKMachineBase.generated.h"

UCLASS(Abstract)
class PGK_API APGKMachineBase : public AActor, public IPGKInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APGKMachineBase();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> MachineWidgetClass;
	
	virtual void Interact_Implementation(APGKCharacter* InteractorCharacter) override;

protected:
	UFUNCTION(Client, Reliable)
	void Client_OpenMachineUI(APGKPlayerController* InteractorController);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
