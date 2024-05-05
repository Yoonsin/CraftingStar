// Fill out your copyright notice in the Description page of Project Settings.


#include "TelekinesisInteractableObject.h"

// Sets default values
ATelekinesisInteractableObject::ATelekinesisInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);

	// Actor Mesh
	ActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ActorMesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> ActorSM(TEXT("StaticMesh'/Game/Assets/BaseContent/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if ( ActorSM.Succeeded() ) {
		ActorMesh->SetStaticMesh(ActorSM.Object);
	}
}

// Called when the game starts or when spawned
void ATelekinesisInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATelekinesisInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ATelekinesisInteractableObject::Init() {
	
}

void ATelekinesisInteractableObject::DrawOuline() {

}

void ATelekinesisInteractableObject::EraseOuline() {

}

void ATelekinesisInteractableObject::ChangeOutlineColor() {

}
