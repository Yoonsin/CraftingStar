// Copyright Epic Games, Inc. All Rights Reserved.

#include "CraftingStarCharacter.h"
#include "CraftingStarPC.h"
#include "CraftingStarPS.h"
#include "CraftingStarGS.h"
#include "CustomEnum.h"
#include "UtilityFunction.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "CraftingStarGameMode.h"
#include "DrawDebugHelpers.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h" 
#include "NiagaraComponent.h"
#include "UtilityFunction.h"
#include "LaserNiagaraComponent.h"
#include "TelekinesisInteractableObject.h"
#include "Ability/AssimilationComponent.h"
#include "Ability/WeaponComponent.h"
#include "Ability/BowComponent.h"
#include "Object/LightSensingObject.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Misc/OutputDeviceNull.h"

//////////////////////////////////////////////////////////////////////////
// ACraftingStarCharacter

ACraftingStarCharacter::ACraftingStarCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f , 75.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f , 540.0f , 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> BodySM(TEXT("SkeletalMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/BodyPart/Body10_SK.Body10_SK'"));
	if ( BodySM.Succeeded() ) {
		GetMesh()->SetSkeletalMesh(BodySM.Object);
	}

	// Character Mesh
	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> HeadSM(TEXT("StaticMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/HeadPart/Head02_Female_SM.Head02_Female_SM'"));
	if ( HeadSM.Succeeded() ) {
		HeadMesh->SetStaticMesh(HeadSM.Object);
	}
	HeadMesh->SetupAttachment(GetMesh() , FName(TEXT("Head")));
	HeadMesh->SetRelativeLocation(FVector(0.0f , 0.0f , 0.0f));
	HeadMesh->SetRelativeRotation(FRotator(-90.0f , 0.0f , 0.0f));
	HeadMesh->SetRelativeLocation(FVector(0.0f , 0.0f , 0.0f));
	HeadMesh->SetRelativeRotation(FRotator(-90.0f , 0.0f , 0.0f));

	HairAndHatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HairAndHatMesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> HairAndHatSM(TEXT("StaticMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/HeadPart/Hair08_SM.Hair08_SM'"));
	if ( HairAndHatSM.Succeeded() ) {
		HairAndHatMesh->SetStaticMesh(HairAndHatSM.Object);
	}
	HairAndHatMesh->SetupAttachment(GetMesh() , FName(TEXT("Head")));
	HairAndHatMesh->SetRelativeLocation(FVector(0.0f , 0.0f , 0.0f));
	HairAndHatMesh->SetRelativeRotation(FRotator(-90.0f , 0.0f , 0.0f));

	EyesMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Eyes"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> EyesSM(TEXT("StaticMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/HeadPart/Eye08_SM.Eye08_SM'"));
	if ( EyesSM.Succeeded() ) {
		EyesMesh->SetStaticMesh(EyesSM.Object);
	}
	EyesMesh->SetupAttachment(GetMesh() , FName(TEXT("Head")));
	EyesMesh->SetRelativeLocation(FVector(0.0f , 0.0f , 0.0f));
	EyesMesh->SetRelativeRotation(FRotator(-90.0f , 0.0f , 0.0f));

	MouthMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mouth"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MouthSM(TEXT("StaticMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/HeadPart/Mouth02_SM.Mouth02_SM'"));
	if ( MouthSM.Succeeded() ) {
		MouthMesh->SetStaticMesh(MouthSM.Object);
	}
	MouthMesh->SetupAttachment(GetMesh() , FName(TEXT("Head")));
	MouthMesh->SetRelativeLocation(FVector(0.0f , 0.0f , 0.0f));
	MouthMesh->SetRelativeRotation(FRotator(-90.0f , 0.0f , 0.0f));

	CloakMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Cloak"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> CloakSM(TEXT("SkeletalMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/BodyPart/Cloak02_SK.Cloak02_SK'"));
	if ( CloakSM.Succeeded() ) {
		CloakMesh->SetSkeletalMesh(CloakSM.Object);
	}
	CloakMesh->SetupAttachment(GetMesh() , FName(TEXT("CloakBone02")));
	CloakMesh->SetRelativeLocation(FVector(40.0f , 20.0f , 0.0f));
	CloakMesh->SetRelativeRotation(FRotator(90.0f , 0.0f , 0.0f));
	
	Weapon_rMesh = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon_R"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> WandSM(TEXT("StaticMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/Weapon/Wand04_SM.Wand04_SM'"));
	if ( WandSM.Succeeded() ) {
		Weapon_rMesh->SetStaticMesh(WandSM.Object);
		Weapon_rMesh->SetWandWeapon(WandSM.Object);
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> TwoHandedWeaponSM(TEXT("StaticMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/Weapon/THS02_Sword_SM.THS02_Sword_SM'"));
	if ( TwoHandedWeaponSM.Succeeded() )
	{
		Weapon_rMesh->SetTwoHandedWeapon(TwoHandedWeaponSM.Object);
	}
	
	Weapon_rMesh->SetupAttachment(GetMesh() , FName(TEXT("Weapon_R")));
	Weapon_rMesh->SetRelativeLocation(FVector(0.0f , 0.0f , 0.0f));
	Weapon_rMesh->SetRelativeRotation(FRotator(0.0f , 0.0f , 0.0f));
	
	// Magic Wand LineTrace Start Point
	SpawnLocSource = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnLoc Source"));
	SpawnLocSource->SetupAttachment(Weapon_rMesh , FName(TEXT("SpawnLoc")));
	SpawnLocSource->SetIsReplicated(true);
	
	Bow_lMesh = CreateDefaultSubobject<UBowComponent>(TEXT("Bow Projection"));
	Bow_lMesh->SetupAttachment(GetMesh() , FName(TEXT("Weapon_L")));
	Bow_lMesh->SetRelativeRotation(FRotator(90.0f , 0.f , 90.0f));
	Bow_lMesh->SetArcher(this);
	Bow_lMesh->Unequip();
	Bow_lMesh->SetWandComponent(Weapon_rMesh);

	Comp_LaserNiagara = CreateDefaultSubobject<ULaserNiagaraComponent>(TEXT("Laser Niagara System"));
	Comp_LaserNiagara->SetupAttachment(SpawnLocSource);
	Comp_LaserNiagara->SetIsReplicated(true);

	




	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom , USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	AssimilationComponent = CreateDefaultSubobject<UAssimilationComponent>(TEXT("Abiility "));
	AssimilationComponent->SetupAttachment(RootComponent);
	AssimilationComponent->SetCapsuleSize(100 , 100);
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	PaletteWidgetRef = NULL;
	PaletteCnt = 0.0f;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

//////////////////////////////////////////////////////////////////////////
// Input
// Called when the game starts or when spawned
void ACraftingStarCharacter::BeginPlay()
{
	Super::BeginPlay();


	//if (LoadingWidgetRef == nullptr && LoadingWidget)
	//{
	//	//LoadingWB
	//	LoadingWidgetRef = CreateWidget(GetWorld() , LoadingWidget);
	//	LoadingWidgetRef->AddToViewport();
	//	//idx++;
	//}


	if ( HasAuthority() ) {
		GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("LOAD Server"));
		LoadSaveData(true);
		
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("LOAD Client"));
		//ServerRequestLoadSaveData();
		LoadSaveData(false);
	}

	

	Comp_LaserNiagara->Hide();
}

void ACraftingStarCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump" , IE_Pressed , this , &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump" , IE_Released , this , &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward" , this , &ACraftingStarCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight" , this , &ACraftingStarCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn" , this , &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate" , this , &ACraftingStarCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp" , this , &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate" , this , &ACraftingStarCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed , this , &ACraftingStarCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released , this , &ACraftingStarCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR" , IE_Pressed , this , &ACraftingStarCharacter::OnResetVR);

	// 팔레트
	PlayerInputComponent->BindAction("Palette" , IE_Pressed , this , &ACraftingStarCharacter::Palette);
	PlayerInputComponent->BindAction("Palette" , IE_Released , this , &ACraftingStarCharacter::StopPalette);
	//월드맵
	PlayerInputComponent->BindAction("WorldMap" , IE_Pressed , this , &ACraftingStarCharacter::WorldMap);
	PlayerInputComponent->BindAction("WorldMap" , IE_Released , this , &ACraftingStarCharacter::StopWorldMap);

	PlayerInputComponent->BindAction("SystemMenu" , IE_Pressed , this , &ACraftingStarCharacter::SystemMenu);
	PlayerInputComponent->BindAction("SystemMenu" , IE_Released , this , &ACraftingStarCharacter::StopSystemMenu);

	//상호작용
	PlayerInputComponent->BindAction("Interaction" , IE_Pressed , this , &ACraftingStarCharacter::Interaction);

	// for using Ability. key: E.
	PlayerInputComponent->BindAction("Ability" , IE_Pressed , this , &ACraftingStarCharacter::ActivateAbility);
	PlayerInputComponent->BindAction("Ability" , IE_Released , this , &ACraftingStarCharacter::DeactivateAbility);

	PlayerInputComponent->BindAction("Ability2" , IE_Pressed , this , &ACraftingStarCharacter::ActivateAbility2);

	// mouse left click
	PlayerInputComponent->BindAction("Click" , IE_Pressed , this , &ACraftingStarCharacter::MouseLeftPressed);
	PlayerInputComponent->BindAction("Click" , IE_Released , this , &ACraftingStarCharacter::MouseLeftReleased);
}


