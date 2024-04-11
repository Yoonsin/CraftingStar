// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeBoard.generated.h"


UCLASS()
class CRAFTINGSTAR_API AMazeBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazeBoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UFUNCTION(BlueprintCallable)
	class AMazeBlock* GetBlockByIndex(int32 index);

	UFUNCTION(BlueprintCallable)
	class AMazeBlock* GetBlockBy2D(int32 Row , int32 Col);

	UFUNCTION(BlueprintCallable)
	void SetOffset();

	FORCEINLINE FVector2D GetVector2DByIndex(int32 index) { return FVector2D(index / MazeCol , index % MazeCol); }
	FORCEINLINE int32 GetIndexBy2D(int32 Row , int32 Col) { return (Row * MazeCol + Col); }

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMazeBlock> DefaultBlock;

	UPROPERTY(EditAnywhere)
	FVector Offset = FVector(200.0f , 200.0f , 0.0f);

private:
	int32 MazeRow = 15;
	int32 MazeCol = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( AllowPrivateAccess = "true" ))
	TArray<UChildActorComponent*> MazeBlockComps;

	UPROPERTY()
	class USceneComponent* RootComp;
};
