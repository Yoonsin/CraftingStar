// Fill out your copyright notice in the Description page of Project Settings.


#include "AssimilationComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "SplineChasingInterface.h"
#include "../Object/AssimilationTrigerComponent.h"

// Sets default values for this component's properties
UAssimilationComponent::UAssimilationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	HaloEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Assimilation Effect"));
	HaloEffect->SetupAttachment(this);

	// ...
}


// Called when the game starts
void UAssimilationComponent::BeginPlay()
{
	Super::BeginPlay();
	
	this->OnComponentBeginOverlap.AddDynamic(this, &UAssimilationComponent::DetectSpline);
	this->OnComponentEndOverlap.AddDynamic(this , &UAssimilationComponent::UndetectSpline);
}


// Called every frame
void UAssimilationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	

	// ...
}

void UAssimilationComponent::Assimilation()
{

	if ( GetOwner()->HasAuthority() )
	{
		MulticastAssimilation();
	}
	else
	{
		ServerAssimilation();
	}
}

void UAssimilationComponent::ServerAssimilation_Implementation()
{
	MulticastAssimilation();
}
void UAssimilationComponent::MulticastAssimilation_Implementation()
{
	UBoxComponent* ClosestTriger = nullptr;
	float ClosestDistance = MAX_FLT;

	for ( auto SplineTriger : SplineTrigers )
	{
		//현재 컴포넌트와 스플라인 트리거의 거리 측정
		float Distance = ( SplineTriger->GetComponentLocation() - this->GetComponentLocation() ).Size();

		if ( Distance < ClosestDistance )
		{
			ClosestTriger = SplineTriger;
			ClosestDistance = Distance;
		}
	}
	

	if ( ClosestTriger && ClosestTriger->Implements<USplineChasingInterface>() )
	{
		ISplineChasingInterface::Execute_ChaseStart(ClosestTriger , this);
	}

	// 현재까지의 가장 가까운 컴포넌트 갱신


}

void UAssimilationComponent::DetectSpline(UPrimitiveComponent* OverlappedComp , AActor* OtherActor ,
		UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
	if ( auto Triger = Cast<UAssimilationTrigerComponent>(OtherComp) )
	{
		UE_LOG(LogTemp , Display , TEXT("Detect Spline"));
		if ( Triger->Implements<USplineChasingInterface>() )
		{
			SplineTrigers.Add(Triger);
		}
	}
	/*
	if(OtherActor->Implements<USplineChasingInterface>())
	{
		
		//ISplineChasingInterface::Execute_ChaseStart(OtherActor, this);
		UE_LOG(LogTemp , Warning , TEXT("A Spline Collision Detected!"));
		Assimilation();
	}

	else if (auto a = Cast<ISplineChasingInterface>(OtherActor))
	{
		//a->ChaseStart();
		a->Execute_ChaseStart(OtherActor, this);
		
		// 스플라인이 따라가게 하고 싶어요
		UE_LOG(LogTemp , Warning , TEXT("B Spline Collision Detected!"));
	}
	*/
}

void UAssimilationComponent::UndetectSpline(UPrimitiveComponent* OverlappedComp , AActor* OtherActor ,
	UPrimitiveComponent* OtherComp , int32 OtherBodyIndex)
{
	if ( auto Triger = Cast<UBoxComponent>(OtherComp) )
	{
		UE_LOG(LogTemp , Display , TEXT("Undetect Spline"));
		if ( Triger->Implements<USplineChasingInterface>() )
		{
			SplineTrigers.Remove(Triger);
		}
	}
}

void UAssimilationComponent::HideOwner(bool bHide)
{
	if ( auto OwnerCharacter = Cast<ACharacter>(GetOwner()) )
	{
		OwnerCharacter->GetMesh()->SetHiddenInGame(bHide);
		TArray<USceneComponent*> AttachedComponents;
		OwnerCharacter->GetMesh()->GetChildrenComponents(true , AttachedComponents);
		for ( USceneComponent* AttachedComponent : AttachedComponents )
		{
			AttachedComponent->SetHiddenInGame(bHide);
		}
	}
}