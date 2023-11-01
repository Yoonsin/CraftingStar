// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingStarPalette.generated.h"

/**
 * 
 */
UCLASS()
class CRAFTINGSTAR_API UCraftingStarPalette : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateAbility();

	//UCraftingStarPalette(const FObjectInitializer& ObjectInitializer);
};
