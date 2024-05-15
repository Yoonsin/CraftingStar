// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingStarPS.h"
#include "CraftingStarGS.h"
#include "UtilityFunction.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"
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

	//PlayerData.Mode = EPlayerRole::EDark;
	//PlayerData.PlayerName = FString("DefalultName");
	
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

void ACraftingStarPS::RequestSave()
{
	if (UUtilityFunction::IsHost(Cast<AController>(GetOwner())))
	{
		

		FOutputDeviceNull Ar;
	    CallFunctionByNameWithArguments(TEXT("Save") , Ar , nullptr , true);

		auto GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
        UFunction* Function = GameInstance->GetClass()->FindFunctionByName(FName("Save"));

		if (Function == nullptr)
	    {
			GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Red , FString::Printf(TEXT("Save Failed")));
			return;
        }

		struct FLocalParameters{};
		FLocalParameters Parameter;

        GameInstance->ProcessEvent(Function,&Parameter);
		//GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Red , FString::Printf(TEXT("HOST X : %f Y : %f Z : %f") , temp.X , temp.Y , temp.Z));
		GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Red , FString::Printf(TEXT("Save Clear") ));
	}
	
}

void ACraftingStarPS::RequestClientUpdate_Implementation(FPlayerData playerData , FTransform transform)
{
	//캐릭터 위치 업데이트
	ACraftingStarGS* CraftingStarGS = Cast<ACraftingStarGS>(UGameplayStatics::GetGameState(this));
	//ACraftingStarCharacter* CraftingStarCharacter = Cast<ACraftingStarCharacter>(Cast<AController>(GetOwner())->GetPawn());
	CraftingStarGS->ProgressData.GuestPlayerPos = transform;

	FVector temp = CraftingStarGS->ProgressData.GuestPlayerPos.GetLocation();
	GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Red , FString::Printf(TEXT("GUEST X : %f Y : %f Z : %f") , temp.X , temp.Y , temp.Z));

	//PS 업데이트
}

void ACraftingStarPS::RequestObtainingAbilityUpdate(EPlayerAbility ability)
{
	if ( ability != EPlayerAbility::ENone ) {
		ServerObtainingAbilityUpdate(ability);
	}
}

void ACraftingStarPS::ServerObtainingAbilityUpdate_Implementation(EPlayerAbility ability)
{
	if(ability != EPlayerAbility::ENone ){
	  PlayerData.AbleAbility[(uint8)ability] = true;
	}
}



