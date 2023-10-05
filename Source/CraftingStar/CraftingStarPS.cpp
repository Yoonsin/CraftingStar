// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingStarPS.h"

// Called when the game starts or when spawned
void ACraftingStarPS::BeginPlay()
{
	Super::BeginPlay();
	PlayerData.Mode = EPlayerRole::EDark;
	PlayerData.PlayerName = FString("DefalultName");
	PlayerData.AbleAbility.Init(true, (int)EPlayerAbility::ENone+1);
	
	//우선 사출과 염동력 능력만 가능하도록 설정
	//PlayerData.AbleAbility[(int)EPlayerAbility::EBlast] = true;
	//PlayerData.AbleAbility[(int)EPlayerAbility::ETelekinesis] = true;


}