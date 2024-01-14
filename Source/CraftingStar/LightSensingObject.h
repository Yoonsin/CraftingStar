// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObject.h"
#include "LightSensingObject.generated.h"

/**
 * 
 */
UCLASS()
class CRAFTINGSTAR_API ALightSensingObject : public AInteractiveObject
{
	GENERATED_BODY()
	
public:
	virtual void Perform_Implementation() override;

	/// <summary>
	/// 빛에 의해 반응했을 때 나오는 함수
	/// </summary>
	/// <param name="Location">빛을 받은 위치</param>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void React(FVector Location);
	/// <summary>
	/// 빛에 의해 반응했을 때 나오는 함수
	/// </summary>
	/// <param name="Location">빛을 받은 위치</param>
	virtual void React_Implementation(FVector Location);
};
