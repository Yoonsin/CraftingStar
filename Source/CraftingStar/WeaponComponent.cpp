// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "TwoHandedSensingInterface.h"

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
		auto targetSense = Cast<ITwoHandedSensingInterface>(OtherActor);
		if ( targetSense )
		{
			//빛 대상
			//IArrowSensingInterface를 상속받은 얘의 React 함수를 호출받는 방법.
			targetSense->Execute_OnHitTwoHanded(OtherActor);
			UE_LOG(LogTemp , Display , TEXT("Hmm"));
		}
		UE_LOG(LogTemp , Display , TEXT("%s") , *OtherActor->GetClass()->GetName());
		
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

