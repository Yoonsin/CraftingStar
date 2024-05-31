// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingStarGS.h"
#include "Net/UnrealNetwork.h"


void ACraftingStarGS::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACraftingStarGS , ProgressData);
	DOREPLIFETIME(ACraftingStarGS , nowObtainingAbility);
	DOREPLIFETIME(ACraftingStarGS , isHostObtain);
	DOREPLIFETIME(ACraftingStarGS , isGuestObtain);
	DOREPLIFETIME(ACraftingStarGS , isStartFlag);


}
