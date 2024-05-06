// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserNiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h" 
#include "NiagaraComponent.h"
#include "CraftingStarCharacter.h"
#include "CraftingStarPS.h"

ULaserNiagaraComponent::ULaserNiagaraComponent()
{

	// Ability: Laser Niagara System
	LaserBody = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Laser Body"));
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> LaserBodyAsset(TEXT("NiagaraSystem'/Game/Assets/Effects/Laser/NS_Laser.NS_Laser'"));
	if ( LaserBodyAsset.Succeeded() ) {
		LaserBody->SetAsset(LaserBodyAsset.Object);
		LaserBody->SetupAttachment(this);
	}
	LaserBody->SetIsReplicated(true);

	LaserImpact = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Laser Impact"));
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> LaserImpactAsset(TEXT("NiagaraSystem'/Game/Assets/Effects/Laser/NS_LaserImpact.NS_LaserImpact'"));
	if ( LaserBodyAsset.Succeeded() ) {
		LaserImpact->SetAsset(LaserImpactAsset.Object);
		LaserImpact->SetupAttachment(this);
	}
	LaserImpact->SetIsReplicated(true);

	SetOwner();

}

void ULaserNiagaraComponent::SetOwner() {
	owner = Cast< ACraftingStarCharacter>(GetOwner());
}

// Set Laser
void ULaserNiagaraComponent::SetLaser(FHitResult Hit , FVector End) {
	// Set the End of Laser Body
	if ( Hit.bBlockingHit && owner) {
		if ( Cast<ACraftingStarPS>(owner->GetPlayerState())->PlayerData.Mode == EPlayerRole::EDark ) {
			ServerLaser(LaserBody , true , Hit.bBlockingHit , Hit.Location , FLinearColor::Black);
		}
		else if ( Cast<ACraftingStarPS>(owner->GetPlayerState())->PlayerData.Mode == EPlayerRole::ELight ) {
			ServerLaser(LaserBody , true , Hit.bBlockingHit , Hit.Location , FLinearColor::White);
		}
	}
	else {
		if ( Cast<ACraftingStarPS>(owner->GetPlayerState())->PlayerData.Mode == EPlayerRole::EDark ) {
			ServerLaser(LaserBody , true , Hit.bBlockingHit , End , FLinearColor::Black);
		}
		else if ( Cast<ACraftingStarPS>(owner->GetPlayerState())->PlayerData.Mode == EPlayerRole::ELight ) {
			ServerLaser(LaserBody , true , Hit.bBlockingHit , End , FLinearColor::White);
		}
	}
	if ( Cast<ACraftingStarPS>(owner->GetPlayerState())->PlayerData.Mode == EPlayerRole::EDark ) {
		ServerLaser(LaserImpact , false , Hit.bBlockingHit , Hit.Location , FLinearColor::Black);
	}
	else if ( Cast<ACraftingStarPS>(owner->GetPlayerState())->PlayerData.Mode == EPlayerRole::ELight ) {
		ServerLaser(LaserImpact , false , Hit.bBlockingHit , Hit.Location , FLinearColor::White);
	}

}

// Show & Hide
void ULaserNiagaraComponent::Show() {
	LaserBody->SetVisibility(true);
	LaserImpact->SetVisibility(true);
}
void ULaserNiagaraComponent::Hide() {
	LaserBody->SetVisibility(false);
	LaserImpact->SetVisibility(false);
}

// Laser Niagara System Replicate
bool ULaserNiagaraComponent::ServerLaser_Validate(UNiagaraComponent* NiagaraComp , bool isBody , bool isHit , FVector end , FLinearColor color) {
	return true;
}
void ULaserNiagaraComponent::ServerLaser_Implementation(UNiagaraComponent* NiagaraComp , bool isBody , bool isHit , FVector end , FLinearColor color) {
	MulticastLaser(NiagaraComp , isBody , isHit , end , color);
}
void ULaserNiagaraComponent::MulticastLaser_Implementation(UNiagaraComponent* NiagaraComp , bool isBody , bool isHit , FVector end , FLinearColor color) {
	if ( isBody ) {
		// Set End point
		NiagaraComp->SetVectorParameter(FName(TEXT("LaserEnd")) , end);
		// Show Laser
		NiagaraComp->SetVisibility(true);
	}
	else {
		// Set End point
		NiagaraComp->SetWorldLocation(end);
		// Show Laser
		NiagaraComp->SetVisibility(isHit);
	}
	NiagaraComp->SetNiagaraVariableLinearColor("Color" , color);
}