void ACraftingStarCharacter::Tick(float DeltaTime)
{
	//          Ӹ          ɷ              ʿ        Ʈ
	Super::Tick(DeltaTime);

	if ( KeepAbility  ) {
		// Activate Ability
		if ( nowAbility != EPlayerAbility::ENone ) {
			if ( nowAbility == EPlayerAbility::EBlast ) {
				// Activate Laser
				WandLineTrace(10000.0f);

			}
			else if ( nowAbility == EPlayerAbility::ETelekinesis ) {
				Telekinesis();
			}
		}
	}

}

void ACraftingStarCharacter::UpdatePlayerAbility(EPlayerAbility playerAbility) {
	auto playerState = Cast<ACraftingStarPS>(GetPlayerState());

	if ( playerState != nullptr ) {
		playerState->NowAbility = playerAbility;
		playerState->RequestPlayerAbility(playerAbility);
	}

	// cancel previous skill if it is running
	if ( nowAbility == EPlayerAbility::EBlast ) {
		if ( KeepAbility ) {
			DeactivateAbility();
			KeepAbility = false;
		}
	}
	else if ( nowAbility == EPlayerAbility::ETelekinesis ) {
		GEngine->AddOnScreenDebugMessage(-1 , 3 , FColor::Red , FString::Printf(TEXT("Try To Cancel tele")));
		if ( abilityReadyStatus ) {
			MouseLeftReleased();
			ActivateAbility();
			KeepAbility = false;
			abilityReadyStatus = false;
		}
	}

	// update now ability
	nowAbility = playerAbility;

}

void ACraftingStarCharacter::UpdatePlayerGMState(EPlayerGMState playerGMState) {

	auto playerState = Cast<ACraftingStarPS>(GetPlayerState());

	if ( playerState != nullptr ) {
		playerState->NowState = playerGMState;
		playerState->RequestPlayerGMState(playerGMState);
	}


}


void ACraftingStarCharacter::Palette() {
	//Ÿ ̸       (0.1 ʴ  1ȸ    Լ  ȣ  )
	GetWorldTimerManager().SetTimer(HoldTimerHandle , this , &ACraftingStarCharacter::RepeatingFunction , 0.01f , true);
}

void ACraftingStarCharacter::StopPalette() {
	//          Ÿ ̸Ӱ    ȿ ϸ  Ÿ ̸      
	if ( GetWorldTimerManager().IsTimerActive(HoldTimerHandle) ) {
		GetWorldTimerManager().ClearTimer(HoldTimerHandle);
		PaletteCnt = 0.0f;
	}


	//      ɷ   غ    ȷ Ʈ             ʾ     
	if ( PaletteWidgetRef == NULL && GetPlayerState() != nullptr && Cast<ACraftingStarPS>(GetPlayerState())->NowState == EPlayerGMState::EAbilityReady ) {
		// ⺻   ·     ư     ٷ      
		auto controller = GetController();
		UpdatePlayerGMState(EPlayerGMState::EIdle);
		//Cast<ACraftingStarPS>(GetPlayerState())->NowState = EPlayerGMState::EIdle;

		return;
	}

	//      ɷ   غ       ȯ
	if ( GetPlayerState() != nullptr && Cast<ACraftingStarPS>(GetPlayerState())->NowAbility != EPlayerAbility::ENone ) UpdatePlayerGMState(EPlayerGMState::EAbilityReady);

	//      ȷ Ʈ UI        ƴٸ  
	if ( PaletteWidgetRef != NULL ) {
		// ȷ Ʈ     
		PaletteWidgetRef->RemoveFromParent();
		PaletteWidgetRef = NULL;

		//Ű      Է       &    콺          Ȱ  ȭ
		SetPause(false);
	}
}

void ACraftingStarCharacter::RepeatingFunction() {

	//UE_LOG(LogTemp, Log, TEXT("GetTimeElapsed : %f"), PaletteCnt);
	if ( PaletteCnt >= 0.05f ) {
		//0.2    ̻  Ȧ   ϸ   ȷ Ʈ     
		PaletteCnt = 0.0f;
		GetWorldTimerManager().ClearTimer(HoldTimerHandle);

		if ( PaletteWidgetRef == NULL ) {
			PaletteWidgetRef = CreateWidget(GetWorld() , PaletteWidget);
			PaletteWidgetRef->AddToViewport();
		}

		//Ű      Է       &    콺        Ȱ  ȭ
		SetPause(true);
		return;
	}
	PaletteCnt += 0.01f;
}

void ACraftingStarCharacter::LogoutClient() {
	if ( LogOutClientWidget != NULL ) {

		LogOutClientWidgetRef = CreateWidget(GetWorld() , LogOutClientWidget);
		if ( LogOutClientWidgetRef == nullptr ) {
			GEngine->AddOnScreenDebugMessage(-1 , 3 , FColor::Red , FString::Printf(TEXT("PTR nullptr")));
		}
		else {

			GEngine->AddOnScreenDebugMessage(-1 , 3 , FColor::Red , FString::Printf(TEXT("PTR ptr")));
			LogOutClientWidgetRef->AddToViewport();
			SetPause(true);
		}

	
	}

}

