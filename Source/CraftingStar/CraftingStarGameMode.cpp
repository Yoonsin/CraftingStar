// Copyright Epic Games, Inc. All Rights Reserved.

#include "CraftingStarGameMode.h"
#include "CraftingStarCharacter.h"
#include "CraftingStarPC.h"
#include "CraftingStarGS.h"
#include "CraftingStarPS.h"
#include "CraftingStarGameInstance.h"
#include "Misc/OutputDeviceNull.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h" 
#include "GameFramework/PlayerStart.h"
#include "GameFramework/GameSession.h"
#include "UtilityFunction.h"
#include "Math/Vector.h"

ACraftingStarGameMode::ACraftingStarGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("'/Game/CraftingStarCharacter/BP_CraftingStarCharacter'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		CharClass = PlayerPawnBPClass.Class;
	}	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ACraftingStarGameMode::PostLogin(APlayerController* newPlayer)
{
	Super::PostLogin(newPlayer);
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

void ACraftingStarGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if ( StartFlag ) {
		if ( LoadFlag == true ) {
			ACraftingStarCharacter* serverCharacter = Cast<ACraftingStarCharacter>(UGameplayStatics::GetPlayerCharacter(this , 0));
			if ( serverCharacter == nullptr ) return;
			UCraftingStarGameInstance* gameInstance = Cast<UCraftingStarGameInstance>(GetGameInstance());
			if ( gameInstance == nullptr ) return;

			//플레이어 데이터 로드
			serverCharacter->PlayerOutfit(gameInstance->nowSaveGame->HostData , gameInstance->nowSaveGame->GuestData);
			serverCharacter->PlayerUIInit();

			//월드 데이터 로드
			LoadWorldData(gameInstance->nowSaveGame);

			//퀘스트 데이터 로드
			LoadQuestData(gameInstance->nowSaveGame);

			//데이터 전부 로드하면 세이브
			UUtilityFunction::Save(gameInstance);
			LoadFlag = false;
		}	
	}
	else {
		ACraftingStarGS* gameState = Cast<ACraftingStarGS>(UGameplayStatics::GetGameState(this));
		if ( gameState == nullptr ) return;
		if ( gameState->isHostInit && gameState->isGuestInit ) {
			StartFlag = true;
			gameState->isStartFlag = true;

			GetWorld()->GetTimerManager().SetTimer(myTimerHandle , FTimerDelegate::CreateLambda([ & ] ()
			{
					LoadFlag = true;	
					GetWorld()->GetTimerManager().ClearTimer(myTimerHandle);
			}) , 5.0f , false); // 반복 실행을 하고 싶으면 false 대신 true 대입
			
			//타이머 콜백 불리기 전 게임 종료시 타이머 꺼야 오류안남
		}	
		// 타이머 초기화
	}
}


void ACraftingStarGameMode::RestartPlayer(AController* NewPlayer) 
{
	Super::RestartPlayer(NewPlayer);
}

void ACraftingStarGameMode::PlayerDied(ACharacter* Character)
{
	AController* CharacterController = Character->GetController();
	RestartPlayer(CharacterController);
}

void ACraftingStarGameMode::RespawnPlayer(ACharacter* NewPlayer)
{
	SpawnLoc = NewPlayer->GetActorLocation();
	
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
		//GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Red , FoundActors[i]->GetName());
		if ( FoundActors[i]->GetName().Contains(FString("PlayerStart_Origin")) ) {
			megetonOriginPlayerStart = Cast<APlayerStart>(FoundActors[i]);
		}
	}

	//origin spawn
	if ( Cast<ACraftingStarGS>(GetWorld()->GetGameState())->isOpenMegetonDoor == false && megetonOriginPlayerStart != nullptr) {
		
		NewPlayer->SetActorLocation(megetonOriginPlayerStart->GetActorLocation());
		megetonOriginPlayerStart = nullptr;
		return;
	}
	megetonOriginPlayerStart = nullptr;
	NewPlayer->SetActorLocation(FoundActors[NearsIdx]->GetActorLocation());
}





