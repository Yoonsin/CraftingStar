// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MazePatrol.generated.h"

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
	FBlackboardKeySelector PartolNumber;
	UPROPERTY(EditAnywhere , Category = "Maze")
	FBlackboardKeySelector Destination;


	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory) override;

	
	
};
