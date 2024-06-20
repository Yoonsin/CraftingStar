// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "CraftingStarSaveGame.h"
#include "CraftingStarGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CRAFTINGSTAR_API UCraftingStarGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()

	
	
public:

	
	UCraftingStarGameInstance(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class UCraftingStarSaveGame* nowSaveGame;
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class UCraftingStarSaveGame* debugSaveGame;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	TArray<class UCraftingStarSaveGame*> SaveGames;
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	int NowSaveIdx;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	TMap<EMapName , FSpawnTransform> MapSpawnDict;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	bool isDebug = false;
	
	UFUNCTION(BlueprintCallable)
	bool SetDebugFile();

	UFUNCTION(BlueprintCallable)
	bool SetNowFile(int targetIdx);

	UFUNCTION(BlueprintCallable)
	bool SaveFile(int targetIdx);

	UFUNCTION(BlueprintCallable)
	bool CreateFile(int targetIdx, EPlayerRole ServerMode, EPlayerRole GuestMode);

	UFUNCTION(BlueprintCallable)
	bool CheckFile(int targetIdx);

	UFUNCTION(BlueprintCallable)
	bool DeleteFile(int targetIdx);

	UFUNCTION(BlueprintCallable)
	bool StartServer(int numPublicConnections , bool isLanMatch);


	bool JoinSession(FOnlineSessionSearchResult InviteResult);

	//( FOnSessionUserInviteAccepted , const bool , const int32 , FUniqueNetIdPtr , const FOnlineSessionSearchResult& );
	void OnSessionInviteAccepted(const bool bWasSuccessful , const int32 ControllerId , FUniqueNetIdPtr UserId , const FOnlineSessionSearchResult& InviteResult);

	UFUNCTION(BlueprintCallable)
	void ChangeSessionResultBlueprint(FBlueprintSessionResult SearchResult);

};
