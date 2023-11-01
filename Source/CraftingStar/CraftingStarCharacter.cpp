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

	//  ȣ ۿ 
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &ACraftingStarCharacter::Interaction);
}


void ACraftingStarCharacter::Tick(float DeltaTime)
{
	//          Ӹ          ɷ              ʿ        Ʈ
	Super::Tick(DeltaTime);

}

void ACraftingStarCharacter::UpdatePlayerAbility(EPlayerAbility playerAbility) {

	auto playerState = Cast<ACraftingStarPS>(GetPlayerState());

	if (playerState != nullptr)
		playerState->RequestPlayerAbility(playerAbility);
	  
}

void ACraftingStarCharacter::UpdatePlayerGMState(EPlayerGMState playerGMState) {

	auto playerState = Cast<ACraftingStarPS>(GetPlayerState());

	if (playerState != nullptr)
		playerState->RequestPlayerGMState(playerGMState);

}


void ACraftingStarCharacter::Palette() {
	//Ÿ ̸       (0.1 ʴ  1ȸ    Լ  ȣ  )
	GetWorldTimerManager().SetTimer(HoldTimerHandle, this, &ACraftingStarCharacter::RepeatingFunction, 0.1f, true);
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
	if (PaletteCnt >= 0.2f) {
		//0.2    ̻  Ȧ   ϸ   ȷ Ʈ     
		PaletteCnt = 0.0f;
		GetWorldTimerManager().ClearTimer(HoldTimerHandle);		
		PaletteWidgetRef = CreateWidget(GetWorld(), PaletteWidget);
		PaletteWidgetRef->AddToViewport();
		

		//Ű      Է       &    콺        Ȱ  ȭ
		SetPause(true);
		return;
	}
	PaletteCnt += 0.1f;
}

void ACraftingStarCharacter::WorldMap() {
	//          
	WorldMapWidgetRef = CreateWidget(GetWorld(), WorldMapWidget);
	WorldMapWidgetRef->AddToViewport();
	SetPause(true);
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

void ACraftingStarCharacter::Interaction() {

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
