// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityFunction.h"
#include "GameFramework/Controller.h"
#include "CraftingStarGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

bool UUtilityFunction::IsHost(AController* Controller) {

	if (Controller == nullptr) {
		return false;
	}
	return Controller->IsLocalPlayerController() && Controller->HasAuthority();
}

bool UUtilityFunction::Save(UGameInstance * instance) {
	
	UCraftingStarGameInstance* gameInstance = Cast<UCraftingStarGameInstance>(instance);
	if ( gameInstance == nullptr )return false;

	return gameInstance->SaveFile(gameInstance->NowSaveIdx);
}

