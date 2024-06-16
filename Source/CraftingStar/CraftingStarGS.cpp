// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingStarGS.h"
#include "CraftingStarCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


void ACraftingStarGS::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACraftingStarGS , ProgressData);
	DOREPLIFETIME(ACraftingStarGS , nowObtainingAbility);
	DOREPLIFETIME(ACraftingStarGS , isHostObtain);
	DOREPLIFETIME(ACraftingStarGS , isGuestObtain);
	DOREPLIFETIME(ACraftingStarGS , isStartFlag);
	DOREPLIFETIME(ACraftingStarGS , isHostInInteractionRange);
	DOREPLIFETIME(ACraftingStarGS , isGuestInInteractionRange);

}


void ACraftingStarGS::InteractChange(bool isHost , bool isInteraction) {

	//GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green ,FString::Printf(TEXT("isHost? %d isInteraction? %d"),isHost,isInteraction));
	ACraftingStarCharacter* hostCharacter = Cast< ACraftingStarCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld() , 0));
	if ( hostCharacter == nullptr ) return;

	hostCharacter->SetInteractionFlag(isHost , isInteraction);
	
}


