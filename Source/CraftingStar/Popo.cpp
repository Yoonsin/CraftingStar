// Fill out your copyright notice in the Description page of Project Settings.


#include "Popo.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APopo::APopo()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	CatchedPlayerCnt = 0;
}

void APopo::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APopo , CatchedPlayerCnt);
}

// Called when the game starts or when spawned
void APopo::BeginPlay()
{
	Super::BeginPlay();

	CatchedPlayerCnt = 0;
}

// Called every frame
void APopo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APopo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

int APopo::CheckCatchedPlayerCnt() {
	return CatchedPlayerCnt;
}

