// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CustomEnum.h"
#include "CustomStruct.h"
#include "CraftingStarSaveGame.generated.h"


/**
 * 
 */
UCLASS()
class CRAFTINGSTAR_API UCraftingStarSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	FProgressData ProgressData;
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	FPlayerData HostData;
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	FPlayerData GuestData;
	
};
