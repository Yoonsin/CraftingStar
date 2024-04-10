// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Ability/LightSensingInterface.h"
#include "MazeBlock.generated.h"

UCLASS()
class CRAFTINGSTAR_API AMazeBlock : public AActor, public ILightSensingInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazeBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void React_Implementation(bool bLightCharacter , FVector Location) override;

};
