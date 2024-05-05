// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "LaserNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS( meta = ( BlueprintSpawnableComponent ) )
class CRAFTINGSTAR_API ULaserNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

	ULaserNiagaraComponent();

private :
	class ACraftingStarCharacter* owner;
	void SetOwner();

protected :

	// Laser: Niagara Component
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = "Laser" , meta = ( AllowPrivateAccess = "true" ))
	class UNiagaraComponent* LaserBody;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = "Laser" , meta = ( AllowPrivateAccess = "true" ))
	class UNiagaraComponent* LaserImpact;

public :
	// Set
	void SetLaser(FHitResult Hit , FVector End);

	// Show & Hide
	void Show();
	void Hide();

	// Server: Replicate
	UFUNCTION(Server , Reliable , WithValidation , Category = "CraftingStar Character")
	void ServerLaser(UNiagaraComponent* NiagaraComp , bool isBody , bool isHit , FVector end , FLinearColor color);
	UFUNCTION(NetMulticast , Unreliable , Category = "CraftingStar Character")
	void MulticastLaser(UNiagaraComponent* NiagaraComp , bool isBody , bool isHit , FVector end , FLinearColor color);

};
