// Fill out your copyright notice in the Description page of Project Settings.


#include "BowComponent.h"
#include "../CraftingStarCharacter.h"
#include "ArrowProjectile.h"

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
		{
			GetAnimInstance()->Montage_Play(ShootBowMontage);
		}
		else
		{
			UE_LOG(LogTemp , Warning , TEXT("Bow does not have Animation Blueprint. Need to Set Bluepirnt"));
		}
			
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
	if ( WandWeaponComponent )
		WandWeaponComponent->SetHiddenInGame(true);
}

void UBowComponent::Unequip()
{
	SetSkeletalMesh(NULL);
	if ( WandWeaponComponent )
		WandWeaponComponent->SetHiddenInGame(false);
}

void UBowComponent::SetArcher(ACharacter* Owner)
{
	Archer = Cast<ACraftingStarCharacter>(Owner);
}

void UBowComponent::ArrowSpawn()
{
	if ( Archer && ArrowClass )
	{
		FRotator SpawnRotation = Archer->GetActorRotation();
		FVector SpawnLocation = Archer->GetActorLocation() + SpawnRotation.RotateVector(Archer->ArrowOffset);
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AArrowProjectile>(ArrowClass , SpawnLocation , SpawnRotation);
	}
	
}