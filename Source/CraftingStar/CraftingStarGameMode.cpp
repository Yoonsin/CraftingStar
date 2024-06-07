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

	//SpawnLoc = FVector(165.f, 0.f, 124.f);
	//SpawnRot = FRotator(0.f, 0.f, 0.f);
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


			serverCharacter->PlayerOutfit(gameInstance->nowSaveGame->HostData , gameInstance->nowSaveGame->GuestData);
			serverCharacter->PlayerUIInit();
			LoadFlag = false;
		}
		
	}
	else {
		
		ACraftingStarGS* gameState = Cast<ACraftingStarGS>(UGameplayStatics::GetGameState(this));
		if ( gameState == nullptr ) return;

		if ( gameState->isHostInit && gameState->isGuestInit ) {


			StartFlag = true;
			gameState->isStartFlag = true;
			
			//플레이어 데이터 로드

		
			

			//플레이어 모드에 맞는 메쉬와 UI 적용
			//for ( auto& playerPS : gameState->PlayerArray ) {
			//	ACraftingStarPS* playerState = Cast<ACraftingStarPS>(playerPS);
			//	if ( playerState == nullptr ) {
			//		GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("playerState is nullptr"));
			//		continue;
			//	}

			//	if ( UUtilityFunction::IsHost(Cast<APlayerController>(playerPS->GetOwner())) ) {	
			//		playerState->PlayerData = gameInstance->nowSaveGame->HostData;
			//	}
			//	else {
			//		playerState->PlayerData = gameInstance->nowSaveGame->GuestData;
			//	}
			//}


			
			GetWorld()->GetTimerManager().SetTimer(myTimerHandle , FTimerDelegate::CreateLambda([ & ] ()
			{
					LoadFlag = true;

					GetWorld()->GetTimerManager().ClearTimer(myTimerHandle);
			}) , 10.0f , false); // 반복 실행을 하고 싶으면 false 대신 true 대입
			
			

			//월드 내용 적용
			//worldStateLoad()
		
		}

		
		
		// 타이머 초기화

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

void ACraftingStarGameMode::PlayerDied(ACharacter* Character)
{
	//ĳ������ �÷��̾� ��Ʈ�ѷ��� ���� ���۷��� ���ϱ�
	AController* CharacterController = Character->GetController();
	RestartPlayer(CharacterController);


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


		//GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Red , FoundActors[i]->GetName());
		
		
		if ( FoundActors[i]->GetName().Contains(FString("PlayerStart_Origin")) ) {
			megetonOriginPlayerStart = Cast<APlayerStart>(FoundActors[i]);
		}
	}


	//origin spawn
	if ( Cast<ACraftingStarGS>(GetWorld()->GetGameState())->isOpenMegetonDoor == false ) {
		
		NewPlayer->SetActorLocation(megetonOriginPlayerStart->GetActorLocation());
		return;
	}

	//�÷��̾� ��ġ ����
	NewPlayer->SetActorLocation(FoundActors[NearsIdx]->GetActorLocation());

}



