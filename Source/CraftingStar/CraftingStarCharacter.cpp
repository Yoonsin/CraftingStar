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


//////////////////////////////////////////////////////////////////////////
// ACraftingStarCharacter

ACraftingStarCharacter::ACraftingStarCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 75.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f,  540.0f, 0.0f); // ...at this rotation rate
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
	
	HairAndHatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HairAndHatMesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> HairAndHatSM(TEXT("StaticMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/HeadPart/Hair08_SM.Hair08_SM'"));
	if ( HairAndHatSM.Succeeded() ) {
		HairAndHatMesh->SetStaticMesh(HairAndHatSM.Object);
	}
	HairAndHatMesh->SetupAttachment(GetMesh(), FName(TEXT("Head")));
	HairAndHatMesh->SetRelativeLocation(FVector(0.0f , 0.0f , 0.0f));
	HairAndHatMesh->SetRelativeRotation(FRotator(-90.0f , 0.0f , 0.0f));

	EyesMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Eyes"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> EyesSM(TEXT("StaticMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/HeadPart/Eye08_SM.Eye08_SM'"));
	if ( EyesSM.Succeeded() ) {
		EyesMesh->SetStaticMesh(EyesSM.Object);
	}
	EyesMesh->SetupAttachment(GetMesh(), FName(TEXT("Head")));
	EyesMesh->SetRelativeLocation(FVector(0.0f , 0.0f , 0.0f));
	EyesMesh->SetRelativeRotation(FRotator(-90.0f , 0.0f , 0.0f));

	MouthMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mouth"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MouthSM(TEXT("StaticMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/HeadPart/Mouth02_SM.Mouth02_SM'"));
	if ( MouthSM.Succeeded() ) {
		MouthMesh->SetStaticMesh(MouthSM.Object);
	}
	MouthMesh->SetupAttachment(GetMesh(), FName(TEXT("Head")));
	MouthMesh->SetRelativeLocation(FVector(0.0f , 0.0f , 0.0f));
	MouthMesh->SetRelativeRotation(FRotator(-90.0f , 0.0f , 0.0f));

	CloakMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Cloak"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> CloakSM(TEXT("SkeletalMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/BodyPart/Cloak02_SK.Cloak02_SK'"));
	if ( CloakSM.Succeeded() ) {
		CloakMesh->SetSkeletalMesh(CloakSM.Object);
	}
	CloakMesh->SetupAttachment(GetMesh(), FName(TEXT("CloakBone02")));
	CloakMesh->SetRelativeLocation(FVector(40.0f , 20.0f , 0.0f));
	CloakMesh->SetRelativeRotation(FRotator(90.0f , 0.0f , 0.0f));

	Weapon_rMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon_R"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> WandSM(TEXT("StaticMesh'/Game/Assets/BaseContent/RPGTinyHeroWavePolyart/Mesh/Weapon/Wand04_SM.Wand04_SM'"));
	if ( WandSM.Succeeded() ) {
		Weapon_rMesh->SetStaticMesh(WandSM.Object);
	}
	Weapon_rMesh->SetupAttachment(GetMesh(), FName(TEXT("Weapon_R")));
	Weapon_rMesh->SetRelativeLocation(FVector(0.0f , 0.0f , 0.0f));
	Weapon_rMesh->SetRelativeRotation(FRotator(0.0f , 0.0f , 0.0f));

	// Magic Wand LineTrace Start Point
	SpawnLocSource = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnLoc Source"));
	SpawnLocSource->SetupAttachment(Weapon_rMesh, FName(TEXT("SpawnLoc")));

	// Ability: Laser Niagara System
	LaserBody = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Laser Body"));
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> LaserBodyAsset(TEXT("NiagaraSystem'/Game/Assets/Effects/Laser/NS_Laser.NS_Laser'"));
	if ( LaserBodyAsset.Succeeded() ) {
		LaserBody->SetAsset(LaserBodyAsset.Object);
	}
	LaserBody->SetupAttachment(Weapon_rMesh , FName(TEXT("SpawnLoc")));

	LaserImpact = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Laser Impact"));
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> LaserImpactAsset(TEXT("NiagaraSystem'/Game/Assets/Effects/Laser/NS_LaserImpact.NS_LaserImpact'"));
	if ( LaserBodyAsset.Succeeded() ) {
		LaserImpact->SetAsset(LaserImpactAsset.Object);
	}
	LaserImpact->SetupAttachment(Weapon_rMesh , FName(TEXT("SpawnLoc")));

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

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
	
	//GameWB     
	CreateWidget(GetWorld(), GameWidget)->AddToViewport();

	LaserBody->SetVisibility(false);
	LaserImpact->SetVisibility(false);
}

void ACraftingStarCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACraftingStarCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACraftingStarCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACraftingStarCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACraftingStarCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACraftingStarCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACraftingStarCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ACraftingStarCharacter::OnResetVR);

	// ȷ Ʈ
	PlayerInputComponent->BindAction("Palette", IE_Pressed, this, &ACraftingStarCharacter::Palette);
	PlayerInputComponent->BindAction("Palette", IE_Released, this, &ACraftingStarCharacter::StopPalette);
	//     
	PlayerInputComponent->BindAction("WorldMap", IE_Pressed, this, &ACraftingStarCharacter::WorldMap);
	PlayerInputComponent->BindAction("WorldMap", IE_Released, this, &ACraftingStarCharacter::StopWorldMap);

	PlayerInputComponent->BindAction("SystemMenu", IE_Pressed, this, &ACraftingStarCharacter::SystemMenu);
	PlayerInputComponent->BindAction("SystemMenu", IE_Released, this, &ACraftingStarCharacter::StopSystemMenu);

	//  ȣ ۿ 
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &ACraftingStarCharacter::Interaction);

	// for using Ability. key: E.
	PlayerInputComponent->BindAction("Ability", IE_Pressed, this, &ACraftingStarCharacter::ActivateAbility);
	PlayerInputComponent->BindAction("Ability", IE_Released, this, &ACraftingStarCharacter::DeactivateAbility);
}


void ACraftingStarCharacter::Tick(float DeltaTime)
{
	//          Ӹ          ɷ              ʿ        Ʈ
	Super::Tick(DeltaTime);

	if ( KeepAbility ) {
		// Activate Ability
		EPlayerAbility nowAbility = Cast<ACraftingStarPS>(GetPlayerState())->NowAbility;
		if ( nowAbility != EPlayerAbility::ENone ) {
			// Laser(EBlast)
			if ( nowAbility == EPlayerAbility::EBlast ) {
				// 테스트
				// Activate Laser
				//GEngine->AddOnScreenDebugMessage(-1 , 3.0f , FColor::Green , TEXT("Activate Laser"));
				// Execute Laser
				ACraftingStarCharacter::WandLineTrace(10000);

			}
		}
	}

}

void ACraftingStarCharacter::UpdatePlayerAbility(EPlayerAbility playerAbility) {

	auto playerState = Cast<ACraftingStarPS>(GetPlayerState());

	if (playerState != nullptr) {
		playerState->NowAbility = playerAbility;
		playerState->RequestPlayerAbility(playerAbility);
	}
		
	  
}

void ACraftingStarCharacter::UpdatePlayerGMState(EPlayerGMState playerGMState) {

	auto playerState = Cast<ACraftingStarPS>(GetPlayerState());

	if (playerState != nullptr) {
		playerState->NowState = playerGMState;
		playerState->RequestPlayerGMState(playerGMState);
	}
		

}


void ACraftingStarCharacter::Palette() {
	//Ÿ ̸       (0.1 ʴ  1ȸ    Լ  ȣ  )
	GetWorldTimerManager().SetTimer(HoldTimerHandle, this, &ACraftingStarCharacter::RepeatingFunction, 0.01f, true);
}

