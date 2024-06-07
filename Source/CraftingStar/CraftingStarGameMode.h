// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CraftingStarCharacter.h"
#include "CustomEnum.h"
#include "GameFramework/PlayerStart.h"
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

	UPROPERTY(VisibleAnywhere , BlueprintReadWrite)
		class APlayerStart* megetonOriginPlayerStart;

	const FOnPlayerDiedSignature& GetOnPlayerDied() const { return OnPlayerDied; }

	void RespawnPlayer(ACharacter* NewPlayer);

	virtual void RestartPlayer(AController* NewPlayer) override;

	bool StartFlag = false;
	bool LoadFlag = false;
	FTimerHandle myTimerHandle;

protected:
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//�÷��̾� ĳ���Ͱ� ������ ȣ��.
	UFUNCTION()
		virtual void PlayerDied(ACharacter* Character);

	//��������Ʈ�� ���ε��� �ñ״�ó. 
	UPROPERTY()
		FOnPlayerDiedSignature OnPlayerDied;

private:
	virtual void PostLogin(APlayerController* newPlayer) override;
	virtual void Logout(AController* Exiting) override;


	TSubclassOf<ACraftingStarCharacter> CharClass;

	FVector SpawnLoc;
	FRotator SpawnRot;


	

};



