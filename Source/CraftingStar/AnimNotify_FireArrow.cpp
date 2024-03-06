// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_FireArrow.h"
#include "CraftingStarCharacter.h"
#include "BowComponent.h"

void UAnimNotify_FireArrow::Notify(USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp , Animation);

    auto Bow = Cast<UBowComponent>(MeshComp);
    if ( Bow )
    {
        Bow->ArrowSpawn();
    }
}