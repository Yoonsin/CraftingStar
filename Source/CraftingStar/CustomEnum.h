// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomEnum.generated.h"

UCLASS()
class CRAFTINGSTAR_API ACustomEnum : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomEnum();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

//�÷��̾� �ɷ�
UENUM(BlueprintType)
enum class EPlayerAbility : uint8
{
	ETelekinesis UMETA(DisplayName = "Telekinesis"),
	EBlast UMETA(DisplayName = "Blast"),
	EAbility_dummy1 UMETA(DisplayName = "Ability_dummy1"),
	EAbility_dummy2 UMETA(DisplayName = "Ability_dummy2"),
	ENone UMETA(DisplayName = "None"),

};

//�÷��̾� ���� 
UENUM(BlueprintType)
enum class EPlayerGMState : uint8
{
	EIdle UMETA(DisplayName = "Idle"),
	EWeaponReady UMETA(DisplayName = "WeaponReady"),
	EWeapon UMETA(DisplayName = "Weapon"),
	EAbilityReady UMETA(DisplayName = "AbilityReady"),
	EAbility UMETA(DisplayName = "Ability"),
	EInteraction UMETA(DisplayName = "Interaction"),
};

//�÷��̾� ����
UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
	EDark UMETA(DisplayName = "Dark"),
	ELight UMETA(DisplayName = "Light"),
};


//�� �̸�
UENUM(BlueprintType)
enum class EMapName : uint8
{
	EWorldMap UMETA(DisplayName = "WorldMap"),
	EKeyStar UMETA(DisplayName = "KeyStar"),
	EFlameAndIceStar UMETA(DisplayName = "FlameAndIceStar"),
};


UENUM(BlueprintType)
enum class EQuestID : uint8
{
	EMegetonNotMeet UMETA(DisplayName = "MegetonNotMeet") ,
	EMegetonMeet UMETA(DisplayName = "MegetonMeet"),
	EMegetonGetKey UMETA(DisplayName = "MegetonGetKey") ,
	EMegetonClear UMETA(DisplayName = "MegetonClear") ,
	EMegetonTransition UMETA(DisplayName = "MegetonTransition"),
	EIncendieStartMovie UMETA(DisplayName = "EIncendieStartMovie"),
	EIncendieEndMovie UMETA(DisplayName = "EIncendieEndMovie") ,
	 
};

