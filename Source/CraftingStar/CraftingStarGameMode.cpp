// Copyright Epic Games, Inc. All Rights Reserved.

#include "CraftingStarGameMode.h"
#include "CraftingStarCharacter.h"
#include "CraftingStarPC.h"
#include "CraftingStarGS.h"
#include "Misc/OutputDeviceNull.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h" 
#include "GameFramework/PlayerStart.h"
#include "UtilityFunction.h"
#include "Math/Vector.h"

ACraftingStarGameMode::ACraftingStarGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/InGameMenu/Player/BP_CraftingStarCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		CharClass = PlayerPawnBPClass.Class;
	}	
	
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SpawnLoc = FVector(165.f, 0.f, 124.f);
	//SpawnRot = FRotator(0.f, 0.f, 0.f);
}

void ACraftingStarGameMode::PostLogin(APlayerController* newPlayer)
{
	Super::PostLogin(newPlayer);

	ACraftingStarGS* CraftingStarGS = Cast<ACraftingStarGS>(UGameplayStatics::GetGameState(this));
	if (CraftingStarGS != nullptr) {
		if (CraftingStarGS->PlayerArray.Num() == 2) {

			//3�� ����� �� �ε�(�ٷ� �ε��ϸ� PlayerState�� �������� ���� ���� ����)
			FTimerHandle DelayTimerHandle;
			float DelayTime = 3.0f;

			GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, FTimerDelegate::CreateLambda([&]()
				{
					//�������Ʈ �Լ��� C++�� ȣ���ؾ���
					auto GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
					UFunction* Function = GameInstance->GetClass()->FindFunctionByName(FName("Load"));


					if (Function == nullptr)
					{
						return;
					}

					struct FLocalParameters{};
					FLocalParameters Parameter;

					GameInstance->ProcessEvent(Function,&Parameter);


					// TimerHandle �ʱ�ȭ
					GetWorld()->GetTimerManager().ClearTimer(DelayTimerHandle);
				}), DelayTime, false);
			
		}
	}
}

void  ACraftingStarGameMode::InitGame()
{
	//모든 클라이언트에서 로딩창 제거
	ACharacter* hostCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld() , 0);
	if ( hostCharacter == nullptr ) return;
	ACraftingStarCharacter* host = Cast<ACraftingStarCharacter>(hostCharacter);
	if ( host == nullptr ) return;
	host->ServerStopLoadingWidget();


	//모든 클라이언트에서 모드 적용


	//맵 위치 + 진행도 에 따라 달라지는 초기화 로직
	switch (NowMapName)
	{
	case EMapName::EWorldMap:
		break;
	case EMapName::EKeyStar:		
		if (ProgressLevel == 0) {
			//Ʃ�丮�� ����
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Tutorial Start")));
		}
		else if (ProgressLevel == 1) {

		}
		break;
	default:
		break;
	}

	
}

void  ACraftingStarGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	
	//���� ����� ���� �������� Ȯ��
	if (UUtilityFunction::IsHost(Exiting)) {
		GEngine->AddOnScreenDebugMessage(-1 , 3 , FColor::Red , FString::Printf(TEXT("Host Logout")));


		 
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1 , 3 , FColor::Red , FString::Printf(TEXT("Guest Logout")));

		ACharacter* hostCharacter  = UGameplayStatics::GetPlayerCharacter(GetWorld() , 0);
		if ( hostCharacter == nullptr ) return;
		

		ACraftingStarCharacter* host = Cast<ACraftingStarCharacter>(hostCharacter);
		if ( host == nullptr ) return;

		
		//host->WorldMap();
		host->LogoutClient();
		

	}
	
}


void ACraftingStarGameMode::BeginPlay()
{
	Super::BeginPlay();

	//PlayerDied ��������Ʈ�� ���� ����� PlayerDied �Լ��� ���ε�.
	if (!OnPlayerDied.IsBound())
	{
		OnPlayerDied.AddDynamic(this, &ACraftingStarGameMode::PlayerDied);
	}
}

void ACraftingStarGameMode::RestartPlayer(AController* NewPlayer) {
	Super::RestartPlayer(NewPlayer);

	////�÷��̾� ����
	//FVector Location = FoundActors[NearsIdx]->GetActorLocation();
	//FRotator Rotation{ 0.0f, 0.0f, 0.0f };
	//FActorSpawnParameters PlayerSpawnParameters{};
	//PlayerSpawnParameters.Owner = this;

	////�÷��̾� ��Ʈ�ѷ� ��������
	//APawn* Player_Character = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, Location, Rotation, PlayerSpawnParameters);
	//GetWorld()->GetFirstPlayerController()->Possess(Player_Character);
}




void ACraftingStarGameMode::RespawnPlayer(ACharacter* NewPlayer)
{
	SpawnLoc = NewPlayer->GetActorLocation();
	
	//��� �÷��̾� ��ŸƮ �迭�� ��ȸ
	//�÷��̾�� ���� ����� ���� ����
	TArray<AActor*> FoundActors;
	float closestDist = 20000000.0f;
	int NearsIdx = 0;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
	GEngine->AddOnScreenDebugMessage(-1 , 2.0f , FColor::Red , FString::Printf(TEXT("FoundActor Num :: %d") , FoundActors.Num()));


	for (int i = 0; i < FoundActors.Num(); i++) {
		if (closestDist > FVector::Distance(FoundActors[i]->GetActorLocation(), SpawnLoc)) {
			closestDist = FVector::Distance(FoundActors[i]->GetActorLocation(), SpawnLoc);
			NearsIdx = i;
			
		}
	}

	//�÷��̾� ��ġ ����
	NewPlayer->SetActorLocation(FoundActors[NearsIdx]->GetActorLocation());

}



void ACraftingStarGameMode::PlayerDied(ACharacter* Character)
{
	//ĳ������ �÷��̾� ��Ʈ�ѷ��� ���� ���۷��� ���ϱ�
	AController* CharacterController = Character->GetController();
	RestartPlayer(CharacterController);


}