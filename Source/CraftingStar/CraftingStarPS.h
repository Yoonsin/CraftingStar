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

	//Ŭ���̾�Ʈ->���� / ���� NowAbility�� �����ʿ� �˷��ִ� �Լ�
	UFUNCTION(BlueprintCallable, Server, Reliable)
		void RequestPlayerAbility(EPlayerAbility playerAbility);
	UFUNCTION(BlueprintCallable, Server, Reliable)
		void RequestPlayerGMState(EPlayerGMState playerGmState);
	UFUNCTION(BlueprintCallable, Server, Reliable)
		void RequestHealth(float Damage);
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
		void RequestSave();
	UFUNCTION(BlueprintCallable, Server, Reliable)
		void RequestClientUpdate(FPlayerData playerData,FTransform transform);

	//상대방한테 보여줄 능력과 상태
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	  EPlayerAbility NowDisplayAbility = EPlayerAbility::ENone;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	  EPlayerGMState NowDisplayState = EPlayerGMState::EIdle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPlayerAbility NowAbility = EPlayerAbility::ENone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPlayerGMState NowState = EPlayerGMState::EIdle;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		FPlayerData PlayerData;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		float Health = 100.0f;
};
