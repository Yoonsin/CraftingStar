// Fill out your copyright notice in the Description page of Project Settings.


#include "AssimilationComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "SplineChasingInterface.h"
#include "../Object/AssimilationTrigerComponent.h"
#include "../CraftingStarPS.h"

// Sets default values for this component's properties
UAssimilationComponent::UAssimilationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	HaloEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Assimilation Effect"));
	HaloEffect->SetupAttachment(this);

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> HaloAsset(TEXT("NiagaraSystem'/Game/CraftingStarCharacter/Abilities/NS_Assimilate.NS_Assimilate'"));
	if ( HaloAsset.Succeeded() ) 
	{
		HaloEffect->SetAsset(HaloAsset.Object);
	}

	HaloEffect->SetVisibility(false);
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
		if ( auto OwnerCharacter = Cast<ACharacter>(GetOwner()) )
		{
			if ( auto State = Cast<ACraftingStarPS>(OwnerCharacter->GetPlayerState()) )
			{
				ISplineChasingInterface::Execute_ChaseStart(ClosestTriger , this , State->PlayerData.Mode == EPlayerRole::ELight);
			}
		}
		
	}

	// 현재까지의 가장 가까운 컴포넌트 갱신


}

void UAssimilationComponent::DetectSpline(UPrimitiveComponent* OverlappedComp , AActor* OtherActor ,
		UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
	if ( auto Triger = Cast<UAssimilationTrigerComponent>(OtherComp) )
	{
		Triger->SetAssimilationComponent(this);
		UE_LOG(LogTemp , Display , TEXT("Detect Spline"));
		if ( Triger->Implements<USplineChasingInterface>() )
		{
			SplineTrigers.Add(Triger);
		}
	}
	
}

void UAssimilationComponent::UndetectSpline(UPrimitiveComponent* OverlappedComp , AActor* OtherActor ,
	UPrimitiveComponent* OtherComp , int32 OtherBodyIndex)
{
	if ( auto Triger = Cast<UAssimilationTrigerComponent>(OtherComp) )
	{
		//Triger->SetAssimilationComponent(nullptr);
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

void UAssimilationComponent::ShowHaloEffect(bool bShow)
{
	if ( HaloEffect )
	{
		if ( auto OwnerCharacter = Cast<ACharacter>(GetOwner()) )
		{
			HaloEffect->SetVisibility(bShow);
			
			if ( auto State = Cast<ACraftingStarPS>(OwnerCharacter->GetPlayerState())) 
			{
				HaloEffect->SetNiagaraVariableLinearColor(FString("Color") , 
					EPlayerRole::ELight == State->PlayerData.Mode ? FLinearColor::White : FLinearColor::Black);
			}
		}

		
			
	}

	else UE_LOG(LogTemp , Warning , TEXT("레이저가 안보이는건 HaloEffcet가 Null이라는건데이게 어떻게 가능하지?"));
}