// Copyright Epic Games, Inc. All Rights Reserved.

#include "PGKCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PGK.h"
#include "PGKPlayerController.h"
#include "Core/PGKBuildingComponent.h"
#include "Core/Interfaces/PGKInteractableInterface.h"
#include "Core/Inventory/PGKInventoryComponent.h"

APGKCharacter::APGKCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
	
	InventoryComponent = CreateDefaultSubobject<UPGKInventoryComponent>(TEXT("PGK_Inventory"));
	InventoryComponent->SetIsReplicated(true);
	BuildingComponent = CreateDefaultSubobject<UPGKBuildingComponent>(TEXT("BuilderComponent"));
}

void APGKCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APGKCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APGKCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APGKCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APGKCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &APGKCharacter::LookInput);
	}
	else
	{
		UE_LOG(LogPGK, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}


void APGKCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void APGKCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void APGKCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void APGKCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void APGKCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void APGKCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

void APGKCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocallyControlled())
	{
		GetWorld()->GetTimerManager().SetTimer(InteractCheckTimer, this, &APGKCharacter::CheckForInteractables, 0.1f, true);
	}
}

void APGKCharacter::CheckForInteractables()
{
	APGKPlayerController* PC = Cast<APGKPlayerController>(GetController());
	if (!PC || !PC->PlayerCameraManager) return;
	
	FVector StartLocation = PC->PlayerCameraManager->GetCameraLocation();
	FVector ForwardVector = PC->PlayerCameraManager->GetCameraRotation().Vector();
	FVector EndLocation = StartLocation + (ForwardVector * 550.0f);
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, QueryParams);

	if (bHit && HitResult.GetActor())
	{
		AActor *HitActor = HitResult.GetActor();
		if (HitActor->Implements<UPGKInteractableInterface>())
		{
			if (HitActor != CurrentInteractable)
			{
				CurrentInteractable = HitActor;
				InteractHitLocation = HitResult.ImpactPoint;
				FText InteractionText = IPGKInteractableInterface::Execute_GetInteractText(CurrentInteractable);
				PC->ShowInteractionWidget(InteractionText);
				OnInteractCheckCompleted();
			}
			return;
		}
	}
	
	if (CurrentInteractable != nullptr)
	{
		CurrentInteractable = nullptr;
		PC->HideInteractionWidget();
		OnInteractCheckCompleted();
	}
}

void APGKCharacter::Server_Interact_Implementation(AActor* InteractableActor)
{
	if (!InteractableActor) return;

	if (InteractableActor->Implements<UPGKInteractableInterface>())
	{
		IPGKInteractableInterface::Execute_Interact(InteractableActor, this);
	}
}

void APGKCharacter::TryInteract()
{
	if (CurrentInteractable)
	{
		Server_Interact(CurrentInteractable);
	}
}

void APGKCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();
    
	GetWorld()->GetTimerManager().SetTimer(InteractCheckTimer, this, &APGKCharacter::CheckForInteractables, 0.1f, true);
}

void APGKCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(InteractCheckTimer);
	Super::EndPlay(EndPlayReason);
}