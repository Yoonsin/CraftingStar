// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "LaserBodyComponent.generated.h"

/**
 * 
 */
UCLASS()
class CRAFTINGSTAR_API ULaserBodyComponent : public UNiagaraComponent
{
	GENERATED_BODY()
	
public:
	// Constructor
	ULaserBodyComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	UPROPERTY(EditAnywhere , Category = "Niagara")
	class UNiagaraSystem* NiagaraSystem;
};
