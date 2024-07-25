// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingStarGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "CraftingStarGS.h"
#include "UtilityFunction.h"
#include "GameFramework/PlayerStart.h"
#include "CraftingStarPS.h"
#include "CraftingStarPC.h"
#include "CraftingStarSubsystem.h"
#include "CraftingStarCharacter.h"
#include "MoviePlayer.h"


UCraftingStarGameInstance::UCraftingStarGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
    SaveGames.Init(nullptr , 3);
	NowSaveIdx = 0;

	FSpawnTransform KeyStarTransform;
	KeyStarTransform.HostPlayerPos.SetLocation(FVector(-90.0f,0.0f,82.0f));
	KeyStarTransform.HostPlayerPos.SetRotation(FQuat(0.0f , 0.0f , -90.0f , 0.0f));
	KeyStarTransform.GuestPlayerPos.SetLocation(FVector(292.0f , 0.0f , 82.0f));
	KeyStarTransform.GuestPlayerPos.SetRotation(FQuat(0.0f , 0.0f , -90.0f , 0.0f));
	
	MapSpawnDict.Add(EMapName::EKeyStar , KeyStarTransform);

	//SessionInviteAcceptedDelegate(FOnSessionUserInviteAcceptedDelegate::CreateUObject(this , &ThisClass::OnSessionInviteAccepted)
	//SessionInviteAcceptedDelegate.BindUObject(this,&ThisClass::OnSessionInviteAccepted);

	
}

//얘는 블프가 더 편할 것 같기도 하고
bool UCraftingStarGameInstance::SetDebugFile()
{

	debugSaveGame =  Cast< UCraftingStarSaveGame>(UGameplayStatics::CreateSaveGameObject(UCraftingStarSaveGame::StaticClass()));
	debugSaveGame->HostData.AbleAbility.Init(false , (int)EPlayerAbility::ENone + 1);
	debugSaveGame->HostData.Mode = EPlayerRole::EDark;

	debugSaveGame->GuestData.AbleAbility.Init(false , (int)EPlayerAbility::ENone + 1);
	debugSaveGame->GuestData.Mode = EPlayerRole::ELight;

	debugSaveGame->ProgressData.NowMapName = EMapName::EKeyStar;
	debugSaveGame->ProgressData.ProgressLevel = 0;
	debugSaveGame->ProgressData.questID = EQuestID::EMegetonNotMeet;
	debugSaveGame->ProgressData.HostPlayerPos = MapSpawnDict[EMapName::EKeyStar].HostPlayerPos;
	debugSaveGame->ProgressData.GuestPlayerPos = MapSpawnDict[EMapName::EKeyStar].GuestPlayerPos;

	nowSaveGame = debugSaveGame;
	return true;
}

bool UCraftingStarGameInstance::SetNowFile(int targetIdx)
{
	FString checkFileName = "CraftingStarGame";
	checkFileName.Append(FString::FromInt(targetIdx));

	UCraftingStarSaveGame* checkFile = Cast< UCraftingStarSaveGame>(UGameplayStatics::LoadGameFromSlot(checkFileName , 0));

	if ( checkFile == nullptr ) {
		//checkFile is Empty
		return false;
	}
	else {
		nowSaveGame = checkFile;
		return true;
	}
}

