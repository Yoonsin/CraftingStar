// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CraftingStarCharacter.h"
#include "CraftingStarGameMode.generated.h"

UCLASS(minimalapi)
class ACraftingStarGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ACraftingStarGameMode();
private:
	virtual void PostLogin(APlayerController* newPlayer) override;
	TSubclassOf<ACraftingStarCharacter> CharClass;

	FVector SpawnLoc;
	FRotator SpawnRot;

};



