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
	
private:
	class UAssimilationComponent* AssimilationComponent;

	
public:
	//해당 트리거가 빛의 별이 반응하게 되어있는가?.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bForLightCharacter;

	UFUNCTION(BlueprintCallable)
	void SetAssimilationComponent(class UAssimilationComponent* AssimilationComp);
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UAssimilationComponent* GetAssimilationComponent() { return this->AssimilationComponent; }

	virtual void ChaseStart_Implementation(class UAssimilationComponent* AssimilationComp, bool bLightCharacter) override;
	virtual void ChaseEnd_Implementation(class UAssimilationComponent* AssimilationComp) override;

};