void  ACraftingStarCharacter::StopLogoutClient() {
	if ( LogOutClientWidgetRef != NULL ) {
		// ȷ Ʈ     
		LogOutClientWidgetRef->RemoveFromParent();
		LogOutClientWidgetRef = NULL;
		SetPause(false);
	}

}

void ACraftingStarCharacter::ServerStopLoadingWidget_Implementation()
{
	MulticastStopLoadingWidget();
	//GEngine->AddOnScreenDebugMessage(-1 , 3 , FColor::Red , FString::Printf(TEXT("RemoveWidget_Server")));
}

void ACraftingStarCharacter::MulticastStopLoadingWidget_Implementation()
{

    //GEngine->AddOnScreenDebugMessage(-1 , 3 , FColor::Red , FString::Printf(TEXT("RemoveWidget_MULTICAST")));

	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());

	//GameWB     
	CreateWidget(GetWorld() , GameWidget)->AddToViewport();
}



void ACraftingStarCharacter::WorldMap() {
	if ( WorldMapWidgetRef == NULL ) {
		WorldMapWidgetRef = CreateWidget(GetWorld() , WorldMapWidget);
		WorldMapWidgetRef->AddToViewport();
		SetPause(true);
	}

}

void ACraftingStarCharacter::StopWorldMap() {
	//      ȷ Ʈ UI        ƴٸ  
	if ( WorldMapWidgetRef != NULL ) {
		// ȷ Ʈ     
		WorldMapWidgetRef->RemoveFromParent();
		WorldMapWidgetRef = NULL;
		SetPause(false);
	}
}

void ACraftingStarCharacter::SystemMenu() {
	if ( SystemMenuWidgetRef == NULL ) {
		SystemMenuWidgetRef = CreateWidget(GetWorld() , SystemMenuWidget);
		SystemMenuWidgetRef->AddToViewport();
		SetPause(true);
	}
}

void ACraftingStarCharacter::StopSystemMenu() {
	if ( SystemMenuWidgetRef != NULL ) {
		// ȷ Ʈ     
		SystemMenuWidgetRef->RemoveFromParent();
		SystemMenuWidgetRef = NULL;
		SetPause(false);
	}
}


void ACraftingStarCharacter::Interaction() {

}

// Magic Wand Line Trace for Ability
bool ACraftingStarCharacter::WandLineTrace(float distance) {

	/* Set LineTrace */

	// Result oof LineTrace
	FHitResult Hit;

	// Ability Spawn Loc Socket Transform
	//FVector SpawnLocation = this->Weapon_rMesh->GetSocketLocation(FName("SpawnLoc"));
	FVector SpawnLocation = this->SpawnLocSource->GetComponentLocation();
	//FVector SpawnLocation = FVector::UpVector;
	
	// Start point and End point of LineTrace
	FVector Start = SpawnLocation;
	//FVector End = SpawnLocation + ( GetActorForwardVector() * distance );
	FVector End = SpawnLocation + ( FollowCamera->GetForwardVector() * distance );

	// Trace Channel: Custom Trace Channel - AbilitySpawn
	ECollisionChannel Channel = ECollisionChannel::ECC_GameTraceChannel1;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	/* Execute LineTrace */
	GetWorld()->LineTraceSingleByChannel(Hit , Start , End , Channel , QueryParams);
	// Visualize LineTrace
	//DrawDebugLine(GetWorld() , Start , End , FColor::Green);

	Comp_LaserNiagara->SetLaser(Hit , End);

	LightAct(Hit.Actor.Get() , Hit.Location);

	return !Hit.bBlockingHit;
	
}

