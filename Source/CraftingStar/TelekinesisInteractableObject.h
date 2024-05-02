// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TelekinesisInteractableObject.generated.h"

UCLASS()
class CRAFTINGSTAR_API ATelekinesisInteractableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATelekinesisInteractableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere , BlueprintReadWrite , Category = Mesh)
	class UStaticMeshComponent* ActorMesh;

	UPROPERTY(VisibleAnywhere , BlueprintReadWrite , Category = Physics)
	bool isPhysics = false;

	virtual void Init();

	virtual void DrawOuline();
	virtual void EraseOuline();

	virtual void ChangeOutlineColor();

};
