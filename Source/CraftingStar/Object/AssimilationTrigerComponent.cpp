// Fill out your copyright notice in the Description page of Project Settings.


#include "AssimilationTrigerComponent.h"
#include "AssimilationObject.h"
#include "../Ability/AssimilationComponent.h"

void UAssimilationTrigerComponent::ChaseStart_Implementation(class UAssimilationComponent* AssimilationComp)
{
	if ( auto Object = Cast<AAssimilationObject>(GetOwner()) )
	{
		Object->SetChasing(true);
		AssimilationComp->HideOwner(true);
		AssimilationComp->ShowHaloEffect(true);
	}
	/*
	if ( GetOwner()->Implements<USplineChasingInterface>() )
	{
		UE_LOG(LogTemp , Warning , TEXT("Chase Start"));
		Execute_ChaseStart(GetOwner() , AssimilationComp);
	}
	*/
}

void UAssimilationTrigerComponent::ChaseEnd_Implementation(class UAssimilationComponent* AssimilationComp)
{
	if ( auto Object = Cast<AAssimilationObject>(GetOwner()) )
	{
		Object->SetChasing(false);
		if ( AssimilationComp )
		{
			AssimilationComp->HideOwner(false);
			AssimilationComp->ShowHaloEffect(false);
		}
		else UE_LOG(LogTemp , Display , TEXT("Why Not"));
			
		SetAssimilationComponent(NULL);
	}
	
}


void UAssimilationTrigerComponent::SetAssimilationComponent(class UAssimilationComponent* AssimilationComp)
{
	this->AssimilationComponent = AssimilationComp;
}