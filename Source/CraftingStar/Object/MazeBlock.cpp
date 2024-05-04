// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeBlock.h"


// Sets default values
AMazeBlock::AMazeBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMazeBlock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMazeBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMazeBlock::React_Implementation(bool bLightCharacter , FVector Location)
{
	if ( bLightCharacter )
	{
		UE_LOG(LogTemp , Display , TEXT("Light Ray Location X: %f   Y: %f") , Location.X , Location.Y);
	}

	else
	{
		UE_LOG(LogTemp , Display , TEXT("Dark Ray Location X: %f   Y: %f") , Location.X , Location.Y);
	}
}