// Ray for Telekinesis
void ACraftingStarCharacter::Telekinesis() {
	/* Set LineTrace */

	// Result oof LineTrace
	FHitResult Hit;

	// Ability Spawn Loc Socket Transform
	FVector SpawnLocation = FollowCamera->GetComponentLocation();
	// Start point and End point of LineTrace
	FVector Start = SpawnLocation;
	FVector End = SpawnLocation + ( FollowCamera->GetForwardVector() * 750 );

	// Trace Channel: Custom Trace Channel - AbilitySpawn
	ECollisionChannel Channel = ECollisionChannel::ECC_GameTraceChannel1;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	/* Execute LineTrace */
	GetWorld()->LineTraceSingleByChannel(Hit , Start , End , Channel , QueryParams);

	// Visualize LineTrace
	//DrawDebugLine(GetWorld() , Start , End , FColor::Green);

	// Draw Laser
	//GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("x : %f  y : %f  z : %f ") , End.X , End.Y , End.Z));
	Comp_LaserNiagara->SetLaser(Hit , End);

	if ( Hit.bBlockingHit ) {

		if ( selectedTarget == NULL ) {

			// Grab selectedTarget Component
			switch ( HasAuthority() ) {
			case true:
				selectedTarget = Hit.GetComponent();
				PhysicsHandle->GrabComponent(selectedTarget , NAME_None , End , true);
				if ( selectedTarget ) {
					// Check is the simulate physics true
					if ( !selectedTarget->IsSimulatingPhysics() ) {
						selectedTarget->SetSimulatePhysics(true);
					}
					else {
					}
				}
				break;
			case false:
				ServerSelectTarget(Hit);
				ServerGrabComponent(End);
				break;
			}
		}
	}

	if ( selectedTarget != NULL ) {
		//GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("selected object distance: %f") , Hit.Distance));

		if ( selectedTarget->GetOwner() ) {
			// Move target if it can be cast
			// Change Tele' Interactive Actor's Outline Color
			if ( Cast<ATelekinesisInteractableObject>(selectedTarget->GetOwner()) )
			{
			// Move selectedTarget Component
				switch ( HasAuthority() ) {
				case true:
					PhysicsHandle->SetTargetLocation(End);
					break;
				case false:
					ServerTeleObjLoc(End);
					break;
				}
				// Set CustomDepth Stencil Value to chagne Color
				Cast<ATelekinesisInteractableObject>(selectedTarget->GetOwner())->ActorMesh->SetCustomDepthStencilValue(1);
			}
		}
	}
}

// Select Target
bool ACraftingStarCharacter::ServerSelectTarget_Validate(FHitResult Hit) {
	return true;
}
void ACraftingStarCharacter::ServerSelectTarget_Implementation(FHitResult Hit) {
	MulticastSelectTarget(Hit);
}
void ACraftingStarCharacter::MulticastSelectTarget_Implementation(FHitResult Hit) {
	GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("select target")));
	selectedTarget = Hit.GetComponent();
}

// Deselect Target
bool ACraftingStarCharacter::ServerDeselectTarget_Validate() {
	return true;
}
void ACraftingStarCharacter::ServerDeselectTarget_Implementation() {
	MulticastDeselectTarget();
}
void ACraftingStarCharacter::MulticastDeselectTarget_Implementation() {
	if ( selectedTarget != NULL ) {
		GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("deselect target")));
		selectedTarget = NULL;
	}
}

// Grab Component on Server
bool ACraftingStarCharacter::ServerGrabComponent_Validate(FVector End) {
	return true;
}
void ACraftingStarCharacter::ServerGrabComponent_Implementation(FVector End) {
	MulticastGrabComponent(End);
}
void ACraftingStarCharacter::MulticastGrabComponent_Implementation(FVector End) {
	PhysicsHandle->GrabComponent(selectedTarget , NAME_None , End , true);
	GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("Grab Component"));

	if ( selectedTarget ) {
		// Check is the simulate physics true
		if ( !selectedTarget->IsSimulatingPhysics() ) {
			selectedTarget->SetSimulatePhysics(true);
			GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("Set Physics true"));
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("Origin Physics true"));
		}
	}
}

// Set and Replicate TelekinesisInteractableObject's Location
bool ACraftingStarCharacter::ServerTeleObjLoc_Validate(FVector End) {
	return true;
}
void ACraftingStarCharacter::ServerTeleObjLoc_Implementation(FVector End) {
	MulticastTeleObjLoc(End);
}
void ACraftingStarCharacter::MulticastTeleObjLoc_Implementation(FVector End) {
	//GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("teleObj RPC")));
	//GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("x : %f  y : %f  z : %f ") , End.X , End.Y , End.Z));
	
	PhysicsHandle->SetTargetLocation(End);
}

// Ability Animaition Replicate
bool ACraftingStarCharacter::ServerAbility_Validate(bool abilityState) {
	return true;
}
void ACraftingStarCharacter::ServerAbility_Implementation(bool abilityState) {
	MulticastAbility(abilityState);
}
void ACraftingStarCharacter::MulticastAbility_Implementation(bool abilityState) {
	if ( abilityState ) {
		GetMesh()->GetAnimInstance()->Montage_Play(AbilityMontage , 1.0f);
		bUseControllerRotationYaw = true;	// Rotate the player based on the controller
	}
	else {
		// Deactivate Ability
		bUseControllerRotationYaw = false;	// Not Rotate the player based on the controller
		GetMesh()->GetAnimInstance()->Montage_Play(DeactiveAbilityMontage , 1.0f);
		// Hide Laser
		Comp_LaserNiagara->Hide();
	}
}

