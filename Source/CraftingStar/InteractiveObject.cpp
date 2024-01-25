// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveObject.h"
#include "Components/BoxComponent.h"

// Sets default values
AInteractiveObject::AInteractiveObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = box;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(RootComponent);
}

// Called every frame
void AInteractiveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when the game starts or when spawned
void AInteractiveObject::BeginPlay()
{
	Super::BeginPlay();

}

int AInteractiveObject::InteractiveFunc() {
	GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("큐브 아야!"));

	return 10;
}

void AInteractiveObject::Perform_Implementation()
{
	UE_LOG(LogTemp , Display , TEXT("Object Performing"));
}