// Fill out your copyright notice in the Description page of Project Settings.


#include "AssimilationTrigerComponent.h"

void UAssimilationTrigerComponent::ChaseStart_Implementation(class UAssimilationComponent* AssmitationComp)
{
	UE_LOG(LogTemp , Warning , TEXT("Chase Start"));
}

void UAssimilationTrigerComponent::ChaseEnd_Implementation(class UAssimilationComponent* AssmitationComp)
{
	UE_LOG(LogTemp , Warning , TEXT("Chase End"));
}