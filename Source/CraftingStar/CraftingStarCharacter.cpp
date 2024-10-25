// Copyright Epic Games, Inc. All Rights Reserved.

#include "CraftingStarCharacter.h"
#include "CraftingStarPC.h"
#include "CraftingStarPS.h"
#include "CraftingStarGS.h"
#include "UtilityFunction.h"
#include "CraftingStarGameInstance.h"
#include "CustomEnum.h"
#include "UtilityFunction.h"
#include "Components/WidgetComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TelekinesisInteractableObject.h"
#include "Kismet/KismetMathLibrary.h"
#include "CraftingStarGameMode.h"
#include "DrawDebugHelpers.h"
#include "NiagaraSystem.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraFunctionLibrary.h" 
#include "NiagaraComponent.h"
#include "Net/UnrealNetwork.h"
#include "UtilityFunction.h"
#include "LaserNiagaraComponent.h"
#include "TelekinesisInteractableObject.h"
#include "Ability/AssimilationComponent.h"
#include "Ability/WeaponComponent.h"
#include "Ability/BowComponent.h"
#include "Object/LightSensingObject.h"
#include "Components/AudioComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Misc/OutputDeviceNull.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Popo.h"

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


	ConstructorHelpers::FObjectFinder<USkeletalMesh> BodySM_Light(TEXT("SkeletalMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/BodyPart/Body10_SK.Body10_SK'"));
	if ( BodySM_Light.Succeeded() ) {
		GetMesh()->SetSkeletalMesh(BodySM_Light.Object);
		BodyMesh_Light = BodySM_Light.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> BodySMMat_Light(TEXT("Material'/Game/Assets/PlayerMaterial/Material/Light/MaskTintPolyart_Light_Body.MaskTintPolyart_Light_Body'"));
	if ( BodySMMat_Light.Succeeded() ) {
		BodyMat_Light = BodySMMat_Light.Object;
;	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> BodySM_Dark(TEXT("SkeletalMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/BodyPart/Body09_SK.Body09_SK'"));
	if ( BodySM_Dark.Succeeded() ) {
		BodyMesh_Dark = BodySM_Dark.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> BodySMMat_Dark(TEXT("Material'/Game/Assets/PlayerMaterial/Material/Dark/MaskTintPolyart_suit.MaskTintPolyart_suit'"));

	if ( BodySMMat_Dark.Succeeded() ) {
		BodyMat_Dark = BodySMMat_Dark.Object;
	}
	
    
	// Character Mesh
	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> HeadSM_Light(TEXT("StaticMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/HeadPart/Head02_Female_SM.Head02_Female_SM'"));
	if ( HeadSM_Light.Succeeded() ) {
		HeadMesh->SetStaticMesh(HeadSM_Light.Object);
		HeadMesh_Light = HeadSM_Light.Object;
	}


	ConstructorHelpers::FObjectFinder<UStaticMesh> HeadSM_Dark(TEXT("StaticMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/HeadPart/Head01_Male_SM.Head01_Male_SM'"));
	if ( HeadSM_Dark.Succeeded() ) {
		HeadMesh_Dark = HeadSM_Dark.Object;
	}


	HeadMesh->SetupAttachment(GetMesh() , FName(TEXT("Head")));
	HeadMesh->SetRelativeLocation(FVector(0.0f , 0.0f , 0.0f));
	HeadMesh->SetRelativeRotation(FRotator(-90.0f , 0.0f , 0.0f));
	HeadMesh->SetRelativeLocation(FVector(0.0f , 0.0f , 0.0f));
	HeadMesh->SetRelativeRotation(FRotator(-90.0f , 0.0f , 0.0f));

	HairAndHatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HairAndHatMesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> HairAndHatSM_Light(TEXT("StaticMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/HeadPart/Hair09_SM.Hair09_SM'"));
	if ( HairAndHatSM_Light.Succeeded() ) {
		HairAndHatMesh->SetStaticMesh(HairAndHatSM_Light.Object);
		HairAndHatMesh_Light = HairAndHatSM_Light.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterialInterface> HairSMMat_Light(TEXT("Material'/Game/Assets/PlayerMaterial/Material/Light/MaskTintPolyart_Light_Hair.MaskTintPolyart_Light_Hair'"));

	if ( HairSMMat_Light.Succeeded() ) {
		HairMat_Light = HairSMMat_Light.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> HairAndHatSM_Dark(TEXT("StaticMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/HeadPart/Hair02_SM.Hair02_SM'"));
	if ( HairAndHatSM_Dark.Succeeded() ) {
		HairAndHatMesh_Dark = HairAndHatSM_Dark.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterialInterface> HairSMMat_Dark(TEXT("Material'/Game/Assets/PlayerMaterial/Material/Dark/MaskTintPolyart_Dark_hair_notTh.MaskTintPolyart_Dark_hair_notTh'"));
	if ( HairSMMat_Dark.Succeeded() ) {
		HairMat_Dark = HairSMMat_Dark.Object;
	}


	HairAndHatMesh->SetupAttachment(GetMesh() , FName(TEXT("Head")));
	HairAndHatMesh->SetRelativeLocation(FVector(0.0f , 0.0f , 0.0f));
	HairAndHatMesh->SetRelativeRotation(FRotator(-90.0f , 0.0f , 0.0f));

	EyesMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Eyes"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> EyesSM_Light(TEXT("StaticMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/HeadPart/Eye08_SM.Eye08_SM'"));
	if ( EyesSM_Light.Succeeded() ) {
		EyesMesh->SetStaticMesh(EyesSM_Light.Object);
		EyesMesh_Light = EyesSM_Light.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterialInterface> EyesSMMat_Light(TEXT("Material'/Game/Assets/PlayerMaterial/Material/Light/MaskTintPolyart_Light_Eye.MaskTintPolyart_Light_Eye'"));

	if ( EyesSMMat_Light.Succeeded() ) {
		EyesMat_Light = EyesSMMat_Light.Object;
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> EyesSM_Dark(TEXT("StaticMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/HeadPart/Eye01_SM.Eye01_SM'"));
	if ( EyesSM_Dark.Succeeded() ) {
		EyesMesh_Dark = EyesSM_Dark.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterialInterface> EyesSMMat_Dark(TEXT("Material'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Material/DefaultPolyart_MAT.DefaultPolyart_MAT'"));

	if ( EyesSMMat_Dark.Succeeded() ) {
		EyesMat_Dark = EyesSMMat_Dark.Object;
	}

	EyesMesh->SetupAttachment(GetMesh() , FName(TEXT("Head")));
	EyesMesh->SetRelativeLocation(FVector(0.0f , 0.0f , 0.0f));
	EyesMesh->SetRelativeRotation(FRotator(-90.0f , 0.0f , 0.0f));

	MouthMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mouth"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MouthSM_Light(TEXT("StaticMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/HeadPart/Mouth02_SM.Mouth02_SM'"));

	if ( MouthSM_Light.Succeeded() ) {
		MouthMesh->SetStaticMesh(MouthSM_Light.Object);
		MouthMesh_Light = MouthSM_Light.Object;
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh> MouthSM_Dark(TEXT("StaticMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/HeadPart/Mouth01_SM.Mouth01_SM'"));
	if ( MouthSM_Dark.Succeeded() ) {
		MouthMesh_Dark = MouthSM_Dark.Object;
	}

	MouthMesh->SetupAttachment(GetMesh() , FName(TEXT("Head")));
	MouthMesh->SetRelativeLocation(FVector(0.0f , 0.0f , 0.0f));
	MouthMesh->SetRelativeRotation(FRotator(-90.0f , 0.0f , 0.0f));

	CloakMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Cloak"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> CloakSM_Light(TEXT("SkeletalMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/BodyPart/Cloak02_SK.Cloak02_SK'"));
	if ( CloakSM_Light.Succeeded() ) {
		CloakMesh->SetSkeletalMesh(CloakSM_Light.Object);
		CloakMesh_Light = CloakSM_Light.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterialInterface> CloakSMMat_Light(TEXT("Material'/Game/Assets/PlayerMaterial/Material/Light/MaskTintPolyart_Light_Cape.MaskTintPolyart_Light_Cape'"));

	if ( CloakSMMat_Light.Succeeded() ) {
		CloakMat_Light =  CloakSMMat_Light.Object;
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> CloakSM_Dark(TEXT("SkeletalMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/BodyPart/Cloak03_SK.Cloak03_SK'"));
	if ( CloakSM_Dark.Succeeded() ) {
		CloakMesh_Dark = CloakSM_Dark.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterialInterface> CloakSMMat_Dark(TEXT("Material'/Game/Assets/PlayerMaterial/Material/Dark/MaskTintPolyart_cape.MaskTintPolyart_cape'"));
	if ( CloakSMMat_Dark.Succeeded() ) {
		CloakMat_Dark = CloakSMMat_Dark.Object;
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

	
	interactTag = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractTag Source"));
	interactTag->SetupAttachment(GetMesh() , FName(TEXT("InteractTag")));
	

	// Sounds
	audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	audioComp->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<USoundWave> Resource_SW_EmissionDark(TEXT("SoundWave'/Game/Assets/Sound/emission_Dark.emission_Dark'"));
	SW_EmissionDark = Resource_SW_EmissionDark.Object;
	static ConstructorHelpers::FObjectFinder<USoundWave> Resource_SW_EmissionLight(TEXT("SoundWave'/Game/Assets/Sound/emission_Light.emission_Light'"));
	SW_EmissionLight = Resource_SW_EmissionLight.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave> Resource_SW_Telekinesis(TEXT("SoundWave'/Game/Assets/Sound/telekinesis.telekinesis'"));
	SW_Telekinesis = Resource_SW_Telekinesis.Object;

	// Create a camera boom (pulls in towards the player if there is a collis`ion)
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

	// Revival Interactive Range
	RevivalInteractiveRange = CreateDefaultSubobject<USphereComponent>(TEXT("RevivalInteractiveRange"));
	RevivalInteractiveRange->SetupAttachment(RootComponent);
	RevivalInteractiveRange->InitSphereRadius(150.f);

	// On Damaged
	AttackedCnt_Popo = 0;
	isKnockedDown = false;

	// Ability
	KeepAbility = false;
	WandReadySign = false;
}

void ACraftingStarCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACraftingStarCharacter , OffsetAxis);
	DOREPLIFETIME(ACraftingStarCharacter , KeepAbility);
	DOREPLIFETIME(ACraftingStarCharacter , isKnockedDown);
}


//////////////////////////////////////////////////////////////////////////
// Input
// Called when the game starts or when spawned
void ACraftingStarCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetPause(false);
	interactTag->SetVisibility(false);


	AController* controller = GetController();

	if ( UUtilityFunction::IsHost (controller)) {
		if ( LoadingWidgetRef == nullptr   && Cast<ACraftingStarGS>(GetWorld()->GetGameState())->isStartFlag == false)
		{
			//LoadingWB
			LoadingWidgetRef = CreateWidget(GetWorld() , LoadingWidget);
			LoadingWidgetRef->AddToViewport();
		}
		
		if ( GameWidgetRef == nullptr ) {
			GameWidgetRef = CreateWidget(GetWorld() , GameWidget);
			GameWidgetRef->AddToViewport();
		}

		GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("LOAD Server"));
		LoadSaveData(true);
	}
	else if(controller != nullptr &&!controller->HasAuthority() && controller->IsLocalPlayerController()) {
		
		
		if ( LoadingWidgetRef == nullptr  && Cast<ACraftingStarGS>(GetWorld()->GetGameState())->isStartFlag == false )
		{
			//LoadingWB
			LoadingWidgetRef = CreateWidget(GetWorld() , LoadingWidget);
			LoadingWidgetRef->AddToViewport();
		}

		if ( GameWidgetRef == nullptr ) {
			GameWidgetRef = CreateWidget(GetWorld() , GameWidget);
			GameWidgetRef->AddToViewport();
		}

		//호스트는 리플리케이션 문제로 통신지연이 되면 데이터 로드를 온전히 못할 수도 있으므로
		//미리 데이터 갖고오기
		//ACraftingStarPS* playerState = Cast<ACraftingStarPS>(GetPlayerState());
		//playerState->PlayerData = data;
		
		GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("LOAD Client"));
		ServerRequestLoadSaveData();
	}

	Comp_LaserNiagara->Hide();

	// Revival
	RevivalInteractiveRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// Popo
	AttackedCnt_Popo = 0;
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
	PlayerInputComponent->BindAxis("LookUp" , this , &ACraftingStarCharacter::LookUp);
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
	/*
	if ( isKnockedDown ) {
		GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("Dead"));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("Alive"));
	}*/

	if ( KeepAbility && WandReadySign ) {
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

// Select Target
bool ACraftingStarCharacter::ServerSetKeepAbility_Validate(bool isKeeping) {
	return true;
}
void ACraftingStarCharacter::ServerSetKeepAbility_Implementation(bool isKeeping) {
	MulticastSetKeepAbility(isKeeping);
}
void ACraftingStarCharacter::MulticastSetKeepAbility_Implementation(bool isKeeping) {
	KeepAbility = isKeeping;
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
			WandReadySign = false;
			DeactivateAbility();
			GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("2"));
			switch ( HasAuthority() ) {
			case true :
				MulticastSetKeepAbility(false);
				break;
			case false :
				ServerSetKeepAbility(false);
				break;
			}
		}
	}
	else if ( nowAbility == EPlayerAbility::ETelekinesis ) {
		GEngine->AddOnScreenDebugMessage(-1 , 3 , FColor::Red , FString::Printf(TEXT("Try To Cancel tele")));
		if ( abilityReadyStatus ) {
			// Deactivate Telekinesis Skill
			MouseLeftReleased();
			ActivateAbility();
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

    GEngine->AddOnScreenDebugMessage(-1 , 3 , FColor::Red , FString::Printf(TEXT("RemoveWidget_MULTICAST")));

	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());

	//GameWB     
	//CreateWidget(GetWorld() , GameWidget)->AddToViewport();
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
	auto playerState = Cast<ACraftingStarPS>(GetPlayerState());
	if ( playerState == nullptr ) return;

	if ( SystemMenuWidgetRef == NULL &&( playerState->NowState != EPlayerGMState::EInteraction )) {
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

// Character On KnockedDown Base
void ACraftingStarCharacter::OnKnockedDown() {
	// Play KnockedDownMontage_Popo
	switch ( HasAuthority() ) {
	case true:
		MulticastPlayMontage(KnockedDownMontage_Popo);
		break;
	case false:
		ServerPlayMontage(KnockedDownMontage_Popo);
		break;
	}

	// Disable Player Movement
	GetCharacterMovement()->DisableMovement();

	// Set isKnockedDown True
	switch ( HasAuthority() ) {
	case true:
		MulticastSetisKnockedDown(true);
		break;
	case false:
		ServerSetisKnockedDown(true);
		break;
	}
}

// On Damaged by Popo
void ACraftingStarCharacter::OnDamaged_Popo() {
	AttackedCnt_Popo++;

	if ( AttackedCnt_Popo >= 3 ) {
		GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("쓰러짐"));
		OnKnockedDown_Popo();	// Play KnockedDownMontage_Popo
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("맞음: %f") , AttackedCnt_Popo));

		// Play HitMontage_Popo
		switch ( HasAuthority() ) {
		case true:
			MulticastPlayMontage(HitMontage_Popo);
			break;
		case false:
			ServerPlayMontage(HitMontage_Popo);
			break;
		}
	}
}

// On KnockedDown by Popo
void ACraftingStarCharacter::OnKnockedDown_Popo() {
	OnKnockedDown();

	// Find Popo
	UClass* PopoBP = StaticLoadClass(APopo::StaticClass() , nullptr , TEXT("Blueprint'/Game/NPC/Monster/Blueprint/Popo/BP_Popo.BP_Popo_C'"));
	AActor* MyPopo = UGameplayStatics::GetActorOfClass(GetWorld() , PopoBP);
	APopo* PopoActor = Cast<APopo>(MyPopo);
	
	// Increase Popo CatchedPlayerCnt
	if ( PopoActor ) {
		PopoActor->CatchedPlayerCnt++;
		GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("포포 냠냠: %d") , PopoActor->CatchedPlayerCnt));
	}

	// Prepare for Revival
	RevivalInteractiveRange->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// Character On Revive Base
void ACraftingStarCharacter::OnRevive() {

	// Set isKnockedDown False
	switch ( HasAuthority() ) {
	case true:
		MulticastSetisKnockedDown(false);
		break;
	case false:
		ServerSetisKnockedDown(false);
		break;
	}

	// Enable Player Movement
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	// Stop KnockedDownMontage_Popo and Play ReviveMontage_Popo
	if ( KnockedDownMontage_Popo && ReviveMontage_Popo ) {
		// Play Animation
		switch ( HasAuthority() ) {
		case true:
			MulticastStopMontage(KnockedDownMontage_Popo);
			MulticastPlayMontage(ReviveMontage_Popo);
			break;
		case false:
			ServerStopMontage(KnockedDownMontage_Popo);
			ServerPlayMontage(ReviveMontage_Popo);
			break;
		}
	}

	// Disable Interactive Range
	RevivalInteractiveRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// On Revive by another Player
void ACraftingStarCharacter::OnRevive_Popo() {
	OnRevive();

	AttackedCnt_Popo = 0;

	// Find Popo
	UClass* PopoBP = StaticLoadClass(APopo::StaticClass() , nullptr , TEXT("Blueprint'/Game/NPC/Monster/Blueprint/Popo/BP_Popo.BP_Popo_C'"));
	AActor* MyPopo = UGameplayStatics::GetActorOfClass(GetWorld() , PopoBP);
	APopo* PopoActor = Cast<APopo>(MyPopo);

	// Decrease Popo CatchedPlayerCnt
	if ( PopoActor ) {
		PopoActor->CatchedPlayerCnt--;
	}
}
// Set and Replicate isKnockedDown
bool ACraftingStarCharacter::ServerSetisKnockedDown_Validate(bool knockedDownValue) {
	return true;
}
void ACraftingStarCharacter::ServerSetisKnockedDown_Implementation(bool knockedDownValue) {
	MulticastSetisKnockedDown(knockedDownValue);
}
void ACraftingStarCharacter::MulticastSetisKnockedDown_Implementation(bool knockedDownValue) {
	isKnockedDown = knockedDownValue;
	if ( knockedDownValue ) {
		GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("Dead!"));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("Alive!"));
	}
}

bool ACraftingStarCharacter::ServerPlayMontage_Validate(UAnimMontage* animMontage) {
	return true;
}
void ACraftingStarCharacter::ServerPlayMontage_Implementation(UAnimMontage* animMontage) {
	MulticastPlayMontage(animMontage);
}
void ACraftingStarCharacter::MulticastPlayMontage_Implementation(UAnimMontage* animMontage) {
	if ( animMontage ) {
		// Play Animation
		GetMesh()->GetAnimInstance()->Montage_Play(animMontage , 1.0f);
	}
}
bool ACraftingStarCharacter::ServerStopMontage_Validate(UAnimMontage* animMontage) {
	return true;
}
void ACraftingStarCharacter::ServerStopMontage_Implementation(UAnimMontage* animMontage) {
	MulticastStopMontage(animMontage);
}
void ACraftingStarCharacter::MulticastStopMontage_Implementation(UAnimMontage* animMontage) {
	if ( animMontage ) {
		// Play Animation
		GetMesh()->GetAnimInstance()->Montage_Play(animMontage , 1.0f);
	}
}

// Magic Wand Line Trace for Ability
bool ACraftingStarCharacter::WandLineTrace(float distance) {

	/* Set LineTrace */

	// Result oof LineTrace
	FHitResult Hit;

	// Ability Spawn Loc Socket Transform
	//FVector SpawnLocation = this->Weapon_rMesh->GetSocketLocation(FName("SpawnLoc"));
	FVector SpawnLocation = this->SpawnLocSource->GetComponentLocation();
	
	// Start point and End point of LineTrace
	FVector Start = SpawnLocation;
	//FVector End = SpawnLocation + ( FollowCamera->GetForwardVector() * distance );
	FVector End = SpawnLocation + ( SpawnLocSource->GetUpVector() * distance );

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
	FVector SpawnLocation = this->SpawnLocSource->GetComponentLocation();
	// Start point and End point of LineTrace
	FVector Start = SpawnLocation;
	//FVector End = SpawnLocation + ( FollowCamera->GetForwardVector() * 750 );
	FVector End = SpawnLocation + ( SpawnLocSource->GetUpVector() * teleLaserDistance );

	// Trace Channel: Custom Trace Channel - AbilitySpawn
	ECollisionChannel Channel = ECollisionChannel::ECC_GameTraceChannel1;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	/* Execute LineTrace */
	GetWorld()->LineTraceSingleByChannel(Hit , Start , End , Channel , QueryParams);

	// Visualize LineTrace
	DrawDebugLine(GetWorld() , Start , End , FColor::Green);

	// Draw Laser
	//GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("x : %f  y : %f  z : %f ") , End.X , End.Y , End.Z));
	Comp_LaserNiagara->SetLaser(Hit , End);
	
	
	if ( Hit.bBlockingHit ) 
	{
		ensure(Hit.GetComponent());
		if ( selectedTarget == NULL && Cast<ATelekinesisInteractableObject>(Hit.GetComponent()->GetOwner()) ) 
		{
			teleLaserDistance = Hit.Distance;
			teleComponentDistance = Hit.Distance;
			//teleForce = 5000.0f;
			// Grab selectedTarget Component
			switch ( HasAuthority() ) {
			case true:
				selectedTarget = Hit.GetComponent();

				//PhysicsHandle->GrabComponent(selectedTarget , NAME_None , End , true);
				PhysicsHandle->GrabComponentAtLocationWithRotation(selectedTarget , NAME_None , Hit.Location , FRotator(0 , 0 , 0));

				if ( selectedTarget ) {
					// Check is the simulate physics true
					if ( !selectedTarget->IsSimulatingPhysics() ) {
						selectedTarget->SetSimulatePhysics(true);
					}
				}
				break;
			case false:
				ServerSelectTarget(Hit);
				ServerGrabComponent(End, Hit);
				break;
			}
		}
	}

	if ( selectedTarget != NULL ) {
		//GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("selected object distance: %f") , Hit.Distance));

		//물체가 도충에 파괴된 경우 GetOwner()에서 크래시가 뜸, 그래서 IsValid를 사용
		if ( IsValid(selectedTarget->GetOwner()) ) {
			// Move target if it can be cast
			// Change Tele' Interactive Actor's Outline Color
			if ( Cast<ATelekinesisInteractableObject>(selectedTarget->GetOwner()) )
			{
				FLatentActionInfo LatentInfo;
				LatentInfo.CallbackTarget = this;
			// Move selectedTarget Component
				switch ( HasAuthority() ) {
				case true:
					PhysicsHandle->SetTargetLocation(End);
					teleComponentDistance = ( End - selectedTarget->GetComponentLocation() ).Size();
					if ( teleComponentDistance <= 100 ) {
						//teleForce = 0;
						//selectedTarget->GetOwner()->GetRootComponent()->ComponentVelocity = FVector(0 , 0 , 0);
						//GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("selected object distance: %f") , teleComponentDistance));
					}
					else {
						//teleForce = 3000.0f;
						//selectedTarget->GetOwner()->GetRootComponent()->ComponentVelocity = UKismetMathLibrary::GetDirectionUnitVector(selectedTarget->GetComponentLocation() , End);
						//GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("selected object distance: %f") , teleComponentDistance));
					}
					
					//selectedTarget->MoveComponent(UKismetMathLibrary::GetDirectionUnitVector(selectedTarget->GetComponentLocation() , End) * selectedTarget->GetMass() * teleForce , selectedTarget->GetComponentRotation() , true);
					
					//UKismetSystemLibrary::MoveComponentTo(selectedTarget , End , selectedTarget->GetRelativeRotation(), false, false, 0.1f, true, EMoveComponentAction::Type::Move , LatentInfo);
					//selectedTarget->AddForce(UKismetMathLibrary::GetDirectionUnitVector(selectedTarget->GetComponentLocation() , End) * selectedTarget->GetMass() * teleForce);
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
	selectedTarget = Hit.GetComponent();
	//123
	auto GrabActor = Cast<ATelekinesisInteractableObject>(Hit.GetComponent()->GetOwner());
	GrabActor->SetTelekinesisOwner(this);
	
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
		selectedTarget = NULL;
	}
}

// Grab Component on Server
bool ACraftingStarCharacter::ServerGrabComponent_Validate(FVector End, FHitResult Hit) {
	return true;
}
void ACraftingStarCharacter::ServerGrabComponent_Implementation(FVector End, FHitResult Hit) {
	MulticastGrabComponent(End, Hit);
}
void ACraftingStarCharacter::MulticastGrabComponent_Implementation(FVector End, FHitResult Hit) {
	PhysicsHandle->GrabComponentAtLocationWithRotation(selectedTarget , NAME_None , Hit.Location, FRotator(0, 0, 0));
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
	//selectedTarget->AddForce(UKismetMathLibrary::GetDirectionUnitVector(selectedTarget->GetComponentLocation(), End) * teleForce * selectedTarget->GetMass());
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
		if ( nowAbility == EPlayerAbility::EBlast ) {	// No: Tele
			bUseControllerRotationYaw = false;	// Not Rotate the player based on the controller
		}
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
	GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("removeOutline"));

}

// Wand Skill Animation Offset
void ACraftingStarCharacter::SetOffsetAxis() {
	
	if ( HasAuthority() )
	{
		ServerSetOffsetAxis();
	}
	else
	{
		MulticastSetOffsetAxis();
	}
	
}
bool ACraftingStarCharacter::ServerSetOffsetAxis_Validate() {
	return true;
}
void ACraftingStarCharacter::ServerSetOffsetAxis_Implementation() {
	MulticastSetOffsetAxis();
}
void ACraftingStarCharacter::MulticastSetOffsetAxis_Implementation() 
{
	OffsetAxis = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation() , GetActorRotation());
}
FRotator ACraftingStarCharacter::GetOffsetAxis() {
	return OffsetAxis;
}

// Input Ability (Key: E)
void ACraftingStarCharacter::ActivateAbility() {
	if ( nowAbility == EPlayerAbility::EBlast ) {
		// Sound
		if ( Cast<ACraftingStarPS>(GetPlayerState())->PlayerData.Mode == EPlayerRole::EDark ) {
			audioComp->SetSound(SW_EmissionDark);
		}
		else if ( Cast<ACraftingStarPS>(GetPlayerState())->PlayerData.Mode == EPlayerRole::ELight ) {
			audioComp->SetSound(SW_EmissionLight);
		}
		audioComp->Play();

		if ( AbilityMontage ) {
			// Play Animation
			bool bIsMontagePlaying = GetMesh()->GetAnimInstance()->Montage_IsPlaying(AbilityMontage);
			if ( !bIsMontagePlaying ) {
				ServerAbility(true);	// request ability animation on server
				switch ( HasAuthority() ) {
				case true:
					MulticastSetKeepAbility(true);
					break;
				case false:
					ServerSetKeepAbility(true);
					break;
				}
			}
		}

		CameraBoom->SetRelativeLocation(FVector(0.0f , 100.0f , 100.0f));
		CameraBoom->bUsePawnControlRotation = false; // Does not rotate the arm based on the controller
	}
	else if ( nowAbility == EPlayerAbility::ETelekinesis ) {
		//GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("Tele"));
		if ( !abilityReadyStatus ) {
			teleComponentDistance = 0;
			//teleForce = 5000.0f;

			abilityReadyStatus = true;

			CameraBoom->SetRelativeLocation(FVector(0.0f , 100.0f , 100.0f));
			CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
			
			bUseControllerRotationYaw = true;
			GetCharacterMovement()->bOrientRotationToMovement = false; // Character doesn't move in the direction of input...
			
			CreateTeleObjOutline();
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("Cancel"));
			CameraBoom->SetRelativeLocation(FVector(0.0f , 0.0f , 0.0f));
			//CameraBoom->bUsePawnControlRotation = false; // Does not rotate the arm based on the controller
			
			bUseControllerRotationYaw = false;
			GetCharacterMovement()->bOrientRotationToMovement = true; // Character  moves in the direction of input...

			RemoveTeleObjOutline();
			switch ( HasAuthority() ) {
			case true :
				if ( selectedTarget != NULL ) {
					selectedTarget = NULL;
				}
				break;
			case false :
				ServerDeselectTarget();
				break;
			}

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
		WandReadySign = false;
		// Sound
		audioComp->Stop();
		audioComp->SetSound(NULL);

		GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("Blast deactivate"));
		switch ( HasAuthority() ) {
		case true:
			MulticastSetKeepAbility(false);
			break;
		case false:
			ServerSetKeepAbility(false);
			break;
		}

		CameraBoom->SetRelativeLocation(FVector(0.0f , 0.0f , 0.0f));
		CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
		if ( DeactiveAbilityMontage ) {
			ServerAbility(false);	// request ability animation on server
			// Play Animation
			GetMesh()->GetAnimInstance()->Montage_IsPlaying(DeactiveAbilityMontage);
		}

	
	}
	else if ( nowAbility == EPlayerAbility::ETelekinesis ) {
		// blank
	}
}

void ACraftingStarCharacter::MouseLeftPressed() {
	if ( nowAbility != EPlayerAbility::ENone ) {
		if ( nowAbility == EPlayerAbility::ETelekinesis && abilityReadyStatus ) {
			//CameraBoom->bUsePawnControlRotation = false; // Does not rotate the arm based on the controller
		
			// Sound
			audioComp->SetSound(SW_Telekinesis);
			audioComp->Play();

			//GetCharacterMovement()->bOrientRotationToMovement = false; // Character doesn't moves in the direction of input...
			
			if ( abilityReadyStatus ) {
				switch ( HasAuthority() ) {
				case true:
					MulticastSetKeepAbility(true);
					break;
				case false:
					ServerSetKeepAbility(true);
					break;
				}

				if ( AbilityMontage ) {
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
		if ( nowAbility == EPlayerAbility::ETelekinesis && KeepAbility ) {
			//CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

			// Sound
			audioComp->Stop();
			audioComp->SetSound(NULL);

			if ( abilityReadyStatus ) {
				WandReadySign = false;
				GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("4"));
				switch ( HasAuthority() ) {
				case true:
					MulticastSetKeepAbility(false);
					break;
				case false:
					ServerSetKeepAbility(false);
					break;
				}

				if ( selectedTarget ) {
					// Change Tele' Interactive Actor's Color
					if ( Cast<ATelekinesisInteractableObject>(selectedTarget->GetOwner()) )
					{
						GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("here"));
						switch ( HasAuthority() ) {
						case true :
							PhysicsHandle->ReleaseComponent();
							if ( Cast<ATelekinesisInteractableObject>(selectedTarget->GetOwner()) )
							{
								selectedTarget->SetSimulatePhysics(false);
								if ( Cast<ATelekinesisInteractableObject>(selectedTarget->GetOwner())->isPhysicsObj ) {
									selectedTarget->SetSimulatePhysics(true);
								}
							}

							// Set CustomDepth Stencil Value to chagne Color
							Cast<ATelekinesisInteractableObject>(selectedTarget->GetOwner())->ActorMesh->SetCustomDepthStencilValue(0);

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
					ServerAbility(false);	// request ability animation on server
					// Play Animation
					GetMesh()->GetAnimInstance()->Montage_IsPlaying(DeactiveAbilityMontage);
				}

				teleLaserDistance = 750;
				teleComponentDistance = 0;
				//teleForce = 5000.0f;

				//GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
				
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

	// Set Simulate Physics to false
	if ( selectedTarget ) {
		if ( Cast<ATelekinesisInteractableObject>(selectedTarget->GetOwner()) )
		{
			selectedTarget->SetSimulatePhysics(false);
			if ( Cast<ATelekinesisInteractableObject>(selectedTarget->GetOwner())->isPhysicsObj ) {
				selectedTarget->SetSimulatePhysics(true);
			}
		}
	}

	// Set CustomDepth Stencil Value to chagne Color
	Cast<ATelekinesisInteractableObject>(selectedTarget->GetOwner())->ActorMesh->SetCustomDepthStencilValue(0);
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

void  ACraftingStarCharacter::LookUp(float Value) {
	//GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("LookUpValue: %f") , Value));
	// Add Pitch Input Value of Controller

	if ( nowAbility == EPlayerAbility::ETelekinesis && KeepAbility ) {
		if ( Value != 0.0f && CameraBoom ) {
			
			//float CameraBoomYaw = CameraBoom->GetComponentRotation().Yaw;

			float CurrentPitch = Cast<ACraftingStarPC>(GetController())->GetControlRotation().Pitch;

			//APlayerController* const PC = Cast<APlayerController>(Controller);
			//float CurrentYaw = PC->RotationInput.Yaw;

			//GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("PC Rot: %f %f %f") , Cast<ACraftingStarPC>(GetController())->GetControlRotation().Pitch , Cast<ACraftingStarPC>(GetController())->GetControlRotation().Yaw , Cast<ACraftingStarPC>(GetController())->GetControlRotation().Roll));
			/*
			if ( CameraBoomYaw >= CameraBoomMinPitch && CameraBoomYaw <= CameraBoomMaxPitch ) {
				//Cast<ACraftingStarPC>(GetController())->AddPitchInput(Value);
				//RotationInput.Pitch += !IsLookInputIgnored() ? Val * InputPitchScale : 0.f;
				GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("if문 진입")));
				AddControllerPitchInput(Value);

			}
			*/

			float NewPitch = 0.f;
			if ( /*0 <= CurrentPitch && */CurrentPitch <= -CameraBoomMinPitch ) {
				GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("1")));
				NewPitch = FMath::Clamp(CurrentPitch - Value , 0.f , -CameraBoomMinPitch);

				if ( CurrentPitch <= 0 ) {
					NewPitch = 360.f - Value;
				}
			}
			else if ( 360 - CameraBoomMaxPitch <= CurrentPitch/* && CurrentPitch < 360*/ ) {
				NewPitch = FMath::Clamp(CurrentPitch - Value , 360 - CameraBoomMaxPitch , 360.f);
				GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("%f") , NewPitch));

				if ( 360 <= CurrentPitch ) {
					NewPitch = -Value;
				}
			}/*
			else {
				if ( -CameraBoomMinPitch < CurrentPitch && CurrentPitch <= 180 ) {
					GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("3")));
					NewPitch = -CameraBoomMinPitch;			// Apply to Equal to or Less than 180 (excluded 0~CameraBoomMinPitch)
				}
				else if ( 180 < CurrentPitch && CurrentPitch < 360 - CameraBoomMaxPitch ) {
					GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("4")));
					NewPitch = 360.f - CameraBoomMaxPitch;	// Apply to Greater than 180 (excluded CameraBoomMaxPitch~360)
				}
			}*/
			Cast<ACraftingStarPC>(GetController())->SetControlRotation(FRotator(NewPitch , Cast<ACraftingStarPC>(GetController())->GetControlRotation().Yaw , Cast<ACraftingStarPC>(GetController())->GetControlRotation().Roll));
			
			/*
			float NewPitch = CurrentPitch + Value;

			NewPitch = FMath::Clamp(NewPitch , CameraBoomMinPitch , CameraBoomMaxPitch);

			CameraBoom->SetRelativeRotation(FRotator(NewPitch , CameraBoom->GetRelativeRotation().Yaw , CameraBoom->GetRelativeRotation().Roll));
			*/
		}
	}
	else {
		AddControllerPitchInput(Value);
	}
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

//서버에게 각각 플레이어 데이터 적용해달라고 요청
void ACraftingStarCharacter::LoadSaveData(bool isHost)
{
	UCraftingStarGameInstance* gameInstance = Cast<UCraftingStarGameInstance>(GetGameInstance());
    if ( gameInstance == nullptr ) return;


	
	if ( (gameInstance->nowSaveGame == nullptr || gameInstance->isDebug) && isHost) {
		//디버그 모드 표시
		gameInstance->SetDebugFile();
		gameInstance->isDebug = true;
	}
	
	ACraftingStarGS* gameState = Cast<ACraftingStarGS>(GetWorld()->GetGameState());

	if (isHost ) {
		//호스트일 경우 진행도 데이터도 저장
		//여기 부분에서 클라이언트 나갈 수도 있음. 주의깊게 보기
		gameState->ProgressData = gameInstance->nowSaveGame->ProgressData;
	}

	//플레이어 위치 적용
	FTransform transform = ( isHost ) ? gameInstance->nowSaveGame->ProgressData.HostPlayerPos : gameInstance->nowSaveGame->ProgressData.GuestPlayerPos;
	GetCapsuleComponent()->SetWorldTransform(transform);
	
	////플레이어 위치에 제일 가까운 플레이어 스타트에 위치
	//if ( UWorld* World = GetWorld() )
	//{
	//	if ( ACraftingStarGameMode* GameMode = Cast<ACraftingStarGameMode>(World->GetAuthGameMode()) )
	//	{
	//		GameMode->RespawnPlayer(this);
	//	}
	//}

	if ( isHost ) {
		FVector pos = transform.GetLocation();
		GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("Host transform %f / %f / %f") ,pos.X,pos.Y,pos.Z ));
	}
	else {
		FVector pos = transform.GetLocation();
		GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("Guest transform %f / %f / %f") , pos.X , pos.Y , pos.Z));
	}
	
	if ( isHost )gameState->isHostInit = true;
	else gameState->isGuestInit = true;

}

void ACraftingStarCharacter::PlayerOutfit_Implementation(FPlayerData hostData , FPlayerData guestData)
{
	//gamestate - player 순회 후
	//player state mode 에 맞는 메시를 player state owner인 캐릭터에 적용시켜주기
	//딜레이 조심
	ACraftingStarGS* gameState = Cast<ACraftingStarGS>(GetWorld()->GetGameState());
	if ( gameState == nullptr )return;

	
	for(auto& playerPS : gameState->PlayerArray )
	{
		ACraftingStarPS* playerState = Cast<ACraftingStarPS>(playerPS);
		if ( playerState == nullptr ) {
			GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("playerState is nullptr"));

			continue;

		}

		ACraftingStarPC* playerController = Cast<ACraftingStarPC>(playerState->GetOwner());
		if ( playerController == nullptr ) {
			playerState->PlayerData = hostData;
		}
		else {
			if ( UUtilityFunction::IsHost( playerController )) {
				playerState->PlayerData = hostData;
			}
			else {
				playerState->PlayerData = guestData;
			}
		}

		ACraftingStarCharacter* playerCharacter = Cast<ACraftingStarCharacter>(playerState->GetPawn());
		if ( playerCharacter == nullptr ) {
			GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("playerCharacter is nullptr"));
			continue;
		}
		//호스트 데이터 적용
		//지금 플레이어 캐릭터가 host인지
		

		FString EnumToString = TEXT("Invalid");
		const UEnum* SEnum = FindObject<UEnum>(ANY_PACKAGE , TEXT("EPlayerRole") , true);
		if ( SEnum )
		{
			EnumToString = SEnum->GetNameStringByValue((int64)playerState->PlayerData.Mode);
		}
		if ( playerCharacter->HasAuthority() ) {
			
			GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("Host Role %s") , *EnumToString));
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , FString::Printf(TEXT("Guest Role %s") , *EnumToString));
		}


		if ( playerState->PlayerData.Mode == EPlayerRole::ELight ) {
			//라이트 메시 적용

			//캐릭터 본
			playerCharacter->GetMesh()->SetSkeletalMesh(BodyMesh_Light);
			playerCharacter->GetMesh()->SetMaterial(0 , BodyMat_Light);
			//머리
			playerCharacter->HeadMesh->SetStaticMesh(HeadMesh_Light);
			//헤어
			playerCharacter->HairAndHatMesh->SetStaticMesh(HairAndHatMesh_Light);
			playerCharacter->HairAndHatMesh->SetMaterial(0 , HairMat_Light);
			//눈
			playerCharacter->EyesMesh->SetStaticMesh(EyesMesh_Light);
			playerCharacter->HairAndHatMesh->SetMaterial(0 , EyesMat_Light);
			//입
			playerCharacter->MouthMesh->SetStaticMesh(MouthMesh_Light);
			//망토
			playerCharacter->CloakMesh->SetSkeletalMesh(CloakMesh_Light);
			playerCharacter->CloakMesh->SetMaterial(0 , CloakMat_Light);

			//레이저 
			playerCharacter->Comp_LaserNiagara->SystemChange(true);
			
		}
		else if ( playerState->PlayerData.Mode == EPlayerRole::EDark){
		    //다크 메시적용
			//캐릭터 본
			playerCharacter->GetMesh()->SetSkeletalMesh(BodyMesh_Dark);
			playerCharacter->GetMesh()->SetMaterial(0 , BodyMat_Dark);
			//머리
			playerCharacter->HeadMesh->SetStaticMesh(HeadMesh_Dark);
			//헤어
			
			playerCharacter->HairAndHatMesh->SetStaticMesh(HairAndHatMesh_Dark);

			if ( HairMat_Dark == nullptr ) {
				GEngine->AddOnScreenDebugMessage(-1 , 3 , FColor::Red , TEXT("HairMat_Dark is nullptr"));
			}

			playerCharacter->HairAndHatMesh->SetMaterial(0 , HairMat_Dark);
			//눈
			playerCharacter->EyesMesh->SetStaticMesh(EyesMesh_Dark);
			//playerCharacter->HairAndHatMesh->SetMaterial(0 , EyesMat_Dark);
			//입
			playerCharacter->MouthMesh->SetStaticMesh(MouthMesh_Dark);
			//망토
			playerCharacter->CloakMesh->SetSkeletalMesh(CloakMesh_Dark);
			playerCharacter->CloakMesh->SetMaterial(0 , CloakMat_Dark);

			//레이저 
			playerCharacter->Comp_LaserNiagara->SystemChange(false);
		}
	}

}

