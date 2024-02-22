// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "WeaponComponent.generated.h"

/**
 * 
 */
UCLASS(meta = ( BlueprintSpawnableComponent ))
class CRAFTINGSTAR_API UWeaponComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

	UWeaponComponent();
private:
	UStaticMesh* WandWeapon;
	UStaticMesh* TwoHandedWeapon;
	
public:

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult);
	bool bCanDamage;
	void SetWandWeapon(UStaticMesh* WeaponMesh);
	void SetTwoHandedWeapon(UStaticMesh* WeaponMesh);
	void WeaponChange();
	void AttackEnd();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
