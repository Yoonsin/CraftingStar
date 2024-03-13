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
	UPROPERTY(EditAnywhere)
	UStaticMesh* WandWeapon;
	UPROPERTY(EditAnywhere)
	UStaticMesh* TwoHandedWeapon;
	
public:

	UFUNCTION()
	//무기에 맞을 때
	void OnHit(UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult);
	
	//무기에 피격판정 있는 지 체크
	bool bCanDamage;

	//완드 설정
	void SetWandWeapon(UStaticMesh* WeaponMesh);

	//양손검 설정
	void SetTwoHandedWeapon(UStaticMesh* WeaponMesh);

	//양손검으로 무기 변환
	void EquipTwoHanded();

	//완드로 무기 변환
	void EquipWand();

	//무기 장착 해제;
	void Unequip();

	//공격이 끝날 경우 호출
	void AttackEnd(); 

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
