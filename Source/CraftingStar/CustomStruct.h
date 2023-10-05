// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Texture2D.h"
#include "CustomEnum.h"
#include "CustomStruct.generated.h"

UCLASS()
class CRAFTINGSTAR_API ACustomStruct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomStruct();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

//플레이어 능력 정보
USTRUCT(Atomic, BlueprintType)
struct FAbilityStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPlayerAbility Ability;
};

//플레이어 데이터
USTRUCT(Atomic, BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPlayerRole Mode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString PlayerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<bool> AbleAbility;
};

//진행도 데이터
USTRUCT(Atomic, BlueprintType)
struct FProgressData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMapName NowMapName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform HostPlayerPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform GuestPlayerPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ProgressLevel;
};