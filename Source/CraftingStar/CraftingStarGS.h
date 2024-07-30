// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CustomEnum.h"
#include "CustomStruct.h"
#include "LevelSequencePlayer.h"
#include "LevelSequence.h"
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
	

	//**obtainingAnimation**// 
	UPROPERTY(Replicated , BlueprintReadWrite , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	EPlayerAbility nowObtainingAbility = EPlayerAbility::ENone;
	UPROPERTY(Replicated , BlueprintReadWrite , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
		bool isHostObtain = false;
	UPROPERTY(Replicated , BlueprintReadWrite , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
		bool isGuestObtain = false;

	UPROPERTY(Replicated , BlueprintReadWrite , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
		bool isStartFlag = false;

	UPROPERTY(BlueprintReadWrite , meta = ( AllowPrivateAccess = "true" ))
		bool isOpenMegetonDoor = false;


	UPROPERTY(Replicated , BlueprintReadWrite , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
		bool isHostInInteractionRange = false;
	UPROPERTY(Replicated , BlueprintReadWrite , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
		bool isGuestInInteractionRange = false;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Sequence , meta = ( AllowPrivateAccess = "true" ))
		ULevelSequence* IncendieStartSequence;

	UFUNCTION(BlueprintCallable)
	void InteractChange(bool isHost,bool isInteraction);

	UFUNCTION(BlueprintImplementableEvent)
	void MultiCastPlaySequence(ULevelSequence* Sequence);

	UFUNCTION(BlueprintImplementableEvent)
    void SinglePlaySequence(ULevelSequence* Sequence);



	bool isHostInit = false;
	bool isGuestInit = false;

	
};
