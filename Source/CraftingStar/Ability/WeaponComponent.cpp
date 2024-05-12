// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "WeaponSensingInterface.h"
#include "../CraftingStarPS.h"
#include "../CraftingStarCharacter.h"

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
		if ( OtherActor->Implements<UWeaponSensingInterface>() )
		{
			if ( auto Owner = Cast<ACraftingStarCharacter>(GetOwner()) )
			{
				if ( auto State = Cast<ACraftingStarPS>(Owner->GetPlayerState()) )
				{
					IWeaponSensingInterface::Execute_AttackByTwoHanded(OtherActor , EPlayerRole::ELight == State->PlayerData.Mode);
					UE_LOG(LogTemp , Display , TEXT("Weapon Hit"));
				}
			}
		}	
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

void UWeaponComponent::WeaponChange()
{
	if ( TwoHandedWeapon )
	{
		SetStaticMesh(TwoHandedWeapon);
	}
	
}

void UWeaponComponent::AttackEnd()
{
	bCanDamage = false;
	if ( WandWeapon )
	{
		SetStaticMesh(WandWeapon);
	}
}