// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObject.h"
#include "TwoHandedSensingInterface.h"
#include "TwoHandedSensingObject.generated.h"

/**
 * 
 */
UCLASS()
class CRAFTINGSTAR_API ATwoHandedSensingObject : public AInteractiveObject, public ITwoHandedSensingInterface
{
	GENERATED_BODY()
public:
	virtual void OnHitTwoHanded_Implementation() override;

	
};
