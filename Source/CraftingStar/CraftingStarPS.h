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
	ACraftingStarPS();

	//클라이언트->서버 / 현재 NowAbility를 서버쪽에 알려주는 함수
	UFUNCTION(BlueprintCallable, Server, Reliable)
		void RequestPlayerAbility(EPlayerAbility playerAbility);
	UFUNCTION(BlueprintCallable, Server, Reliable)
		void RequestPlayerGMState(EPlayerGMState playerGmState);
	UFUNCTION(BlueprintCallable, Server, Reliable)
		void RequestHealth(float Damage);

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	  EPlayerAbility NowAbility = EPlayerAbility::ENone;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	  EPlayerGMState NowState = EPlayerGMState::EIdle;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		FPlayerData PlayerData;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		float Health = 100.0f;
};
