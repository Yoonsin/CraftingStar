// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "../Ability/SplineChasingInterface.h"
#include "AssimilationTrigerComponent.generated.h"

/**
 * 
 */
UCLASS(meta = ( BlueprintSpawnableComponent ))
class CRAFTINGSTAR_API UAssimilationTrigerComponent : public UBoxComponent, public ISplineChasingInterface
{
	GENERATED_BODY()
	

public:
	virtual void ChaseStart_Implementation(class UAssimilationComponent* AssmitationComp) override;
	virtual void ChaseEnd_Implementation(class UAssimilationComponent* AssmitationComp) override;

};
