// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeBoard.h"
#include "Components/ChildActorComponent.h"
#include "MazeBlock.h"
#include "EngineUtils.h"

// Sets default values
AMazeBoard::AMazeBoard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	RootComponent = RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Componnet"));

	//Maze 생성
	int count = 0;
	for ( size_t Row = 0; Row < MazeRow; Row++ )
	{
		for ( size_t Col = 0; Col < MazeCol; Col++ )
		{

            //UChildActorComponent를 등록합니다.
			auto MazeBlockComp = CreateDefaultSubobject<UChildActorComponent>(*FString::Printf(TEXT("MazeBlock %d") , ++count));
			MazeBlockComp->SetupAttachment(RootComponent);
            MazeBlockComp->AttachToComponent(RootComponent , FAttachmentTransformRules::KeepRelativeTransform);
			MazeBlockComp->SetChildActorClass(DefaultBlock);

            //UChildActorComponent의 위치를 지정합니다.
            FVector NewLocation = FVector(Col * Offset.X , Row * Offset.Y , 0.0f);
            MazeBlockComp->SetRelativeLocation(NewLocation);
            MazeBlockComp->SetRelativeLocation(FVector::ZeroVector);

            //UChildActorComponent를 배열에 추가합니다.
            MazeBlockComps.Add(MazeBlockComp);
		}
	}



}

// Called when the game starts or when spawned
void AMazeBoard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMazeBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AMazeBlock* AMazeBoard::GetBlockByIndex(int32 index)
{
    // 인덱스가 유효한지 확인합니다.
    if ( index < 0 || index >= MazeBlockComps.Num() )
    {
        // 인덱스가 유효하지 않으면 nullptr을 반환합니다.
        return nullptr;
    }

    // MazeBlockComps에서 UChildActorComponent를 가져옵니다.
    UChildActorComponent* MazeBlockComp = MazeBlockComps[index];
    if ( !MazeBlockComp )
    {
        // MazeBlockComp이 유효하지 않으면 nullptr을 반환합니다.
        return nullptr;
    }

    // UChildActorComponent에서 액터를 가져옵니다.
    AActor* ChildActor = MazeBlockComp->GetChildActor();
    if ( !ChildActor )
    {
        // ChildActor가 유효하지 않으면 nullptr을 반환합니다.
        return nullptr;
    }

    // 가져온 액터를 AMazeBlock으로 캐스팅합니다.
    AMazeBlock* MazeBlock = Cast<AMazeBlock>(ChildActor);
    if ( !MazeBlock )
    {
        // MazeBlock으로의 캐스팅이 실패하면 nullptr을 반환합니다.
        return nullptr;
    }

    // 유효한 AMazeBlock 포인터를 반환합니다.
    return MazeBlock;
}

AMazeBlock* AMazeBoard::GetBlockBy2D(int32 Row , int32 Col)
{
    // 1차원 배열에서의 인덱스 계산
    int32 index = Row * MazeCol + Col;

    // 계산된 인덱스가 배열 범위를 벗어나지 않도록 보장
    if ( index < 0 || index >= MazeBlockComps.Num() )
    {
        // 인덱스가 유효하지 않으면 nullptr을 반환합니다.
        return nullptr;
    }
    
    UChildActorComponent* MazeBlockComp = MazeBlockComps[index];
    if ( !MazeBlockComp )
    {
        // MazeBlockComp이 유효하지 않으면 nullptr을 반환합니다.
        return nullptr;
    }

    // UChildActorComponent에서 액터를 가져옵니다.
    AActor* ChildActor = MazeBlockComp->GetChildActor();
    if ( !ChildActor )
    {
        // ChildActor가 유효하지 않으면 nullptr을 반환합니다.
        return nullptr;
    }

    // 가져온 액터를 AMazeBlock으로 캐스팅합니다.
    AMazeBlock* MazeBlock = Cast<AMazeBlock>(ChildActor);
    if ( !MazeBlock )
    {
        // MazeBlock으로의 캐스팅이 실패하면 nullptr을 반환합니다.
        return nullptr;
    }

    // 유효한 AMazeBlock 포인터를 반환합니다.
    return MazeBlock;
}

void AMazeBoard::SetOffset()
{
    int count = 0;

    // MazeBlockComps 배열의 이터레이터 생성
    for ( auto It = MazeBlockComps.CreateIterator(); It; ++It )
    {
        UChildActorComponent* var = *It; // 이터레이터를 사용하여 요소에 접근

        int32 Row = It.GetIndex() / MazeCol;
        int32 Col = It.GetIndex() % MazeCol;

        FVector NewLocation = FVector(Col * Offset.X , Row * Offset.Y , 0.0f);
        // 누적된 상대 위치를 설정
        var->SetRelativeLocation(NewLocation);
    }
}