void ACraftingStarCharacter::StopPalette() {
	//          Ÿ ̸Ӱ    ȿ ϸ  Ÿ ̸      
	if (GetWorldTimerManager().IsTimerActive(HoldTimerHandle)) {
		GetWorldTimerManager().ClearTimer(HoldTimerHandle);
		PaletteCnt = 0.0f;
	}

	//      ɷ   غ    ȷ Ʈ             ʾ     
	if (PaletteWidgetRef == NULL && GetPlayerState() != nullptr && Cast<ACraftingStarPS>(GetPlayerState())->NowState == EPlayerGMState::EAbilityReady) {
		// ⺻   ·     ư     ٷ      
		auto controller = GetController();
		UpdatePlayerGMState(EPlayerGMState::EIdle);
		//Cast<ACraftingStarPS>(GetPlayerState())->NowState = EPlayerGMState::EIdle;
		
		return;
	}

	//      ɷ   غ       ȯ
	if (GetPlayerState() != nullptr && Cast<ACraftingStarPS>(GetPlayerState())->NowAbility != EPlayerAbility::ENone) UpdatePlayerGMState(EPlayerGMState::EAbilityReady);

	//      ȷ Ʈ UI        ƴٸ  
	if (PaletteWidgetRef != NULL) {
		// ȷ Ʈ     
		PaletteWidgetRef->RemoveFromParent();
		PaletteWidgetRef = NULL;

		//Ű      Է       &    콺          Ȱ  ȭ
		SetPause(false);
	}

}

void ACraftingStarCharacter::RepeatingFunction() {
	
	//UE_LOG(LogTemp, Log, TEXT("GetTimeElapsed : %f"), PaletteCnt);
	if (PaletteCnt >= 0.05f) {
		//0.2    ̻  Ȧ   ϸ   ȷ Ʈ     
		PaletteCnt = 0.0f;
		GetWorldTimerManager().ClearTimer(HoldTimerHandle);		

		if (PaletteWidgetRef == NULL) {
			PaletteWidgetRef = CreateWidget(GetWorld(), PaletteWidget);
			PaletteWidgetRef->AddToViewport();
		}
		
		//Ű      Է       &    콺        Ȱ  ȭ
		SetPause(true);
		return;
	}
	PaletteCnt += 0.01f;
}

void ACraftingStarCharacter::WorldMap() {
	if (WorldMapWidgetRef == NULL) {
		WorldMapWidgetRef = CreateWidget(GetWorld(), WorldMapWidget);
		WorldMapWidgetRef->AddToViewport();
		SetPause(true);
	}
	
}

void ACraftingStarCharacter::StopWorldMap() {
	//      ȷ Ʈ UI        ƴٸ  
	if (WorldMapWidgetRef != NULL) {
		// ȷ Ʈ     
		WorldMapWidgetRef->RemoveFromParent();
		WorldMapWidgetRef = NULL;
		SetPause(false);
	}
}

void ACraftingStarCharacter::SystemMenu() {
	if (SystemMenuWidgetRef == NULL) {
		SystemMenuWidgetRef = CreateWidget(GetWorld(), SystemMenuWidget);
		SystemMenuWidgetRef->AddToViewport();
		SetPause(true);
	}
}

void ACraftingStarCharacter::StopSystemMenu() {
	if (SystemMenuWidgetRef != NULL) {
		// ȷ Ʈ     
		SystemMenuWidgetRef->RemoveFromParent();
		SystemMenuWidgetRef = NULL;
		SetPause(false);
	}
}


void ACraftingStarCharacter::Interaction() {

}

