// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveColorCube.h"

// Sets default values
AInteractiveColorCube::AInteractiveColorCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Cube Mesh
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeSM(TEXT("StaticMesh'/Game/Assets/BaseContent/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if ( CubeSM.Succeeded() ) {
		CubeMesh->SetStaticMesh(CubeSM.Object);
	}
	CubeMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AInteractiveColorCube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractiveColorCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int AInteractiveColorCube::InteractiveFunc() {
	GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("큐브 아야!"));

	return 10;
}
