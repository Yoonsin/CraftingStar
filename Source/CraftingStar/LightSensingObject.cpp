// Fill out your copyright notice in the Description page of Project Settings.


#include "LightSensingObject.h"

void ALightSensingObject::Perform_Implementation()
{
	UE_LOG(LogTemp , Display , TEXT("LightObject"));
}

void ALightSensingObject::React_Implementation(FVector Location)
{
	UE_LOG(LogTemp , Display , TEXT("Light Location X: %f   Y: %f"), Location.X , Location.Y);
}
