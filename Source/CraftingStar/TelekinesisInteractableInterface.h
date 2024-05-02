// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TelekinesisInteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTelekinesisInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRAFTINGSTAR_API ITelekinesisInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	class UStaticMeshComponent* StaticMesh;

	bool isPhysics = false;

	virtual void Init() = 0;

	virtual void DrawOuline() = 0;
	virtual void EraseOuline() = 0;

	virtual void ChangeOutlineColor() = 0;
};
