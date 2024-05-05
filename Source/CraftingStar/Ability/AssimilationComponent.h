// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "AssimilationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRAFTINGSTAR_API UAssimilationComponent : public UCapsuleComponent
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
	void DetectSpline(UPrimitiveComponent* OverlappedComp , AActor* OtherActor ,
		UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult);

	UFUNCTION()
	void UndetectSpline(UPrimitiveComponent* OverlappedComp , AActor* OtherActor ,
		UPrimitiveComponent* OtherComp , int32 OtherBodyIndex);

	

	//동화 능력을 일으키기 위한 트리거
	UPROPERTY(VisibleAnywhere, meta = ( AllowPrivateAccess = "true" ))
	TArray<class UBoxComponent*> SplineTrigers;

	

public:	
	// Sets default values for this component's properties
	UAssimilationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//@param bHide: true이면 메쉬를 숨기고, false이면 메쉬를 드러나게 한다.
	void HideOwner(bool bHide);

	//@param bShow: true이면 나이아가라 이펙트를 보이게 하고, false이면 숨긴다.
	void ShowHaloEffect(bool bShow);
	
	//동화 능력
	UFUNCTION(BlueprintCallable)
	void Assimilation();

	UFUNCTION(Server, Reliable)
	void ServerAssimilation();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastAssimilation();


public:

	//동화 능력이 사용될 때 나오는 이펙트
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* HaloEffect;
};
