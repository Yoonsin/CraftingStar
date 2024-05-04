// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MazePatrol.generated.h"


enum EDirection : int32
{
	North = 0,
	South = 1,
	East = 2,
	West= 3
};

/**
 * 
 */
UCLASS()
class CRAFTINGSTAR_API UBTTask_MazePatrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	UBTTask_MazePatrol();

public:
	UPROPERTY(EditAnywhere , Category = "Maze")
	FBlackboardKeySelector PatrolNumber;
	UPROPERTY(EditAnywhere , Category = "Maze")
	FBlackboardKeySelector Destination;


	TArray<TArray<TCHAR>> Board;


	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory) override;

	
	
};
