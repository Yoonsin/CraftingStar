// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "../CustomEnum.h"
#include "LaserComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRAFTINGSTAR_API ULaserComponent : public USceneComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = "Laser" , meta = ( AllowPrivateAccess = "true" ))
	class UNiagaraComponent* LaserBody;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = "Laser" , meta = ( AllowPrivateAccess = "true" ))
	class UNiagaraComponent* LaserImpact;

public:	
	// Sets default values for this component's properties
	ULaserComponent();


public:
	//Activate Laser
	UFUNCTION()
	void EmitLaser(FVector Location);
	UFUNCTION()
	//Deactivate Laser
	void StopLaser();
	
	UFUNCTION()
	void DrawLaser(FHitResult Hit , FVector End);

	UFUNCTION()
	void LightAct(AActor* target , FVector Location);

protected:
	UFUNCTION(Server , Reliable)
	void ServerEmitLaser(FVector Location);
	UFUNCTION(NetMulticast , Reliable)
	void MulticastEmitLaser(FVector Location);

	UFUNCTION(Server , Reliable)
	void ServerStopLaser();
	UFUNCTION(NetMulticast , Reliable)
	void MulticastStopLaser();


public:
	class ACraftingStarCharacter* Owner;
	EPlayerRole OwnerRole;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
