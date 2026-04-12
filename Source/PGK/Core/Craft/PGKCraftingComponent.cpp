// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved


#include "Core/Craft/PGKCraftingComponent.h"

// Sets default values for this component's properties
UPGKCraftingComponent::UPGKCraftingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPGKCraftingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPGKCraftingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<UPGKCraftingRecipeData*> UPGKCraftingComponent::GetAvailableRecipes() const
{
	return AvailableRecipes;
}

