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
		DarkLaser = LaserBodyAsset.Object;
	}

	//Light Materials Append
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> LaserBodyAsset_Light(TEXT("NiagaraSystem'/Game/Assets/Effects/Laser/NS_Laser_Light.NS_Laser_Light'"));
	if ( LaserBodyAsset.Succeeded() ) {
		LightLaser = LaserBodyAsset_Light.Object;
	}


	LaserBody->SetIsReplicated(true);

	LaserImpact = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Laser Impact"));
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> LaserImpactAsset(TEXT("NiagaraSystem'/Game/Assets/Effects/Laser/NS_LaserImpact.NS_LaserImpact'"));
	if ( LaserBodyAsset.Succeeded() ) {
		DarkLaserImpact = LaserImpactAsset.Object;
		LaserImpact->SetAsset(LaserImpactAsset.Object);
		LaserImpact->SetupAttachment(this);
	}

	//Light Materials Append
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> LaserImpactAsset_Light(TEXT("NiagaraSystem'/Game/Assets/Effects/Laser/NS_LaserImpact_Light.NS_LaserImpact_Light'"));
	if ( LaserBodyAsset.Succeeded() ) {
		LightLaserImpact = LaserImpactAsset_Light.Object;
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
		if ( owner->nowAbility == EPlayerAbility::EBlast ) {
			if ( Cast<ACraftingStarPS>(owner->GetPlayerState())->PlayerData.Mode == EPlayerRole::EDark ) {
				ServerLaser(LaserBody , true , Hit.bBlockingHit , Hit.Location , FLinearColor::Black);
			}
			else if ( Cast<ACraftingStarPS>(owner->GetPlayerState())->PlayerData.Mode == EPlayerRole::ELight ) {
				ServerLaser(LaserBody , true , Hit.bBlockingHit , Hit.Location , FLinearColor::White);
			}
		}
		else if ( owner->nowAbility == EPlayerAbility::ETelekinesis ) {
			ServerLaser(LaserBody , true , Hit.bBlockingHit , Hit.Location , FLinearColor(0.66, 1, 0, 1));
		}
	}
	else {
		if ( owner->nowAbility == EPlayerAbility::EBlast ) {
			if ( Cast<ACraftingStarPS>(owner->GetPlayerState())->PlayerData.Mode == EPlayerRole::EDark ) {
				ServerLaser(LaserBody , true , Hit.bBlockingHit , End , FLinearColor::Black);
			}
			else if ( Cast<ACraftingStarPS>(owner->GetPlayerState())->PlayerData.Mode == EPlayerRole::ELight ) {
				ServerLaser(LaserBody , true , Hit.bBlockingHit , End , FLinearColor::White);
			}
		} else if ( owner->nowAbility == EPlayerAbility::ETelekinesis ) {
			ServerLaser(LaserBody , true , Hit.bBlockingHit , End , FLinearColor(0.66 , 1 , 0 , 1));
		}
	}


	if ( owner->nowAbility == EPlayerAbility::EBlast ) {
		if ( Cast<ACraftingStarPS>(owner->GetPlayerState())->PlayerData.Mode == EPlayerRole::EDark ) {
			ServerLaser(LaserImpact , false , Hit.bBlockingHit , Hit.Location , FLinearColor::Black);
		}
		else if ( Cast<ACraftingStarPS>(owner->GetPlayerState())->PlayerData.Mode == EPlayerRole::ELight ) {
			ServerLaser(LaserImpact , false , Hit.bBlockingHit , Hit.Location , FLinearColor::White);
		}
	}
	else if ( owner->nowAbility == EPlayerAbility::ETelekinesis ) {
		if ( owner->selectedTarget ) {
			if ( Hit.Component == owner->selectedTarget ) {
				ServerLaser(LaserImpact , false , true , Hit.Location , FLinearColor(0.66 , 1 , 0 , 1));
			} else {
				ServerLaser(LaserImpact , false , false , Hit.Location , FLinearColor(0.66 , 1 , 0 , 1));
			}
		}
		else {
			ServerLaser(LaserImpact , false , Hit.bBlockingHit , Hit.Location , FLinearColor(0.66 , 1 , 0 , 1));
		}
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

void ULaserNiagaraComponent::SystemChange(bool bIsLight)
{
	if ( bIsLight )
	{
		if ( LightLaser && LightLaserImpact )
		{
			LaserBody->SetAsset(LightLaser);
			LaserImpact->SetAsset(LightLaserImpact);
		}
		else UE_LOG(LogTemp , Display , TEXT("Light Laser System Null"));

		
	}
	else
	{
		if ( DarkLaser && DarkLaserImpact )
		{
			LaserBody->SetAsset(DarkLaser);
			LaserImpact->SetAsset(DarkLaserImpact);
		}
		else UE_LOG(LogTemp , Display , TEXT("Dark Laser System Null"));
	}

	
}

bool ULaserNiagaraComponent::ServerHide_Validate() {
	return true;
}
void ULaserNiagaraComponent::ServerHide_Implementation() {

	MulticastHide();
}
void ULaserNiagaraComponent::MulticastHide_Implementation() {
	Hide();
}

// Laser Niagara System Replicate
bool ULaserNiagaraComponent::ServerLaser_Validate(UNiagaraComponent* NiagaraComp , bool isBody , bool isHit , FVector end , FLinearColor color) {
	return true;
}
void ULaserNiagaraComponent::ServerLaser_Implementation(UNiagaraComponent* NiagaraComp , bool isBody , bool isHit , FVector end , FLinearColor color) {
	
	MulticastLaser(NiagaraComp , isBody , isHit , end , color);
}
void ULaserNiagaraComponent::MulticastLaser_Implementation(UNiagaraComponent* NiagaraComp , bool isBody , bool isHit , FVector end , FLinearColor color) {
	
	//클라는 end = 0 나옴 

	if ( isBody ) {
		// Set End point
		NiagaraComp->SetVectorParameter(FName(TEXT("LaserEnd")) , end);
		// Show Laser
		//GEngine->AddOnScreenDebugMessage(-1 , 3 , FColor::Green , FString::Printf(TEXT("Show Laser")));
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
