// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Ability/SplineChasingInterface.h"
#include "AssimilationObject.generated.h"

UCLASS()
class CRAFTINGSTAR_API AAssimilationObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAssimilationObject();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(BlueprintReadWrite , EditAnywhere , meta = ( AllowPrivateAccess = "true" ))
	bool bIsChasing;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UAssimilationTrigerComponent* TrigerComponent;

	////따라갈 스플라인 컴포넌트
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//class USplineComponent* ChasingSplineComp;

public:
	UFUNCTION(BlueprintCallable)
	void SetChasing(bool bChase) { bIsChasing = bChase; }

	//Object의 Chasing을 구현하는 부분, 조건에 따라 매 틱때 구현됨.
	UFUNCTION(BlueprintNativeEvent , BlueprintCallable)
	void ChaseSpline(float DeltaTime);
	virtual void ChaseSpline_Implementation(float DeltaTime);
};
