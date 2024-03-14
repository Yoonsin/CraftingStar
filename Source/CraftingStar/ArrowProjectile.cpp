// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "ArrowSensingInterface.h"

// Sets default values
AArrowProjectile::AArrowProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	CollisionComp->SetBoxExtent(FVector(15.f , 15.f, 15.f));
	RootComponent = CollisionComp;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetRelativeRotation(FRotator(0.f , -90.f , 0.f));
	Mesh->SetRelativeLocation(FVector(-90.f , 0.f , 0.f));
	Mesh->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> ArrowSK(TEXT("SkeletalMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/Weapon/Arrows/Arrow01_SK.Arrow01_SK'"));
	if ( ArrowSK.Succeeded() )
	{
		Mesh->SetSkeletalMesh(ArrowSK.Object);
	}

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectiles"));
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->InitialSpeed = 800.0f;
	ProjectileMovement->MaxSpeed = 800.0f;

	InitialLifeSpan = 5.f;
	
	

}

// Called when the game starts or when spawned
void AArrowProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArrowProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArrowProjectile::OnHit(UPrimitiveComponent* HitComp , AActor* OtherActor , UPrimitiveComponent* OtherComp , FVector NormalImpulse , const FHitResult& Hit)
{
	auto targetSense = Cast<IArrowSensingInterface>(Hit.Actor);
	if ( targetSense )
	{
		//빛 대상
		//IArrowSensingInterface를 상속받은 얘의 React 함수를 호출받는 방법.
		targetSense->Execute_OnHitArrow(Hit.Actor.Get());
	}
}

