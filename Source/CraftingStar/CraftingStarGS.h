// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CustomEnum.h"
#include "CustomStruct.h"
#include "CraftingStarGS.generated.h"

/**
 * 
 */
UCLASS()
class CRAFTINGSTAR_API ACraftingStarGS : public AGameState
{
	GENERATED_BODY()
public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FProgressData ProgressData;
	
	
};
