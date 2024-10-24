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

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Physics)
	bool isPhysicsObj = false;

	UPROPERTY(Replicated , VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	bool isSelected = false;
	UFUNCTION(Server , Reliable , WithValidation , Category = "Ability")
	void ServerSetIsSeleted(bool value);
	UFUNCTION(NetMulticast , Unreliable , Category = "Ability")
	void MulticastSetIsSeleted(bool value);

	virtual void Init();

	virtual void DrawOuline();
	virtual void EraseOuline();

	virtual void ChangeOutlineColor();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	class ACraftingStarCharacter* ControllingPlayer;

public:
	//telekinesis에 의해 움직여지고 있을 때 움직이고 있는 CraftingStarCharacter를 설정하는 함수.
	void SetTelekinesisOwner(class ACraftingStarCharacter* Player);

	UFUNCTION(Server, Unreliable)
	void ServerSetTelekinesisOwner(class ACraftingStarCharacter* Player);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastSetTelekinesisOwner(class ACraftingStarCharacter* Player);
};