bool UCraftingStarGameInstance::SaveFile( int targetIdx )
{
	UCraftingStarSaveGame* savingFile = SaveGames[targetIdx];
	if ( savingFile == nullptr ) return false;

	ACraftingStarGS* gameState = Cast<ACraftingStarGS>(GetWorld()->GetGameState());
	if ( gameState == nullptr ) return false;

	savingFile->ProgressData = gameState->ProgressData;
	for( APlayerState * ps : gameState->PlayerArray ) {
		ACraftingStarPS* playerState = Cast<ACraftingStarPS>(ps);
		if ( playerState == nullptr ) return false;

		ACraftingStarPC* playerController = Cast<ACraftingStarPC>(playerState->GetOwner());
		 ACraftingStarCharacter* character = Cast<ACraftingStarCharacter>(ps->GetPawn());
		if ( playerController == nullptr ) return false;

		if ( UUtilityFunction::IsHost(playerController) ) {
			//GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("Host transform %f / %f / %f") , pos.X , pos.Y , pos.Z));
			savingFile->HostData = playerState->PlayerData;

			if ( savingFile->ProgressData.questID == EQuestID::EMegetonTransition ) continue;

			FTransform PlayerStartPos = GetClosestDistTransform(character->GetTransform().GetLocation());

			//플레이어 스타트에서 살짝 떨어진 위치로 각각 설정
			FTransform HostPlayerPos = PlayerStartPos;
			HostPlayerPos.SetLocation(PlayerStartPos.GetLocation() - FVector(100 , 0 , 0));

			FTransform GuestPlayerPos = PlayerStartPos;
			GuestPlayerPos.SetLocation(PlayerStartPos.GetLocation() + FVector(100 , 0 , 0));

			savingFile->ProgressData.HostPlayerPos = HostPlayerPos;
			savingFile->ProgressData.GuestPlayerPos = GuestPlayerPos;

		}
		else {
			savingFile->GuestData = playerState->PlayerData;
		}
	}



	FString creatingFileName = "CraftingStarGame";
	creatingFileName.Append(FString::FromInt(targetIdx));
	

	//인게임 -> 인게임 전환시에는 NowSaveGame을 설정해 줘야함
	//(Load는 로비->인게임 넘어갈 때 한번밖에 안하기 때문)
	if ( savingFile->ProgressData.questID == EQuestID::EMegetonTransition ) {
		savingFile->ProgressData.questID = EQuestID::EIncendieDefault;
		nowSaveGame = savingFile;
	}

	return UGameplayStatics::SaveGameToSlot(savingFile , creatingFileName , 0);
}

bool UCraftingStarGameInstance::CreateFile( int targetIdx , EPlayerRole ServerMode , EPlayerRole GuestMode)
{
	FString creatingFileName = "CraftingStarGame";
	creatingFileName.Append(FString::FromInt(targetIdx));

	UCraftingStarSaveGame* creatingFile = Cast< UCraftingStarSaveGame>(UGameplayStatics::CreateSaveGameObject(UCraftingStarSaveGame::StaticClass()));

	if ( creatingFile == nullptr ) return false;

	creatingFile->HostData.AbleAbility.Init(false , (int)EPlayerAbility::ENone + 1);
	creatingFile->HostData.Mode = ServerMode;

	creatingFile->GuestData.AbleAbility.Init(false , (int)EPlayerAbility::ENone + 1);
	creatingFile->GuestData.Mode = GuestMode;

	creatingFile->ProgressData.NowMapName = EMapName::EKeyStar;
	creatingFile->ProgressData.ProgressLevel = 0;
	creatingFile->ProgressData.questID = EQuestID::EMegetonNotMeet;
	creatingFile->ProgressData.HostPlayerPos = MapSpawnDict[EMapName::EKeyStar].HostPlayerPos;
	creatingFile->ProgressData.GuestPlayerPos = MapSpawnDict[EMapName::EKeyStar].GuestPlayerPos;
	//creatingFile->ProgressData.WorldObjects.Init()

	SaveGames[targetIdx] = creatingFile;
	return UGameplayStatics::SaveGameToSlot(SaveGames[targetIdx] , creatingFileName , 0);
}

bool UCraftingStarGameInstance::CheckFile( int targetIdx )
{
	FString checkFileName = "CraftingStarGame";
	checkFileName.Append(FString::FromInt(targetIdx));

	UCraftingStarSaveGame* checkFile = Cast< UCraftingStarSaveGame>(UGameplayStatics::LoadGameFromSlot(checkFileName , 0));

	if ( checkFile == nullptr ) {
		//creatingFile is Empty
		return false;
	}
	else {
		//cratingFile is Exist
		SaveGames[targetIdx] = checkFile;
		return true;
	}

}

