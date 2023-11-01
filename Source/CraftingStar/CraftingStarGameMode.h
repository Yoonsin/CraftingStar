// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CraftingStarCharacter.h"
#include "CustomEnum.h"
#include "CraftingStarGameMode.generated.h"

UCLASS()
class ACraftingStarGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ACraftingStarGameMode();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int ProgressLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EMapName NowMapName;
	UFUNCTION(BlueprintCallable)
		void InitGame();

private:
	virtual void PostLogin(APlayerController* newPlayer) override;
	TSubclassOf<ACraftingStarCharacter> CharClass;

	FVector SpawnLoc;
	FRotator SpawnRot;


	

};



