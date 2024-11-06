// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Net/UnrealNetwork.h"
#include "LaserBeamComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRAFTINGSTAR_API ULaserBeamComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULaserBeamComponent();

	void SetLaser();

	void VisualizeLaser();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	class ACraftingStarCharacter* Owner;

	// Start of Laser Beam
	UPROPERTY(Replicated)
	FVector LaserStart;
	// End of Laser Beam
	UPROPERTY(Replicated)
	FVector LaserEnd;

	// Niagara Component
	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* LaserBeamComponent;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetLaser();

};
