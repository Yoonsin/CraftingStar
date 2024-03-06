// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObject.h"
#include "LightSensingInterface.h"
#include "LightSensingObject.generated.h"

/**
 * 
 */
UCLASS()
class CRAFTINGSTAR_API ALightSensingObject : public AInteractiveObject, public ILightSensingInterface
{
	GENERATED_BODY()
	
public:
	virtual void Perform_Implementation() override;
	virtual void React_Implementation(bool bLightCharacter, FVector Location) override;
};