void ACraftingStarCharacter::CreateTeleObjOutline() {
	// Create Tele' Interactable Actor's Outline
	// activate the outline of objects that can be interacted with telekinesis skill
	TArray<AActor*> TeleActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld() , ATelekinesisInteractableObject::StaticClass() , TeleActors);
	for ( int i = 0; i < TeleActors.Num(); ++i )
	{
		Cast<ATelekinesisInteractableObject>(TeleActors[i])->ActorMesh->SetRenderCustomDepth(true);
		Cast<ATelekinesisInteractableObject>(TeleActors[i])->ActorMesh->SetCustomDepthStencilValue(0);
	}
}
void ACraftingStarCharacter::RemoveTeleObjOutline() {
	// Remove Tele' Interactable Actor's Outline
	// deactivate the outline of objects that can be interacted with telekinesis skill
	TArray<AActor*> TeleActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld() , ATelekinesisInteractableObject::StaticClass() , TeleActors);
	for ( int i = 0; i < TeleActors.Num(); ++i )
	{
		Cast<ATelekinesisInteractableObject>(TeleActors[i])->ActorMesh->SetCustomDepthStencilValue(0);
		Cast< ATelekinesisInteractableObject>(TeleActors[i])->ActorMesh->SetRenderCustomDepth(false);
	}

}

// Input Ability (Key: E)
void ACraftingStarCharacter::ActivateAbility() {
	if ( nowAbility == EPlayerAbility::EBlast ) {
		KeepAbility = true;
		CameraBoom->SetRelativeLocation(FVector(0.0f , 100.0f , 100.0f));
		//CameraBoom->bUsePawnControlRotation = false; // Does not rotate the arm based on the controller
		if ( AbilityMontage ) {
			// Play Animation
			bool bIsMontagePlaying = GetMesh()->GetAnimInstance()->Montage_IsPlaying(AbilityMontage);
			if ( !bIsMontagePlaying ) {
				ServerAbility(true);	// request ability animation on server
			}
		}
	}
	else if ( nowAbility == EPlayerAbility::ETelekinesis ) {
		GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("Tele"));
		if ( !abilityReadyStatus ) {
			GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("Tele ready"));
			abilityReadyStatus = true;

			CameraBoom->SetRelativeLocation(FVector(0.0f , 100.0f , 100.0f));
			//CameraBoom->bUsePawnControlRotation = false; // Does not rotate the arm based on the controller

			//GetCharacterMovement()->bOrientRotationToMovement = false; // Character doesn't moves in the direction of input...
			
			CreateTeleObjOutline();
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("Cancel"));
			CameraBoom->SetRelativeLocation(FVector(0.0f , 0.0f , 0.0f));
			//CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

			//GetCharacterMovement()->bOrientRotationToMovement = false; // Character doesn't moves in the direction of input...

			switch ( HasAuthority() ) {
			case true :
				if ( selectedTarget != NULL ) {
					selectedTarget = NULL;
				}
			case false :
				ServerDeselectTarget();
			}
			RemoveTeleObjOutline();

			abilityReadyStatus = false;
		}
	}
	else if ( nowAbility == EPlayerAbility::EAbility_dummy1 )
	{
		UseProjectionTwoHanded();
	}

	else if ( nowAbility == EPlayerAbility::EAbility_dummy2 )
	{
		AssimilationComponent->Assimilation();
	}
}
void ACraftingStarCharacter::DeactivateAbility() {
	if ( nowAbility == EPlayerAbility::EBlast ) {
		GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("Blast deactivate"));
		KeepAbility = false;

		CameraBoom->SetRelativeLocation(FVector(0.0f , 0.0f , 0.0f));
		//CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
		if ( DeactiveAbilityMontage ) {
			GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("Blast stop montage play"));
			// Play Animation
			bool bIsMontagePlaying = GetMesh()->GetAnimInstance()->Montage_IsPlaying(DeactiveAbilityMontage);
			if ( !bIsMontagePlaying ) {
				GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("Blast montage stop"));
				ServerAbility(false);	// request ability animation on server
			}
		}
	}
	else if ( nowAbility == EPlayerAbility::ETelekinesis ) {
		// blank
	}
}

// Replicate Orient Roation to Move
bool ACraftingStarCharacter::ServerOrientRotationToMove_Validate(bool rotateToMove) {
	return true;
}
void ACraftingStarCharacter::ServerOrientRotationToMove_Implementation(bool rotateToMove) {
	MulticastOrientRotationToMove(rotateToMove);
}
void ACraftingStarCharacter::MulticastOrientRotationToMove_Implementation(bool rotateToMove) {
	GetCharacterMovement()->bOrientRotationToMovement = rotateToMove;	// false: fix , true: rotate
}

void ACraftingStarCharacter::MouseLeftPressed() {
	if ( nowAbility != EPlayerAbility::ENone ) {
		if ( nowAbility == EPlayerAbility::ETelekinesis ) {
			switch ( HasAuthority() ) {
			case true :
				GetCharacterMovement()->bOrientRotationToMovement = false; // Character doesn't moves in the direction of input...
				break;
			case false :
				ServerOrientRotationToMove(false);
				break;
			}
			if ( abilityReadyStatus ) {
				KeepAbility = true;

				if ( AbilityMontage ) {
					// Play Animation
					bool bIsMontagePlaying = GetMesh()->GetAnimInstance()->Montage_IsPlaying(AbilityMontage);
					if ( !bIsMontagePlaying ) {
						ServerAbility(true);	// request ability animation on server
					}
				}
			}
		}
	}
}

