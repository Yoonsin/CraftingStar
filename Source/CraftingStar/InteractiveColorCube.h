// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveColorCube.generated.h"

UCLASS()
class CRAFTINGSTAR_API AInteractiveColorCube : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* mesh;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* box;
	
public:	
	// Sets default values for this actor's properties
	AInteractiveColorCube();
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = Manipulation)
	bool canBeManipulated = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UStaticMeshComponent* CubeMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Perform(); //오브젝트가 작동하기 위한 내부구현
	virtual void Perform_Implementation();

	int InteractiveFunc();
};
