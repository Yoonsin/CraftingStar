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

//플레이어 능력
UENUM(BlueprintType)
enum class EPlayerAbility : uint8
{
	ETelekinesis UMETA(DisplayNmae = "Telekinesis"),
	EBlast UMETA(DisplayNmae = "Blast"),
	EAbility_dummy1 UMETA(DisplayNmae = "Ability_dummy1"),
	EAbility_dummy2 UMETA(DisplayNmae = "Ability_dummy2"),
	ENone UMETA(DisplayNmae = "None"),

};

//플레이어 상태 
UENUM(BlueprintType)
enum class EPlayerGMState : uint8
{
	EIdle UMETA(DisplayNmae = "Idle"),
	EWeaponReady UMETA(DisplayNmae = "WeaponReady"),
	EWeapon UMETA(DisplayNmae = "Weapon"),
	EAbilityReady UMETA(DisplayNmae = "AbilityReady"),
	EAbility UMETA(DisplayNmae = "Ability"),
	EInteraction UMETA(DisplayNmae = "Interaction"),
};

//플레이어 역할
UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
	EDark UMETA(DisplayNmae = "Dark"),
	ELight UMETA(DisplayNmae = "Light"),
};


//맵 이름
UENUM(BlueprintType)
enum class EMapName : uint8
{
	EWorldMap UMETA(DisplayNmae = "WorldMap"),
	EKeyStar UMETA(DisplayNmae = "KeyStar"),
};

