// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilityFunction.generated.h"

/**
 * 
 */
UCLASS()
class CRAFTINGSTAR_API UUtilityFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static bool IsHost(AController* Controller);

	UFUNCTION(BlueprintCallable)
		static bool Save(UGameInstance* instance);

};
