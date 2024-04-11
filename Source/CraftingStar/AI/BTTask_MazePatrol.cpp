// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MazePatrol.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Mob/MazePatroller.h"
#include "GameFramework/Controller.h"
#include "../Object/MazeBoard.h"
#include "../Object/MazeBlock.h"

UBTTask_MazePatrol::UBTTask_MazePatrol()
{
	NodeName = "Maze Patrol";
	
}

EBTNodeResult::Type UBTTask_MazePatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    //GetOwner로 AIController를 가져온디.
    auto OwnController = Cast<AController>(OwnerComp.GetOwner());
    if ( !OwnController )return EBTNodeResult::Failed;
    
    //GetPawn으로 AMazePatroller를 가져온다.
    auto Patroller = Cast<AMazePatroller>(OwnController->GetPawn());

    if ( BlackboardComp && Patroller)
    {

       // 현재 있는 밸류;
        int32 CurrentNumber = BlackboardComp->GetValueAsInt(PatrolNumber.SelectedKeyName);
        //갈 장소 GO GO GO, 블럭 위치
        FVector Gogogo = BlackboardComp->GetValueAsVector(Destination.SelectedKeyName);
        

        //여기부터 구현 부분.
        if ( auto Maze = Patroller->Maze )
        {
            //현재 있는 블록을 알아보자
            auto Block = Maze->GetBlockByIndex(CurrentNumber);

            //그렇다면 직진할 방향을 정할까요?
            EDirection dir = static_cast<EDirection>( FMath::RandRange(0 , 3) );
            
            //앞으로 갈 블록이 '널이 아니고' '땅이면' 새로운 블록을 찾아볼까요?
            for ( auto PreNumber = CurrentNumber; Block != nullptr && Block->BlockType == EMazeBlockType::MBT_Floor; )
            {
                //한 칸 나가기
                CurrentNumber = PreNumber;
                //현재 번호를 입력해 현재 위치 반환
                auto CurVector = Maze->GetVector2DByIndex(CurrentNumber);

                //방향 연산 하고
                switch ( dir )
                {
                case North:
                    CurVector.Y++;
                    break;
                case South:
                    CurVector.Y--;
                    break;
                case East:
                    CurVector.X++;
                    break;
                case West:
                    CurVector.X--;
                    break;
                default:
                    break;
                }
                //이제 앞으로 나갈 곳을 연산
                PreNumber = Maze->GetIndexBy2D(CurVector.X , CurVector.Y);

                //앞으로 갈 블록을 저장
                Block = Maze->GetBlockByIndex(PreNumber);
                
            }

            //현재 블록 값을 셋팅
            Block = Maze->GetBlockByIndex(CurrentNumber);
            if ( Block )
            {
                Gogogo = Block->GetActorLocation();
                BlackboardComp->SetValueAsVector(PatrolNumber.SelectedKeyName , Gogogo);
            }

        }
        BlackboardComp->SetValueAsInt(PatrolNumber.SelectedKeyName , CurrentNumber);
        BlackboardComp->SetValueAsVector(Destination.SelectedKeyName , Gogogo);
        return EBTNodeResult::Succeeded; // Or return EBTNodeResult::Failed; based on task success or failure


    }
    return EBTNodeResult::Failed;
}