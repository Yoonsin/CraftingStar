// Fill out your copyright notice in the Description page of Project Settings.


#include "AssimilationObject.h"
#include "AssimilationTrigerComponent.h"
#include "Components/SplineComponent.h"

// Sets default values
AAssimilationObject::AAssimilationObject()
{
	bIsChasing = false;

	TrigerComponent = CreateDefaultSubobject<UAssimilationTrigerComponent>(TEXT("Assimilation Triger"));
	TrigerComponent->SetupAttachment(RootComponent);

	ChasingSplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("Chasing Spline"));
	ChasingSplineComp->SetupAttachment(RootComponent);


 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAssimilationObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAssimilationObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ChaseSpline(DeltaTime);
}


void AAssimilationObject::ChaseSpline_Implementation(float DeltaTime)
{
	if ( NULL )
	{
		UE_LOG(LogTemp , Warning , TEXT("Chasing"));
	}
}
	



