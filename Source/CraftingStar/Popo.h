// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Popo.generated.h"

UCLASS()
class CRAFTINGSTAR_API APopo : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APopo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UPROPERTY(Replicated , EditAnywhere , BlueprintReadWrite , Category = Attack , meta = ( AllowPrivateAccess = "true" ))
	int CatchedPlayerCnt;

	UFUNCTION(BlueprintCallable)
	int CheckCatchedPlayerCnt();
};