// Magic Wand Line Trace for Ability
bool ACraftingStarCharacter::WandLineTrace(float distance) const {

	/* Set LineTrace */

	// Result oof LineTrace
	FHitResult Hit;

	// Ability Spawn Loc Socket Transform
	FVector SpawnLocation = this->Weapon_rMesh->GetSocketLocation(FName("SpawnLoc"));
	// Start point and End point of LineTrace
	FVector Start = SpawnLocation;
	FVector End = SpawnLocation + ( GetActorForwardVector() * distance );

	// Trace Channel: Custom Trace Channel - AbilitySpawn
	ECollisionChannel Channel = ECollisionChannel::ECC_GameTraceChannel1;
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	/* Execute LineTrace */
	GetWorld()->LineTraceSingleByChannel(Hit , Start , End , Channel, QueryParams);
	// Visualize LineTrace
	DrawDebugLine(GetWorld() , Start , End , FColor::Green);

	// Set the End of Laser Body
	if ( Hit.bBlockingHit ) {
		LaserBody->SetVectorParameter(FName(TEXT("LaserEnd")) , Hit.Location);
	}
	else {
		LaserBody->SetVectorParameter(FName(TEXT("LaserEnd")) , End);
	}
	
	// Show Laser
	LaserBody->SetVisibility(true);
	LaserImpact->SetVisibility(Hit.bBlockingHit);
	LaserImpact->SetWorldLocation(Hit.Location);

	return !Hit.bBlockingHit;
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
	}
	else {
		GetMesh()->GetAnimInstance()->Montage_Play(DeactiveAbilityMontage , 1.0f);
	}
}

// Input Ability
void ACraftingStarCharacter::ActivateAbility() {
	KeepAbility = true;
	if ( AbilityMontage ) {
		// Play Animation
		bool bIsMontagePlaying = GetMesh()->GetAnimInstance()->Montage_IsPlaying(AbilityMontage);
		if ( !bIsMontagePlaying ) {
			ServerAbility(true);	// request ability animation on server
		}
	}
}
void ACraftingStarCharacter::DeactivateAbility() {
	KeepAbility = false;
	if (DeactiveAbilityMontage) {
		// Play Animation
		bool bIsMontagePlaying = GetMesh()->GetAnimInstance()->Montage_IsPlaying(DeactiveAbilityMontage);
		if (!bIsMontagePlaying) {
			ServerAbility(false);	// request ability animation on server
		}
		// Activate Ability
		EPlayerAbility nowAbility = Cast<ACraftingStarPS>(GetPlayerState())->NowAbility;
		if (nowAbility != EPlayerAbility::ENone) {
			// Laser(EBlast)
			if (nowAbility == EPlayerAbility::EBlast) {
				// Deactivate Laser
				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Deactivate Laser"));
				// Hid Laser
				LaserBody->SetVisibility(false);
				LaserImpact->SetVisibility(false);
			}
		}
	}
}

void  ACraftingStarCharacter::SetPause(bool isPaused) {
	if (Cast<ACraftingStarPC>(GetController()) != nullptr) {
		if (isPaused) {
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

void ACraftingStarCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ACraftingStarCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ACraftingStarCharacter::TurnAtRate(float Rate)
{
	if ( !KeepAbility ) {
		// calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void ACraftingStarCharacter::LookUpAtRate(float Rate)
{
	if ( !KeepAbility ) {
		// calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void ACraftingStarCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && !KeepAbility)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACraftingStarCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) && !KeepAbility )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


//게임 플레이도중 플레이어가 사라짐
void ACraftingStarCharacter::Destroyed()
{
	Super::Destroyed();

	//게임 모드에서 OnPlayerDied 이벤트에 바인딩한 예 
	if (UWorld* World = GetWorld())
	{
		if (ACraftingStarGameMode* GameMode = Cast<ACraftingStarGameMode>(World->GetAuthGameMode()))
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
	if (UWorld* World = GetWorld())
	{
		if (ACraftingStarGameMode* GameMode = Cast<ACraftingStarGameMode>(World->GetAuthGameMode()))
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