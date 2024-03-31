// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserComponent.h"
#include "NiagaraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"

#include "../CraftingStarPS.h"
#include "../CraftingStarCharacter.h"
#include "../UtilityFunction.h"
#include "LightSensingInterface.h"
#include "../CustomEnum.h"


// Sets default values for this component's properties
ULaserComponent::ULaserComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	LaserBody = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Laser Body"));
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> LaserBodyAsset(TEXT("NiagaraSystem'/Game/Assets/Effects/Laser/NS_Laser.NS_Laser'"));
	if ( LaserBodyAsset.Succeeded() ) {
		LaserBody->SetAsset(LaserBodyAsset.Object);
	}
	LaserBody->SetupAttachment(this);

	LaserImpact = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Laser Impact"));
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> LaserImpactAsset(TEXT("NiagaraSystem'/Game/Assets/Effects/Laser/NS_LaserImpact.NS_LaserImpact'"));
	if ( LaserBodyAsset.Succeeded() ) {
		LaserImpact->SetAsset(LaserImpactAsset.Object);
	}
	LaserImpact->SetupAttachment(this);

	Owner = Cast<ACraftingStarCharacter>(GetOwner());
}


// Called when the game starts
void ULaserComponent::BeginPlay()
{
	Super::BeginPlay();
	if ( !Owner ) return;
	if ( auto State = Cast<ACraftingStarPS>(Owner->GetPlayerState()))
	{
		OwnerRole = State->PlayerData.Mode;
	}
	// ...
	
}


// Called every frame
void ULaserComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULaserComponent::EmitLaser(FVector Location)
{
	ServerEmitLaser(Location);
	return;
}

void ULaserComponent::ServerEmitLaser_Implementation(FVector Location)
{
	MulticastEmitLaser(Location);
}

void ULaserComponent::MulticastEmitLaser_Implementation(FVector Location)
{
	FHitResult Hit;

	// Ability Spawn Loc Socket Transform
	FVector SpawnLocation = this->GetRelativeLocation();
	// Start point and End point of LineTrace
	FVector Start = SpawnLocation;
	
	FVector End = SpawnLocation + ( GetForwardVector() * 10000 );

	// Trace Channel: Custom Trace Channel - AbilitySpawn
	ECollisionChannel Channel = ECollisionChannel::ECC_GameTraceChannel1;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this->GetOwner());

	/* Execute LineTrace */
	GetWorld()->LineTraceSingleByChannel(Hit , Start , End , Channel , QueryParams);
	// Visualize LineTrace
	DrawDebugLine(GetWorld() , Start , End , FColor::Green);

	DrawLaser(Hit , End);

	LightAct(Hit.Actor.Get() , Hit.Location);
}

void ULaserComponent::StopLaser()
{
	ServerStopLaser();
}


void ULaserComponent::ServerStopLaser_Implementation()
{
	MulticastStopLaser();
}

void ULaserComponent::MulticastStopLaser_Implementation()
{

}

void ULaserComponent::DrawLaser(FHitResult Hit , FVector End)
{

	//Draw LaserBody
	if ( Hit.bBlockingHit ) 
		LaserBody->SetVectorParameter(FName(TEXT("LaserEnd")) , End); // Set End point
	else 
		LaserBody->SetWorldLocation(End); // Set End point
	LaserBody->SetVisibility(Hit.bBlockingHit);// Show Laser
	LaserBody->SetNiagaraVariableLinearColor("Color" , EPlayerRole::ELight == OwnerRole ? FLinearColor::White : FLinearColor::Black);

	//Draw LaserImpact
	LaserImpact->SetWorldLocation(End);
	LaserImpact->SetVisibility(Hit.bBlockingHit);
	LaserImpact->SetNiagaraVariableLinearColor("Color" , EPlayerRole::ELight == OwnerRole ? FLinearColor::White : FLinearColor::Black);
}



void ULaserComponent::LightAct(AActor* target , FVector Location)
{
	
	//Here Ejection Abillity Interaction
	auto targetSense = Cast<ILightSensingInterface>(target);
	if ( targetSense && Owner)
	{
		if ( auto State = Cast<ACraftingStarPS>(Owner->GetPlayerState()) ) {
			targetSense->Execute_React(target , ( State->PlayerData.Mode == EPlayerRole::ELight ? true : false ) , Location);
		}
	}
}
