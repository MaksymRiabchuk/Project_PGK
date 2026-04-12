// Copyright 2026 Maksym Riabchuk, Andrii Diachuk. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PGKCraftingComponent.generated.h"

class UPGKCraftingRecipeData;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PGK_API UPGKCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPGKCraftingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting")
	TArray<UPGKCraftingRecipeData*> AvailableRecipes;

public:
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	TArray<UPGKCraftingRecipeData*> GetAvailableRecipes() const;
		
};