void ACraftingStarCharacter::MouseLeftReleased() {
	if ( nowAbility != EPlayerAbility::ENone ) {
		if ( nowAbility == EPlayerAbility::ETelekinesis ) {
			if ( abilityReadyStatus ) {
				KeepAbility = false;

				if ( selectedTarget ) {
					// Change Tele' Interactive Actor's Color
					if ( Cast<ATelekinesisInteractableObject>(selectedTarget->GetOwner()) )
					{
						// Set CustomDepth Stencil Value to chagne Color
						Cast<ATelekinesisInteractableObject>(selectedTarget->GetOwner())->ActorMesh->SetCustomDepthStencilValue(0);
						switch ( HasAuthority() ) {
						case true :
							PhysicsHandle->ReleaseComponent();
							if ( Cast<ATelekinesisInteractableObject>(selectedTarget->GetOwner()) )
							{
								if ( !Cast<ATelekinesisInteractableObject>(selectedTarget->GetOwner())->isPhysics ) {
									selectedTarget->SetSimulatePhysics(false);
								}
							}
							selectedTarget = NULL;
							break;
						case false :
							ServerReleaseComponent();
							ServerDeselectTarget();
							break;
						}
					}
				}

				if ( DeactiveAbilityMontage ) {
					// Play Animation
					bool bIsMontagePlaying = GetMesh()->GetAnimInstance()->Montage_IsPlaying(DeactiveAbilityMontage);
					if ( !bIsMontagePlaying ) {
						ServerAbility(false);	// request ability animation on server
					}
				}

				switch ( HasAuthority() ) {
				case true:
					GetCharacterMovement()->bOrientRotationToMovement = true; // Character't moves in the direction of input...
					break;
				case false:
					ServerOrientRotationToMove(true);
					break;
				}
			}
		}
	}
}

// Release Component on Server
bool ACraftingStarCharacter::ServerReleaseComponent_Validate() {
	return true;
}
void ACraftingStarCharacter::ServerReleaseComponent_Implementation() {
	MulticastReleaseComponent();
}
void ACraftingStarCharacter::MulticastReleaseComponent_Implementation() {
	PhysicsHandle->ReleaseComponent();
	GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("Release Component"));

	// Set Simulate Physics to false
	if ( selectedTarget ) {
		GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("Have selectedTarget"));
		if ( Cast<ATelekinesisInteractableObject>(selectedTarget->GetOwner()) )
		{
			if ( !Cast<ATelekinesisInteractableObject>(selectedTarget->GetOwner())->isPhysics ) {
				selectedTarget->SetSimulatePhysics(false);
				GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("SetSimulatePhysics"));
			}
		}
	}
}

void ACraftingStarCharacter::ActivateAbility2()
{
	if ( nowAbility == EPlayerAbility::EAbility_dummy1 )
	{
		UseProjectionBow();
	}
}

void  ACraftingStarCharacter::SetPause(bool isPaused) {
	if ( Cast<ACraftingStarPC>(GetController()) != nullptr ) {
		if ( isPaused ) {
			//Ű      Է       &    콺        Ȱ  ȭ
			Cast<ACraftingStarPC>(GetController())->SetInputMode(FInputModeGameAndUI());
			DisableInput(Cast<ACraftingStarPC>(GetController()));
			Cast<ACraftingStarPC>(GetController())->SetShowMouseCursor(true);
		}
		else {
			//Ű      Է       &    콺          Ȱ  ȭ
			Cast<ACraftingStarPC>(GetController())->SetInputMode(FInputModeGameOnly());
			EnableInput(Cast<ACraftingStarPC>(GetController()));
			Cast<ACraftingStarPC>(GetController())->SetShowMouseCursor(false);
		}
	}
}

void ACraftingStarCharacter::OnResetVR()
{
	// If CraftingStar is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in CraftingStar.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ACraftingStarCharacter::TouchStarted(ETouchIndex::Type FingerIndex , FVector Location)
{
	Jump();
}

void ACraftingStarCharacter::TouchStopped(ETouchIndex::Type FingerIndex , FVector Location)
{
	StopJumping();
}

void ACraftingStarCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACraftingStarCharacter::LookUpAtRate(float Rate)
{
	// forbid to look up while keeping using ability
	if ( nowAbility == EPlayerAbility::EBlast ) {
		if ( !KeepAbility ) {
			// calculate delta for this frame from the rate information
			AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
		}
	}
}

void ACraftingStarCharacter::MoveForward(float Value)
{
	// forbid to move character while using blast skill
	if ( nowAbility == EPlayerAbility::EBlast && KeepAbility ) {
		return;
	}

	if ( ( Controller != nullptr ) && ( Value != 0.0f ) )
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0 , Rotation.Yaw , 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction , Value);
	}
}

