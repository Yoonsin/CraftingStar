// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CraftingStarSubsystem.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnCreateSessionComplete , bool , Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnUpdateSessionComplete , bool , Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnDestroySessionComplete , bool , Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FCSOnJoinSessionComplete , EOnJoinSessionCompleteResult::Type Result);
UCLASS()
class CRAFTINGSTAR_API UCraftingStarSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UCraftingStarSubsystem();

	//create
	UFUNCTION(BlueprintCallable)
	void CreateSession(int numPublicConnections , bool isLanMatch);
	
	FCSOnCreateSessionComplete OnCreateSessionCompleteEvent;

	//update
	UFUNCTION(BlueprintCallable)
	void UpdateSession();

	FCSOnUpdateSessionComplete OnUpdateSessionCompleteEvent;

	//delete
	UFUNCTION(BlueprintCallable)
	void DestroySession();

	FCSOnDestroySessionComplete OnDestroySessionCompleteEvent;

	//join
	//FonlineSessionSearchResult 가 블루프린트에 쓸 수 없는 구조체라 블루프린트에서 호출할 수 없음
	void JoinSession(const FOnlineSessionSearchResult& SessionResult);

	FCSOnJoinSessionComplete OnJoinGameSessionCompleteEvent;

protected:
	//create
	void OnCreateSessionCompleted(FName SessionName , bool Successful);

	//update
	void OnUpdateSessionCompleted(FName SessionName , bool Successful);

	//delete
	void OnDestroySessionCompleted(FName SessionName , bool Successful);

	//join
	void OnJoinSessionCompleted(FName SessionName , EOnJoinSessionCompleteResult::Type Result);
	bool TryTravelToCurrentSession();

private:
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	
	//create
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;

	//update
	FOnUpdateSessionCompleteDelegate UpdateSessionCompleteDelegate;
	FDelegateHandle UpdateSessionCompleteDelegateHandle;
	
	//delete
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;

	//join
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
};
