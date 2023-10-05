// Copyright Epic Games, Inc. All Rights Reserved.

#include "CraftingStarCharacter.h"
#include "CraftingStarPC.h"
#include "CraftingStarPS.h"
#include "CustomEnum.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// ACraftingStarCharacter

ACraftingStarCharacter::ACraftingStarCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

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
}

//////////////////////////////////////////////////////////////////////////
// Input
// Called when the game starts or when spawned
void ACraftingStarCharacter::BeginPlay()
{
	Super::BeginPlay();
	//GameWB 생성
	CreateWidget(GetWorld(), GameWidget)->AddToViewport();

	//플레이어 스테이트가 생성되는데는 1~2초정도 걸린다
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

	//팔레트
	PlayerInputComponent->BindAction("Palette", IE_Pressed, this, &ACraftingStarCharacter::Palette);
	PlayerInputComponent->BindAction("Palette", IE_Released, this, &ACraftingStarCharacter::StopPalette);
	//월드맵
	PlayerInputComponent->BindAction("WorldMap", IE_Pressed, this, &ACraftingStarCharacter::WorldMap);
	PlayerInputComponent->BindAction("WorldMap", IE_Released, this, &ACraftingStarCharacter::StopWorldMap);

	//상호작용
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &ACraftingStarCharacter::Interaction);
}

void ACraftingStarCharacter::Palette() {
	//타이머 실행 (0.1초당 1회씩 함수 호출)
	GetWorldTimerManager().SetTimer(HoldTimerHandle, this, &ACraftingStarCharacter::RepeatingFunction, 0.1f, true);
}

void ACraftingStarCharacter::StopPalette() {
	//만약 아직 타이머가 유효하면 타이머 리셋
	if (GetWorldTimerManager().IsTimerActive(HoldTimerHandle)) {
		GetWorldTimerManager().ClearTimer(HoldTimerHandle);
		PaletteCnt = 0.0f;
	}

	//만약 능력 준비고 팔레트가 생성되지 않았으면
	if (PaletteWidgetRef == NULL && Cast<ACraftingStarPS>(GetPlayerState())->NowState == EPlayerGMState::EAbilityReady) {
		//기본상태로 돌아가고 바로 종료
		Cast<ACraftingStarPS>(GetPlayerState())->NowState = EPlayerGMState::EIdle;
		return;
	}

	//상태 능력 준비모드로 전환
	if (Cast<ACraftingStarPS>(GetPlayerState())->NowAbility != EPlayerAbility::ENone) Cast<ACraftingStarPS>(GetPlayerState())->NowState = EPlayerGMState::EAbilityReady;

	//만약 팔레트 UI가 생성됐다면 
	if (PaletteWidgetRef != NULL) {
		//팔레트 삭제
		PaletteWidgetRef->RemoveFromParent();
		PaletteWidgetRef = NULL;

		//키보드 입력 가능 & 마우스 포인터 비활성화
		SetPause(false);
	}

}

void ACraftingStarCharacter::RepeatingFunction() {
	
	//UE_LOG(LogTemp, Log, TEXT("GetTimeElapsed : %f"), PaletteCnt);
	if (PaletteCnt >= 0.5f) {
		//0.5초 이상 홀드하면 팔레트 생성
		PaletteCnt = 0.0f;
		GetWorldTimerManager().ClearTimer(HoldTimerHandle);		
		PaletteWidgetRef = CreateWidget(GetWorld(), PaletteWidget);
		PaletteWidgetRef->AddToViewport();

		//키보드 입력 정지 & 마우스 포인터 활성화
		SetPause(true);

		return;
	}
	PaletteCnt += 0.1f;
}

void ACraftingStarCharacter::WorldMap() {
	//월드맵 생성
	WorldMapWidgetRef = CreateWidget(GetWorld(), WorldMapWidget);
	WorldMapWidgetRef->AddToViewport();
	SetPause(true);
}

void ACraftingStarCharacter::StopWorldMap() {
	//만약 팔레트 UI가 생성됐다면 
	if (WorldMapWidgetRef != NULL) {
		//팔레트 삭제
		WorldMapWidgetRef->RemoveFromParent();
		WorldMapWidgetRef = NULL;
		SetPause(false);
	}
}

void ACraftingStarCharacter::Interaction() {

}

void  ACraftingStarCharacter::SetPause(bool isPaused) {
	if (isPaused) {
		//키보드 입력 정지 & 마우스 포인터 활성화
		Cast<ACraftingStarPC>(GetController())->SetInputMode(FInputModeGameAndUI());
		DisableInput(Cast<ACraftingStarPC>(GetController()));
		Cast<ACraftingStarPC>(GetController())->SetShowMouseCursor(true);
	}
	else {
		//키보드 입력 가능 & 마우스 포인터 비활성화
		Cast<ACraftingStarPC>(GetController())->SetInputMode(FInputModeGameOnly());
		EnableInput(Cast<ACraftingStarPC>(GetController()));
		Cast<ACraftingStarPC>(GetController())->SetShowMouseCursor(false);
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
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACraftingStarCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACraftingStarCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
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
	if ( (Controller != nullptr) && (Value != 0.0f) )
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
