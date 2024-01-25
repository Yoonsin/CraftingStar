// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LightSensingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULightSensingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRAFTINGSTAR_API ILightSensingInterface
{
	GENERATED_BODY()


	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent , BlueprintCallable)
	void React(bool bLightCharacter, FVector Location);
	//빛에 의해 반응했을 때 나오는 함수
	//라이트 캐릭터에 의한 것인지와 Location을 전달.
	virtual void React_Implementation(bool bLightCharacter, FVector Location) =0;
};
