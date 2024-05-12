// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponSensingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponSensingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRAFTINGSTAR_API IWeaponSensingInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent , BlueprintCallable)
	void AttackByTwoHanded(bool bLightCharacter);
	virtual void AttackByTwoHanded_Implementation(bool bLightCharacter) = 0;
};
