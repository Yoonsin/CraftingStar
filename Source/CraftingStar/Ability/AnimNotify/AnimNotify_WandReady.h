// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_WandReady.generated.h"

/**
 * 
 */
UCLASS()
class CRAFTINGSTAR_API UAnimNotify_WandReady : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation) override;
};
