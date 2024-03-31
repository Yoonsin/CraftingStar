// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveObject.h"
#include "Components/BoxComponent.h"

// Sets default values
AInteractiveObject::AInteractiveObject()
{

	box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = box;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(RootComponent);
	

	
	

	
}

// Called when the game starts or when spawned
void AInteractiveObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractiveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AInteractiveObject::Perform_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("Object Performing"));
}
