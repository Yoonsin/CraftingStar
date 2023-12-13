// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CraftingStarCharacter.h"
#include "CustomEnum.h"
#include "CraftingStarGameMode.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDiedSignature, ACharacter*, Character);
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

	const FOnPlayerDiedSignature& GetOnPlayerDied() const { return OnPlayerDied; }

	//플레이어의 폰을 스폰 시도.
	void RespawnPlayer(ACharacter* NewPlayer);

	virtual void RestartPlayer(AController* NewPlayer) override;


protected:
	virtual void BeginPlay() override;

	//플레이어 캐릭터가 죽으면 호출.
	UFUNCTION()
		virtual void PlayerDied(ACharacter* Character);

	//델리게이트를 바인딩할 시그니처. 
	UPROPERTY()
		FOnPlayerDiedSignature OnPlayerDied;

private:
	virtual void PostLogin(APlayerController* newPlayer) override;
	virtual void Logout(AController* Exiting) override;

	TSubclassOf<ACraftingStarCharacter> CharClass;

	FVector SpawnLoc;
	FRotator SpawnRot;


	

};



