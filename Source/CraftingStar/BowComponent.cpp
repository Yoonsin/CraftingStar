// Fill out your copyright notice in the Description page of Project Settings.


#include "BowComponent.h"
#include "CraftingStarCharacter.h"

UBowComponent::UBowComponent()
{

	ConstructorHelpers::FObjectFinder<USkeletalMesh> BowSM(TEXT("SkeletalMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/Weapon/Bows/Bow01_SK.Bow01_SK'"));
	if ( BowSM.Succeeded() ) 
	{
		BowMesh = BowSM.Object;
		this->SetSkeletalMesh(BowMesh);
	}
	
}

void UBowComponent::Shoot()
{
	Equip();
	if ( ShootBowMontage )
	{
		if ( GetAnimInstance() )
			GetAnimInstance()->Montage_Play(ShootBowMontage);
	}

	if ( Archer && ShootArcherMontage )
	{
		Archer->GetMesh()->GetAnimInstance()->Montage_Play(ShootArcherMontage);
	}
}

void UBowComponent::ShootEnd()
{
	Unequip();
}

void UBowComponent::Equip()
{
	SetSkeletalMesh(BowMesh);
}

void UBowComponent::Unequip()
{
	SetSkeletalMesh(NULL);
}

void UBowComponent::SetArcher(ACharacter* Owner)
{
	Archer = Owner;
}