bool UCraftingStarGameInstance::DeleteFile( int targetIdx )
{
	FString deletingFileName = "CraftingStarGame";
	deletingFileName.Append(FString::FromInt(targetIdx));

	return UGameplayStatics::DeleteGameInSlot(deletingFileName , 0);
}

bool UCraftingStarGameInstance::StartServer(int numPublicConnections , bool isLanMatch)
{
	UCraftingStarSubsystem* subSystem = GetSubsystem<UCraftingStarSubsystem>();
	if ( subSystem == nullptr ) return false;

	subSystem->CreateSession(numPublicConnections,isLanMatch);
	return true;
}

bool UCraftingStarGameInstance::JoinSession(FOnlineSessionSearchResult result)
{
	UCraftingStarSubsystem* subSystem = GetSubsystem<UCraftingStarSubsystem>();
	if ( subSystem == nullptr ) return false;

	subSystem->JoinSession(result);
	return true;
}

void UCraftingStarGameInstance::OnSessionInviteAccepted(const bool bWasSuccessful , const int32 ControllerId , FUniqueNetIdPtr UserId , const FOnlineSessionSearchResult& InviteResult) {

	if ( bWasSuccessful ) {
		GEngine->AddOnScreenDebugMessage(-1 , 3 , FColor::Red , FString::Printf(TEXT("Session Invite Accepted")));

		if ( InviteResult.IsValid() ) {
			GEngine->AddOnScreenDebugMessage(-1 , 3 , FColor::Red , FString::Printf(TEXT("Session isVaild")));

			UCraftingStarSubsystem* subSystem = GetSubsystem<UCraftingStarSubsystem>();
			if ( subSystem == nullptr ) return;
			
			subSystem->JoinSession(InviteResult);
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1 , 3 , FColor::Red , FString::Printf(TEXT("Session Invite Accepted fail")));
	}
	GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("InvitedAccepted"));

	
}

void UCraftingStarGameInstance::ChangeSessionResultBlueprint(FBlueprintSessionResult SearchResult) {
	FOnlineSessionSearchResult result;

	result = SearchResult.OnlineResult;
	JoinSession(result);
}

FTransform UCraftingStarGameInstance::GetClosestDistTransform(FVector NowPlayerPos) {
	
	APlayerStart* megetonOriginPlayerStart = nullptr;
	TArray<AActor*> FoundActors;
	float closestDist = 20000000.0f;
	int NearsIdx = 0;
	UGameplayStatics::GetAllActorsOfClass(GetWorld() , APlayerStart::StaticClass() , FoundActors);
	


	for ( int i = 0; i < FoundActors.Num(); i++ ) {
		if ( closestDist > FVector::Distance(FoundActors[i]->GetActorLocation() , NowPlayerPos) ) {
			closestDist = FVector::Distance(FoundActors[i]->GetActorLocation() , NowPlayerPos);
			NearsIdx = i;
		}

		if ( FoundActors[i]->GetName().Contains(FString("PlayerStart_Origin")) ) {
			megetonOriginPlayerStart = Cast<APlayerStart>(FoundActors[i]);
		}
	}



	if ( Cast<ACraftingStarGS>(GetWorld()->GetGameState())->isOpenMegetonDoor == false && megetonOriginPlayerStart != nullptr) {
		return megetonOriginPlayerStart->GetActorTransform();
	}

	
	return FoundActors[NearsIdx]->GetActorTransform();


}


void UCraftingStarGameInstance::BeginLoadingScreen()
{
	if ( !IsRunningDedicatedServer() )
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.MinimumLoadingScreenDisplayTime = 5.0f;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.bAllowEngineTick = true;
		LoadingScreen.bMoviesAreSkippable = false;
		LoadingScreen.PlaybackType = EMoviePlaybackType::MT_Normal;

		//LoadingScreen.MoviePaths.Add(TEXT("example"));
		LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);

		GEngine->AddOnScreenDebugMessage(-1 , 5.0f , FColor::Red , FString::Printf(TEXT("Set Loading Screen")));
	}

}

void UCraftingStarGameInstance::EndLoadingScreen()
{
	GetMoviePlayer()->StopMovie();
}