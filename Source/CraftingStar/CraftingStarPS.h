// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CustomEnum.h"
#include "CustomStruct.h"
#include "CraftingStarPS.generated.h"

/**
 * 
 */
UCLASS()
class CRAFTINGSTAR_API ACraftingStarPS : public APlayerState
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	  EPlayerAbility NowAbility = EPlayerAbility::ENone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	  EPlayerGMState NowState = EPlayerGMState::EIdle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FPlayerData PlayerData;
};
