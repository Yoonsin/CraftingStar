// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_TwoHandedEnd.h"
#include "../../CraftingStarCharacter.h"
#include "../WeaponComponent.h"

void UAnimNotify_TwoHandedEnd::Notify(USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp , Animation);

    AActor* Owner = MeshComp->GetOwner();
    if ( Owner )
    {
        ACraftingStarCharacter* CharacterOwner = Cast<ACraftingStarCharacter>(Owner);
        if ( CharacterOwner )
        {
            CharacterOwner->GetWeaponComponent()->AttackEnd();
        }
    }
}