void ACraftingStarCharacter::PlayerUIInit_Implementation()
{
	if ( HasAuthority() ) {
		//authority
		if ( LoadingWidgetRef != NULL ) {
			LoadingWidgetRef->RemoveFromParent();
			LoadingWidgetRef = NULL;
		}
	}
	else {
		//remote
		ACraftingStarCharacter* PlayerCharacter = Cast<ACraftingStarCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld() , 0));
		if ( PlayerCharacter != nullptr && PlayerCharacter->LoadingWidgetRef != NULL ) {
			PlayerCharacter->LoadingWidgetRef->RemoveFromParent();
			PlayerCharacter->LoadingWidgetRef = NULL;
		}
	}

	SetPause(false);
}

void ACraftingStarCharacter::SetInteractionFlag_Implementation(bool isHost , bool isInteraction) {
	//authority(서버)
	//remote(서버)
	//여기서 뭔가 조작하면 될 것 같은데

	ACraftingStarCharacter* targetPlayer = this;
	ACraftingStarGS* gameState = Cast<ACraftingStarGS>(GetWorld()->GetGameState());
	if ( gameState == nullptr ) return;

	if ( UUtilityFunction::IsHost(GetController())) {
		//호스트
		
		for ( APlayerState* playerState : gameState->PlayerArray ) {
			AController* controller = Cast<AController>(playerState->GetOwner());
			if ( controller == nullptr ) return;
			
			if ( UUtilityFunction::IsHost(controller) && isHost ) {
				//호스트 쪽의 호스트 캐릭터
				targetPlayer = Cast<ACraftingStarCharacter>(playerState->GetPawn());
				break;
			}
			else if(!UUtilityFunction::IsHost(controller) && !isHost ) {
				//호스트 쪽의 게스트 캐릭터
				targetPlayer = Cast<ACraftingStarCharacter>(playerState->GetPawn());
				break;
			}

		}
	}
	else {
		//게스트
		for ( APlayerState* playerState : gameState->PlayerArray ) {
			AController* controller = Cast<AController>(playerState->GetOwner());
			
			if ( controller == nullptr && isHost ) {
				//게스트 쪽의 호스트 캐릭터
				targetPlayer = Cast<ACraftingStarCharacter>(playerState->GetPawn());
				break;
			}
			else if ( controller != nullptr && !isHost ) {
				//게스트 쪽의 게스트 캐릭터
				targetPlayer = Cast<ACraftingStarCharacter>(playerState->GetPawn());
				break;
			}
		}
	}
	
	if ( targetPlayer == nullptr ) return;
	targetPlayer->interactTag->SetVisibility(isInteraction);
}

