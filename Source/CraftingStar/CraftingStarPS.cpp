// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingStarPS.h"
#include "CraftingStarGS.h"
#include "UtilityFunction.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "CraftingStarCharacter.h"

ACraftingStarPS::ACraftingStarPS()
{
	//bReplicates = true;
}

void ACraftingStarPS::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACraftingStarPS, NowDisplayAbility);
	DOREPLIFETIME(ACraftingStarPS, NowDisplayState);
	DOREPLIFETIME(ACraftingStarPS, Health);
	DOREPLIFETIME(ACraftingStarPS, PlayerData);
}

// Called when the game starts or when spawned
void ACraftingStarPS::BeginPlay()
{
	Super::BeginPlay();

	//this->SetReplicates(true);
	//bAlwaysRelevant = true;

	PlayerData.Mode = EPlayerRole::EDark;
	PlayerData.PlayerName = FString("DefalultName");
	
	//원래는 전부 false이지만 디버그를 위해 서버로 바꿔줌
	//PlayerData.AbleAbility.Init(false, (int)EPlayerAbility::ENone+1);
	
	
	//�켱 ����� ������ �ɷ¸� �����ϵ��� ����
	//PlayerData.AbleAbility[(int)EPlayerAbility::EBlast] = true;
	//PlayerData.AbleAbility[(int)EPlayerAbility::ETelekinesis] = true;

}


void ACraftingStarPS::RequestPlayerAbility_Implementation(EPlayerAbility playerAbility)
{
	NowDisplayAbility = playerAbility;
}

void ACraftingStarPS::RequestPlayerGMState_Implementation(EPlayerGMState playerGMState)
{
	NowDisplayState = playerGMState;
}

void ACraftingStarPS::RequestHealth_Implementation(float Damage)
{
	if (Health - Damage < 0.0f)Health = 0.0f;
	else Health -= Damage;

}

void ACraftingStarPS::RequestSave_Implementation()
{
	
	if (UUtilityFunction::IsHost(Cast<AController>(GetOwner())))
	{
		//호스트
		//캐릭터 위치 업데이트
		ACraftingStarGS* CraftingStarGS = Cast<ACraftingStarGS>(UGameplayStatics::GetGameState(this));
		ACraftingStarCharacter* CraftingStarCharacter = Cast<ACraftingStarCharacter>(Cast<AController>(GetOwner())->GetPawn());
		CraftingStarGS->ProgressData.HostPlayerPos = CraftingStarCharacter->GetTransform();
	}
	else {
		//게스트
		
		//서버한테 데이터 업데이트 요청
		RequestClientUpdate(PlayerData);
	}
}

void ACraftingStarPS::RequestClientUpdate_Implementation(FPlayerData playerData)
{
	//캐릭터 위치 업데이트
	ACraftingStarGS* CraftingStarGS = Cast<ACraftingStarGS>(UGameplayStatics::GetGameState(this));
	ACraftingStarCharacter* CraftingStarCharacter = Cast<ACraftingStarCharacter>(Cast<AController>(GetOwner())->GetPawn());
	CraftingStarGS->ProgressData.GuestPlayerPos = CraftingStarCharacter->GetTransform();

	//PS 업데이트
	PlayerData = playerData;
}



