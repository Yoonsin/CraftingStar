// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"


UWeaponComponent::UWeaponComponent()
{
	SetNotifyRigidBodyCollision(true);
	bCanDamage = false;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this , &UWeaponComponent::OnHit);
}

void UWeaponComponent::OnHit(UPrimitiveComponent* OverlappedComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
	if ( bCanDamage )
	{
		UE_LOG(LogTemp , Display , TEXT("Weapon Hit"));
	}
	
}

void UWeaponComponent::SetWandWeapon(UStaticMesh* WeaponMesh)
{
	WandWeapon = WeaponMesh;

}

void UWeaponComponent::SetTwoHandedWeapon(UStaticMesh* WeaponMesh)
{
	TwoHandedWeapon = WeaponMesh;
	
}

void UWeaponComponent::EquipTwoHanded()
{
	if ( TwoHandedWeapon )
	{
		SetStaticMesh(TwoHandedWeapon);
	}
	
}

void UWeaponComponent::EquipWand()
{
	if ( WandWeapon )
	{
		SetStaticMesh(WandWeapon);
	}
}

void UWeaponComponent::Unequip()
{
	SetStaticMesh(NULL);
}

void UWeaponComponent::AttackEnd()
{
	bCanDamage = false;
	if ( WandWeapon )
	{
		SetStaticMesh(WandWeapon);
	}
}