void ACraftingStarCharacter::MoveRight(float Value)
{
	// forbid to move character while using blast skill
	if ( nowAbility == EPlayerAbility::EBlast && KeepAbility ) {
		return;
	}

	if ( ( Controller != nullptr ) && ( Value != 0.0f ) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0 , Rotation.Yaw , 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction , Value);
	}
}


//게임 플레이도중 플레이어가 사라짐
void ACraftingStarCharacter::Destroyed()
{
	Super::Destroyed();

	//게임 모드에서 OnPlayerDied 이벤트에 바인딩한 예 
	if ( UWorld* World = GetWorld() )
	{
		if ( ACraftingStarGameMode* GameMode = Cast<ACraftingStarGameMode>(World->GetAuthGameMode()) )
		{
			GameMode->GetOnPlayerDied().Broadcast(this);
		}
	}
}

//리스폰 게임플레이 요청
void ACraftingStarCharacter::CallRespawnPlayer_Implementation()
{
	//폰 컨트롤러에 대한 레퍼런스 구하기
	AController* CortollerRef = GetController();

	//위치 변경
	if ( UWorld* World = GetWorld() )
	{
		if ( ACraftingStarGameMode* GameMode = Cast<ACraftingStarGameMode>(World->GetAuthGameMode()) )
		{
			GameMode->RespawnPlayer(this);
		}
	}

	//플레이어 삭제 이벤트
	//플레이어 소멸.  
	//Destroy();

	////월드와 월드의 게임 모드가 RestartPlayer 함수를 호출하도록 함.
	//if (UWorld* World = GetWorld())
	//{
	//	if (ACraftingStarGameMode* GameMode = Cast<ACraftingStarGameMode>(World->GetAuthGameMode()))
	//	{
	//		GameMode->RestartPlayer(CortollerRef);
	//	}
	//}
}


void ACraftingStarCharacter::LightAct(AActor* target , FVector Location)
{
	if ( !target )
	{
		return;
	}
	if ( auto State = Cast<ACraftingStarPS>(this->GetPlayerState()) )
	{
		if ( HasAuthority() )
		{
			MulticastLightAct(target , Location , EPlayerRole::ELight == State->PlayerData.Mode);
		}
		else
		{
			ServerLightAct(target , Location , EPlayerRole::ELight == State->PlayerData.Mode);
		}
	}



}

void ACraftingStarCharacter::ServerLightAct_Implementation(AActor* target , FVector Location , bool bIsLight)
{
	MulticastLightAct(target , Location , bIsLight);
}

void ACraftingStarCharacter::MulticastLightAct_Implementation(AActor* target , FVector Location , bool bIsLight)
{
	//Here Ejection Abillity Interaction
	if ( target->Implements<ULightSensingInterface>() )
	{
		//빛 대상
		//ILightSensingInterface를 상속받은 얘의 React 함수를 호출받는 방법.
		ILightSensingInterface::Execute_React(target , bIsLight , Location);
	}
}

void ACraftingStarCharacter::UseProjectionTwoHanded()
{
	ServerUseProjectionTwoHanded();
}

void ACraftingStarCharacter::ServerUseProjectionTwoHanded_Implementation()
{
	MulticastUseProjectionTwoHanded();
}

void ACraftingStarCharacter::MulticastUseProjectionTwoHanded_Implementation()
{
	if ( auto montage = GetWeaponComponent()->GetProjectionTwoHandedMontage() )
	{
		Weapon_rMesh->WeaponChange();
		Weapon_rMesh->bCanDamage = true;

		GetMesh()->GetAnimInstance()->Montage_Play(montage);
	}


}

void ACraftingStarCharacter::UseProjectionBow()
{
	ServerUseProjectionBow();
}

void ACraftingStarCharacter::ServerUseProjectionBow_Implementation()
{
	MulticastUseProjectionBow();
}

void ACraftingStarCharacter::MulticastUseProjectionBow_Implementation()
{
	Bow_lMesh->Equip();
	Bow_lMesh->Shoot();

}

void ACraftingStarCharacter::ServerRequestLoadSaveData_Implementation()
{
	LoadSaveData(false);
}

void ACraftingStarCharacter::LoadSaveData(bool isHost)
{
	
	FOutputDeviceNull Ar;

	if ( isHost ) {
		CallFunctionByNameWithArguments(TEXT("BP_LoadSaveData_Server") , Ar , nullptr , true);
	}
	else {
		CallFunctionByNameWithArguments(TEXT("BP_LoadSaveData_Client") , Ar , nullptr , true);
	}
	
}

