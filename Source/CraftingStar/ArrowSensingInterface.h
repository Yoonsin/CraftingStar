// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ArrowSensingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UArrowSensingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRAFTINGSTAR_API IArrowSensingInterface
{
	GENERATED_BODY()

	
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent , BlueprintCallable)
	void React();
	virtual void React_Implementation() = 0;

};
