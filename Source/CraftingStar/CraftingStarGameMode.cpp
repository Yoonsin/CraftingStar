// Copyright Epic Games, Inc. All Rights Reserved.

#include "CraftingStarGameMode.h"
#include "CraftingStarCharacter.h"
#include "CraftingStarPC.h"
#include "UObject/ConstructorHelpers.h"

ACraftingStarGameMode::ACraftingStarGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/InGameMenu/Player/BP_CraftingStarCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

