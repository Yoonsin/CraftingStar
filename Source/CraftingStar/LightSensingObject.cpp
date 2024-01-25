// Fill out your copyright notice in the Description page of Project Settings.


#include "LightSensingObject.h"

void ALightSensingObject::Perform_Implementation()
{
	UE_LOG(LogTemp , Display , TEXT("LightObject"));
}

void ALightSensingObject::React_Implementation(bool bLightCharacter, FVector Location)
{
	if ( bLightCharacter )
	{
		UE_LOG(LogTemp , Display , TEXT("Light Ray Location X: %f   Y: %f") , Location.X , Location.Y);
	}

	else
	{
		UE_LOG(LogTemp , Display , TEXT("Dark Ray Location X: %f   Y: %f") , Location.X , Location.Y);
	}
}