void ACraftingStarCharacter::ServerObtainAbility_Implementation(EPlayerAbility ability) {
	//GetGameState()->
	Cast<ACraftingStarGS>(GetWorld()->GetGameState())->nowObtainingAbility = ability;
	
	MulticastObtainAbility(ability);
}


void ACraftingStarCharacter::MulticastObtainAbility_Implementation(EPlayerAbility ability) {
	//SetPause(true);

	if ( PaletteObtaingAnimationRef == NULL ) {
		
	/*	FString EnumToString = TEXT("Invalid");
		const UEnum* SEnum = FindObject<UEnum>(ANY_PACKAGE , TEXT("EPlayerAbility") , true);
		if ( SEnum )
		{
			EnumToString =  SEnum->GetNameStringByValue((int64)ability);
		}



		GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green ,EnumToString );*/

		PaletteObtaingAnimationRef = CreateWidget(GetWorld() , PaletteObtaingAnimationWidget);
		PaletteObtaingAnimationRef->AddToViewport();
		PaletteObtaingAnimationRef = NULL;
		
	}
}

void ACraftingStarCharacter::ServerObtainAbilityComplete_Implementation(bool isHost) {
	//GetGameState()->

	if ( isHost ) {
		Cast<ACraftingStarGS>(GetWorld()->GetGameState())->isHostObtain = true;
	}
	else {
		Cast<ACraftingStarGS>(GetWorld()->GetGameState())->isGuestObtain = true;
	}
	
	
}