// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_WandReady.h"
#include "../../CraftingStarCharacter.h"

void UAnimNotify_WandReady::Notify(USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp , Animation);

    AActor* Owner = MeshComp->GetOwner();
    if ( Owner )
    {
        ACraftingStarCharacter* CharacterOwner = Cast<ACraftingStarCharacter>(Owner);
        if ( CharacterOwner )
        {
            CharacterOwner->WandReadySign = true;
            GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("ready true"));
        }
    }
}
