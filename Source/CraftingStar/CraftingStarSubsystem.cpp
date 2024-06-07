// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineSubsystemUtils.h"
#include "Kismet/GameplayStatics.h"
#include "CraftingStarSubsystem.h"

UCraftingStarSubsystem::UCraftingStarSubsystem() 
	: CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this,&ThisClass::OnCreateSessionCompleted)),
	UpdateSessionCompleteDelegate(FOnUpdateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnUpdateSessionCompleted)),
	DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this , &ThisClass::OnDestroySessionCompleted)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this , &ThisClass::OnJoinSessionCompleted))
{

}


void UCraftingStarSubsystem::CreateSession(int numPublicConnections , bool isLanMatch)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if ( !sessionInterface.IsValid() ) {
		OnCreateSessionCompleteEvent.Broadcast(false);
		return;
	}

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->NumPrivateConnections = numPublicConnections;
	LastSessionSettings->NumPublicConnections = numPublicConnections;
	LastSessionSettings->bAllowInvites = true;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
	LastSessionSettings->bIsDedicated = false;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bIsLANMatch = isLanMatch;
	LastSessionSettings->bShouldAdvertise = true;

	LastSessionSettings->Set(SETTING_MAPNAME , FString("L_MatchingMenu") , EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	CreateSessionCompleteDelegateHandle = sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	if ( !sessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId() , NAME_GameSession , *LastSessionSettings) ) {
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		OnCreateSessionCompleteEvent.Broadcast(false);
	}
}

void UCraftingStarSubsystem::OnCreateSessionCompleted(FName SessionName , bool Successful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if ( sessionInterface ) {
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	UGameplayStatics::OpenLevel(GetWorld() , FName("/Game/Maps/L_MatchingMenu") , true , ( (FString)( L"Listen" ) ));
	OnCreateSessionCompleteEvent.Broadcast(Successful);
}

void UCraftingStarSubsystem::UpdateSession()
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if ( !sessionInterface.IsValid() ) {
		OnUpdateSessionCompleteEvent.Broadcast(false);
		return;
	}

	TSharedPtr<FOnlineSessionSettings> updatedSessionSettings = MakeShareable(new FOnlineSessionSettings(*LastSessionSettings));
	//Update
	//updatedSessionSettings->Set(SETTING_MAPNAME , FString("Updated Level Name") , EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    
	UpdateSessionCompleteDelegateHandle = sessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegate);

	if ( !sessionInterface->UpdateSession(NAME_GameSession , *updatedSessionSettings) )
	{
		sessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);
		OnUpdateSessionCompleteEvent.Broadcast(false);
	}
	else
	{
		LastSessionSettings = updatedSessionSettings;
	}

}

void UCraftingStarSubsystem::OnUpdateSessionCompleted(FName SessionName , bool Successful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if ( sessionInterface ) {
		sessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);
	}

	OnUpdateSessionCompleteEvent.Broadcast(Successful);
}

void UCraftingStarSubsystem::DestroySession()
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if ( !sessionInterface.IsValid() )
	{

		OnDestroySessionCompleteEvent.Broadcast(false);
		return;
	}

	DestroySessionCompleteDelegateHandle = sessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	if ( !sessionInterface->DestroySession(NAME_GameSession) )
	{
		sessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);

		OnDestroySessionCompleteEvent.Broadcast(false);
	}
}

void UCraftingStarSubsystem::OnDestroySessionCompleted(FName SessionName , bool Successful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if ( sessionInterface )
	{
		sessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}
	OnDestroySessionCompleteEvent.Broadcast(Successful);
}

void UCraftingStarSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult) {
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if ( !sessionInterface.IsValid() )
	{
		OnJoinGameSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	JoinSessionCompleteDelegateHandle = sessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if ( !sessionInterface->JoinSession(*localPlayer->GetPreferredUniqueNetId() , NAME_GameSession , SessionResult) )
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

		OnJoinGameSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}

}

void  UCraftingStarSubsystem::OnJoinSessionCompleted(FName SessionName , EOnJoinSessionCompleteResult::Type Result)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if ( sessionInterface )
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	OnJoinGameSessionCompleteEvent.Broadcast(Result);
}

bool  UCraftingStarSubsystem::TryTravelToCurrentSession()
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if ( !sessionInterface.IsValid() )
	{
		return false;
	}

	FString connectString;
	if ( !sessionInterface->GetResolvedConnectString(NAME_GameSession , connectString) )
	{
		return false;
	}

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	playerController->ClientTravel(connectString , TRAVEL_Absolute);
	return true;
}