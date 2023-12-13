// Copyright Epic Games, Inc. All Rights Reserved.

#include "CraftingStarGameMode.h"
#include "CraftingStarCharacter.h"
#include "CraftingStarPC.h"
#include "CraftingStarGS.h"
#include "Misc/OutputDeviceNull.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h" 
#include "UtilityFunction.h"

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

