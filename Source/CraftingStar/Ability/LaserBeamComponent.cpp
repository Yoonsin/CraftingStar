// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserBeamComponent.h"
#include "../CraftingStarCharacter.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
ULaserBeamComponent::ULaserBeamComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);

	LaserBeamComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserBeamComponent"));
	LaserBeamComponent->SetupAttachment(this);
}


// Called when the game starts
void ULaserBeamComponent::BeginPlay()
{
	Super::BeginPlay();	

	Owner = Cast<ACraftingStarCharacter>(GetOwner());
}

void ULaserBeamComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ULaserBeamComponent , LaserStart);
	DOREPLIFETIME(ULaserBeamComponent , LaserEnd);
}

void ULaserBeamComponent::SetLaser() {
	if ( Owner ) {
		if ( Owner->GetOwner() ) {
			ServerSetLaser();
			VisualizeLaser();
		}
	}
}

bool ULaserBeamComponent::ServerSetLaser_Validate() {
	return true;
}
void ULaserBeamComponent::ServerSetLaser_Implementation() {
	if ( !Owner ) return;

	FHitResult Hit;

	FVector Start = Owner->SpawnLocSource->GetComponentLocation();
	FVector ForwardVector = Owner->SpawnLocSource->GetUpVector();
	FVector End = Start + ( ForwardVector * 10000 );
	
	// Trace Channel: Custom Trace Channel - AbilitySpawn
	ECollisionChannel Channel = ECollisionChannel::ECC_GameTraceChannel1;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);

	LaserStart = Start;
	/* Execute LineTrace */
	if ( GetWorld()->LineTraceSingleByChannel(Hit , Start , End , Channel , QueryParams) ) {
		LaserEnd = Hit.Location;
	}
	else {
		LaserEnd = End;
	}
}

void ULaserBeamComponent::VisualizeLaser() {
	DrawDebugLine(GetWorld() , LaserStart , LaserEnd , FColor::Green);

	if ( LaserBeamComponent ) {
		LaserBeamComponent->SetWorldLocation(LaserStart);
		LaserBeamComponent->SetVectorParameter(TEXT("BeamEnd") , LaserEnd);
	}
}