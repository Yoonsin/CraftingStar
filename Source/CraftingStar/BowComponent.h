// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "BowComponent.generated.h"

/**
 * 
 */
UCLASS(meta = ( BlueprintSpawnableComponent ))
class CRAFTINGSTAR_API UBowComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
	UBowComponent();

private:
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Mesh , meta = ( AllowPrivateAccess = "true" ))
	USkeletalMesh* BowMesh;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = AnimMontage, meta = ( AllowPrivateAccess = "true" ))
	class UAnimMontage* ShootBowMontage;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = AnimMontage , meta = ( AllowPrivateAccess = "true" ))
	class UAnimMontage* ShootArcherMontage;

	class ACharacter* Archer;

public:
	void Shoot();
	void Equip();
	void Unequip();

public:
	void SetArcher(class ACharacter* Owner);
};
