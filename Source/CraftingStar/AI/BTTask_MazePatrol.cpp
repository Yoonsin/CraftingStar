// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MazePatrol.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Mob/FlameGuardian.h"

UBTTask_MazePatrol::UBTTask_MazePatrol()
{
	NodeName = "Maze Patrol";
	
}

EBTNodeResult::Type UBTTask_MazePatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    auto Owner = Cast<AFlameGuardian>(OwnerComp.GetOwner());
    if ( BlackboardComp && Owner )
    {
        
       // 현재 있는 밸류;
        int CurrentNumber = BlackboardComp->GetValueAsInt(Destination.SelectedKeyName);


        UE_LOG(LogTemp , Display , TEXT("%d"), CurrentNumber);
        // Perform your task logic here

        return EBTNodeResult::Succeeded; // Or return EBTNodeResult::Failed; based on task success or failure
    }
    return EBTNodeResult::Failed;
}