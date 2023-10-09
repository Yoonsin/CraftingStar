// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingStarPS.h"
#include "CraftingStarGS.h"
#include "UtilityFunction.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

ACraftingStarPS::ACraftingStarPS()
{

}

void ACraftingStarPS::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACraftingStarPS, NowAbility);
	DOREPLIFETIME(ACraftingStarPS, NowState);
	DOREPLIFETIME(ACraftingStarPS, Health);
}

// Called when the game starts or when spawned
void ACraftingStarPS::BeginPlay()
{
	Super::BeginPlay();
	PlayerData.Mode = EPlayerRole::EDark;
	PlayerData.PlayerName = FString("DefalultName");
	
	//false�� �ϸ� �ȷ�Ʈ �ɷ� ���� �������� ����
	PlayerData.AbleAbility.Init(true, (int)EPlayerAbility::ENone+1);
	
	//�켱 ����� ������ �ɷ¸� �����ϵ��� ����
	//PlayerData.AbleAbility[(int)EPlayerAbility::EBlast] = true;
	//PlayerData.AbleAbility[(int)EPlayerAbility::ETelekinesis] = true;

}


void ACraftingStarPS::RequestPlayerAbility_Implementation(EPlayerAbility playerAbility)
{
	NowAbility = playerAbility;
}

void ACraftingStarPS::RequestPlayerGMState_Implementation(EPlayerGMState playerGMState)
{
	NowState = playerGMState;
}

void ACraftingStarPS::RequestHealth_Implementation(float Damage)
{
	if (Health - Damage < 0.0f)Health = 0.0f;
	else Health -= Damage;

}

