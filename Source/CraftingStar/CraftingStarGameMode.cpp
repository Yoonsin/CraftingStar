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
		CharClass = PlayerPawnBPClass.Class;
	}

	SpawnLoc = FVector(165.f, 0.f, 124.f);
	SpawnRot = FRotator(0.f, 0.f, 0.f);
}

void ACraftingStarGameMode::PostLogin(APlayerController* newPlayer)
{
	Super::PostLogin(newPlayer);

	/*
	if (ACraftingStarCharacter* Character = GetWorld()->SpawnActor<ACraftingStarCharacter>(CharClass,SpawnLoc,SpawnRot)) {
		if (ACraftingStarPC* Controller = Cast<ACraftingStarPC>(newPlayer))
		{
			Controller->Possess(Character);
		}
	} */
	
}

