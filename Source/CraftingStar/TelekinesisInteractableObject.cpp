// Fill out your copyright notice in the Description page of Project Settings.


#include "TelekinesisInteractableObject.h"
#include <Net/UnrealNetwork.h>
#include "CraftingStarCharacter.h"

// Sets default values
ATelekinesisInteractableObject::ATelekinesisInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this Component's Replication Property
	// SetReplicates() 에는 초기화 되기 이전(EX: Constructor)에 사용하지 않고, 객체가 만들어진 이후(EX: BeginPlay)에 사용하는 것이 바람직하다.
	//SetReplicates(true);
	bReplicates = true;

	// Set Rotation

	// Actor Mesh
	ActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ActorMesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> ActorSM(TEXT("StaticMesh'/Game/Assets/BaseContent/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if ( ActorSM.Succeeded() ) {
		ActorMesh->SetStaticMesh(ActorSM.Object);
		ActorMesh->SetIsReplicated(true);
	}
	SetRootComponent(ActorMesh);
}

void ATelekinesisInteractableObject::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATelekinesisInteractableObject , isSelected);
}

// Called when the game starts or when spawned
void ATelekinesisInteractableObject::BeginPlay()
{
	Super::BeginPlay();

	// Set Physics
	ActorMesh->SetSimulatePhysics(isPhysicsObj);
}

// Called every frame
void ATelekinesisInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Select Target
bool ATelekinesisInteractableObject::ServerSetIsSeleted_Validate(bool value) {
	return true;
}
void ATelekinesisInteractableObject::ServerSetIsSeleted_Implementation(bool value) {
	MulticastSetIsSeleted(value);
}
void ATelekinesisInteractableObject::MulticastSetIsSeleted_Implementation(bool value) {
	isSelected = value;
}

void ATelekinesisInteractableObject::Init() {
	
}

void ATelekinesisInteractableObject::DrawOuline() {

}

void ATelekinesisInteractableObject::EraseOuline() {

}

void ATelekinesisInteractableObject::ChangeOutlineColor() {

}

void ATelekinesisInteractableObject::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if ( IsValid(ControllingPlayer) )
	{
		//플레이어가 잡은 액터가 터졌을 경우
		//ControllingPlayer->DeactivateAbility(); 
	}
	
}

void ATelekinesisInteractableObject::SetTelekinesisOwner(ACraftingStarCharacter* Player)
{
	if ( HasAuthority() )
	{
		MulticastSetTelekinesisOwner(Player);
	}
	else
	{
		ServerSetTelekinesisOwner(Player);
	}
}

void ATelekinesisInteractableObject::ServerSetTelekinesisOwner_Implementation(ACraftingStarCharacter* Player)
{
	MulticastSetTelekinesisOwner(Player);
}

void ATelekinesisInteractableObject::MulticastSetTelekinesisOwner_Implementation(ACraftingStarCharacter* Player)
{
	ControllingPlayer = Player;
}