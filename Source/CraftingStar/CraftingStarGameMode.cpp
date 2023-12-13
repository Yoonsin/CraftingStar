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

	//SpawnLoc = FVector(165.f, 0.f, 124.f);
	//SpawnRot = FRotator(0.f, 0.f, 0.f);
}

void ACraftingStarGameMode::PostLogin(APlayerController* newPlayer)
{
	Super::PostLogin(newPlayer);

	ACraftingStarGS* CraftingStarGS = Cast<ACraftingStarGS>(UGameplayStatics::GetGameState(this));
	if (CraftingStarGS != nullptr) {
		if (CraftingStarGS->PlayerArray.Num() == 2) {

			//3초 딜레이 후 로드(바로 로드하면 PlayerState가 생성되지 않을 수도 있음)
			FTimerHandle DelayTimerHandle;
			float DelayTime = 3.0f;

			GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, FTimerDelegate::CreateLambda([&]()
				{
					//블루프린트 함수를 C++로 호출해야함
					auto GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
					UFunction* Function = GameInstance->GetClass()->FindFunctionByName(FName("Load"));


					if (Function == nullptr)
					{
						return;
					}

					struct FLocalParameters{};
					FLocalParameters Parameter;

					GameInstance->ProcessEvent(Function,&Parameter);


					// TimerHandle 초기화
					GetWorld()->GetTimerManager().ClearTimer(DelayTimerHandle);
				}), DelayTime, false);
			
		}
	}
}

void  ACraftingStarGameMode::InitGame()
{
	switch (NowMapName)
	{
	case EMapName::EWorldMap:
		break;
	case EMapName::EKeyStar:		
		if (ProgressLevel == 0) {
			//튜토리얼 시작
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

	//게임 종료시 누가 나갔는지 확인
	if (UUtilityFunction::IsHost(Exiting)) {
		UE_LOG(LogTemp, Log, TEXT("Host Logout"));
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Guest Logout"));
	}
	
}


void ACraftingStarGameMode::BeginPlay()
{
	Super::BeginPlay();

	//PlayerDied 델리게이트를 게임 모드의 PlayerDied 함수에 바인딩.
	if (!OnPlayerDied.IsBound())
	{
		OnPlayerDied.AddDynamic(this, &ACraftingStarGameMode::PlayerDied);
	}
}

void ACraftingStarGameMode::RestartPlayer(AController* NewPlayer) {
	Super::RestartPlayer(NewPlayer);

	////플레이어 스폰
	//FVector Location = FoundActors[NearsIdx]->GetActorLocation();
	//FRotator Rotation{ 0.0f, 0.0f, 0.0f };
	//FActorSpawnParameters PlayerSpawnParameters{};
	//PlayerSpawnParameters.Owner = this;

	////플레이어 컨트롤러 소유설정
	//APawn* Player_Character = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, Location, Rotation, PlayerSpawnParameters);
	//GetWorld()->GetFirstPlayerController()->Possess(Player_Character);
}




void ACraftingStarGameMode::RespawnPlayer(ACharacter* NewPlayer)
{
	SpawnLoc = NewPlayer->GetActorLocation();
	
	//모든 플레이어 스타트 배열을 순회
	//플레이어랑 제일 가까운 곳을 설정
	TArray<AActor*> FoundActors;
	float closestDist = 20000000.0f;
	int NearsIdx = 0;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);

	for (int i = 0; i < FoundActors.Num(); i++) {
		if (closestDist > FVector::Distance(FoundActors[i]->GetActorLocation(), SpawnLoc)) {
			closestDist = FVector::Distance(FoundActors[i]->GetActorLocation(), SpawnLoc);
			NearsIdx = i;
			
		}
	}

	//플레이어 위치 변경
	NewPlayer->SetActorLocation(FoundActors[NearsIdx]->GetActorLocation());

}



void ACraftingStarGameMode::PlayerDied(ACharacter* Character)
{
	//캐릭터의 플레이어 컨트롤러에 대한 레퍼런스 구하기
	AController* CharacterController = Character->GetController();
	RestartPlayer(CharacterController);


}