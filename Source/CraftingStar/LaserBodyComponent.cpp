// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserBodyComponent.h"

ULaserBodyComponent::ULaserBodyComponent() {
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void ULaserBodyComponent::BeginPlay() {
	Super::BeginPlay();

	if ( NiagaraSystem ) {
		SetAsset(NiagaraSystem);
		Activate();
	}
}

void ULaserBodyComponent::TickComponent(float DeltaTime , ELevelTick TickType , FActorComponentTickFunction * ThisTickFunction) {
	Super::TickComponent(DeltaTime , TickType , ThisTickFunction);
}
