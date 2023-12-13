// Fill out your copyright notice in the Description page of Project Settings.

#include "Net/UnrealNetwork.h"
#include "CraftingStarGS.h"

ACraftingStarGS::ACraftingStarGS()
{
	KeyNum = 0;
}

void ACraftingStarGS::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACraftingStarGS, ProgressData);
	DOREPLIFETIME(ACraftingStarGS, KeyNum);
}

void  ACraftingStarGS::UpdateProgressLevel_Implementation(int level)
{
	ProgressData.ProgressLevel = level;

	//게임모드 값의 ProgressLevel도 업데이트
	
}