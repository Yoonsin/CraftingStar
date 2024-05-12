// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SplineChasingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USplineChasingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 스플라인을 따라가는 기능이 포함된 인터페이스
 *
 * 
 */
class CRAFTINGSTAR_API ISplineChasingInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//스플라인을 따라 움직이기 시작할 때 호출하는 함수
	UFUNCTION(BlueprintNativeEvent , BlueprintCallable)
	void ChaseStart(class UAssimilationComponent* AssmitationComp, bool bLightCharacter);
	virtual void ChaseStart_Implementation(class UAssimilationComponent* AssmitationComp, bool bLightCharacter) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ChaseEnd(class UAssimilationComponent* AssmitationComp);
	virtual void ChaseEnd_Implementation(class UAssimilationComponent* AssmitationComp ) = 0;
};
