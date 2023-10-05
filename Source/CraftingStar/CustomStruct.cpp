// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomStruct.h"

// Sets default values
ACustomStruct::ACustomStruct()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACustomStruct::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomStruct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

