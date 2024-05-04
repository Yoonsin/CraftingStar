// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_BowEnd.h"
#include "../../CraftingStarCharacter.h"
#include "../BowComponent.h"

void UAnimNotify_BowEnd::Notify(USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp , Animation);

    AActor* Owner = MeshComp->GetOwner();
    if ( Owner )
    {
        ACraftingStarCharacter* CharacterOwner = Cast<ACraftingStarCharacter>(Owner);
        if ( CharacterOwner )
        {
            CharacterOwner->GetBowComponent()->ShootEnd();
        }
    }
}