// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArrowProjectile.generated.h"

UCLASS()
class CRAFTINGSTAR_API AArrowProjectile : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Mesh , meta = ( AllowPrivateAccess = "true" ))
	class USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Mesh , meta = ( AllowPrivateAccess = "true" ))
	class UBoxComponent* CollisionComp;
	class UProjectileMovementComponent* ProjectileMovement;
public:	
	// Sets default values for this actor's properties
	AArrowProjectile();
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , FVector NormalImpulse , const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
