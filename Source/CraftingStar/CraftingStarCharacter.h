// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "CustomStruct.h"
#include "CustomEnum.h"
#include "Net/UnrealNetwork.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GameFramework/PlayerController.h"
#include "CraftingStarCharacter.generated.h"

UCLASS(config=Game)
class ACraftingStarCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly , Category = "Physics")
	UPhysicsHandleComponent* PhysicsHandle;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = UI , meta = ( AllowPrivateAccess = "true" ))
    TSubclassOf<UUserWidget> LoadingWidget;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> PaletteWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> WorldMapWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> GameWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> SystemMenuWidget;

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = UI , meta = ( AllowPrivateAccess = "true" ))
	TSubclassOf<UUserWidget> LogOutClientWidget;

	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = UI , meta = ( AllowPrivateAccess = "true" ))
	TSubclassOf<UUserWidget> PaletteObtaingAnimationWidget;

	//�ȷ�Ʈ
	class UUserWidget* PaletteWidgetRef;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = UI , meta = ( AllowPrivateAccess = "true" ))
	class UUserWidget* PaletteObtaingAnimationRef;
	
	FTimerHandle HoldTimerHandle;
	float PaletteCnt;

	//�����
	class UUserWidget* WorldMapWidgetRef;
	class UUserWidget* SystemMenuWidgetRef;
	class UUserWidget* LoadingWidgetRef;

	// Telekinesis

	float teleLaserDistance = 750.0f;
	float teleComponentDistance = 0.0f;
	UPROPERTY(EditAnywhere , Category = "Forces")
	float teleForce = 1000.0f;

	void Telekinesis();
	void CreateTeleObjOutline();
	void RemoveTeleObjOutline();
	// Select Target
	UFUNCTION(Server , Reliable , WithValidation , Category = "Telekinesis")
	void ServerSelectTarget(FHitResult Hit);

	

	UFUNCTION(NetMulticast , Unreliable , Category = "Telekinesis")
	void MulticastSelectTarget(FHitResult Hit);
	// Deselect Target
	UFUNCTION(Server , Reliable , WithValidation , Category = "Telekinesis")
	void ServerDeselectTarget();
	UFUNCTION(NetMulticast , Unreliable , Category = "Telekinesis")
	void MulticastDeselectTarget();
	// Server Grab Component
	UFUNCTION(Server , Reliable , WithValidation , Category = "Telekinesis")
	void ServerGrabComponent(FVector End, FHitResult Hit);
	UFUNCTION(NetMulticast , Unreliable , Category = "Telekinesis")
	void MulticastGrabComponent(FVector End, FHitResult Hit);
	// Object Locate Replication
	UFUNCTION(Server , Reliable , WithValidation , Category = "Telekinesis")
	void ServerTeleObjLoc(FVector End);
	UFUNCTION(NetMulticast , Unreliable , Category = "Telekinesis")
	void MulticastTeleObjLoc(FVector End);
	// Server Release Component
	UFUNCTION(Server , Reliable , WithValidation , Category = "Telekinesis")
	void ServerReleaseComponent();
	UFUNCTION(NetMulticast , Unreliable , Category = "Telekinesis")
	void MulticastReleaseComponent();
	
public:
	ACraftingStarCharacter();
	class UUserWidget* LogOutClientWidgetRef;
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UFUNCTION(BlueprintCallable , Server , Reliable)
	void ServerObtainAbility(EPlayerAbility ability);
	// Replicate Maintain Wand Skill and control the pitch (vertical)
	UPROPERTY(Replicated , VisibleAnywhere , BlueprintReadOnly , Category = AimOffset)
	FRotator OffsetAxis;
	UFUNCTION(Server , Reliable , WithValidation , BlueprintCallable , Category = "AimOffset")
	void ServerSetOffsetAxis();
	UFUNCTION(NetMulticast , Unreliable , Category = "AimOffset")
	void MulticastSetOffsetAxis();
	UFUNCTION(BlueprintCallable)
	void SetOffsetAxis();
	UFUNCTION(BlueprintCallable)
	FRotator GetOffsetAxis();


	UFUNCTION(BlueprintCallable , Server , Reliable)
		void ServerObtainAbilityComplete(bool isHost);

	UFUNCTION(BlueprintCallable ,NetMulticast , Reliable )
	void MulticastObtainAbility(EPlayerAbility ability);

	UFUNCTION(BlueprintCallable , NetMulticast , Reliable)
	void PlayerOutfit(FPlayerData hostData , FPlayerData guestData);

	FTimerHandle delayTimerHandle;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	class UUserWidget* GameWidgetRef;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	class UWidgetComponent* interactTag;

	// On Damaged: Popo Attack Interaction
	UFUNCTION(BlueprintCallable)
	void OnDamaged_Popo();
	UFUNCTION(BlueprintCallable)
	void OnCollapsed_Popo();
	UFUNCTION(BlueprintCallable)
	void OnRevive_Popo();

	// On Damaged: Replicate Animation
	UFUNCTION(Server , Reliable , WithValidation , Category = "Animation")
	void ServerPlayOnDamagedMontage(UAnimMontage* animMontage);
	UFUNCTION(NetMulticast , Reliable , Category = "Animation")
	void MulticastPlayOnDamagedMontage(UAnimMontage* animMontage);
	
	// On Damaged: Attacked Count
	int AttackedCnt_Popo;

	UPROPERTY(Replicated , EditAnywhere , BlueprintReadWrite , Category = Popo , meta = ( AllowPrivateAccess = "true" ))
	bool isCollapsed;

	// Telekinesis
	UPrimitiveComponent* selectedTarget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
	void LookUp(float Value);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	//�ȷ�Ʈ
	void Palette(); 
	void RepeatingFunction();

	//�����
	
	void SystemMenu();

	//��ȣ�ۿ�
	void Interaction();

	// Mouse Events
	void MouseLeftPressed();
	void MouseLeftReleased();

	// Sounds
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Sounds , meta = ( AllowPrivateAccess = "true" ))
	class USoundWave* SW_EmissionDark;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Sounds , meta = ( AllowPrivateAccess = "true" ))
	class USoundWave* SW_EmissionLight;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Sounds , meta = ( AllowPrivateAccess = "true" ))
	class USoundWave* SW_Telekinesis;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Sounds , meta = ( AllowPrivateAccess = "true" ))
	class UAudioComponent* audioComp;

	// OnDamaged
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = AnimMontage)
	class UAnimMontage* CollapsedMontage_Popo;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = AnimMontage)
	class UAnimMontage* ReviveMontage_Popo;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = AnimMontage)
	class UAnimMontage* HitMontage_Popo;

	// Ability

	void ActivateAbility();
	void DeactivateAbility();
	void ActivateAbility2();

	// Wand Skill Animation: Blast, Telekinesis
	// Activate Wand Skill
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AnimMontage)
	class UAnimMontage* AbilityMontage;
	// Deactivate Wand Skill
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AnimMontage)
	class UAnimMontage* DeactiveAbilityMontage;
	
	
	// Anim Replicate
	UFUNCTION(Server, Reliable, WithValidation, Category = "Ability")
	void ServerAbility(bool abilityState);
	UFUNCTION(NetMulticast, Reliable, Category = "Ability")
	void MulticastAbility(bool abilityState);

	//Ability Projection 능력 "투영"
	void UseProjectionTwoHanded();
	UFUNCTION(Server, Reliable)
	void ServerUseProjectionTwoHanded();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastUseProjectionTwoHanded();

	void UseProjectionBow();
	UFUNCTION(Server , Reliable)
	void ServerUseProjectionBow();
	UFUNCTION(NetMulticast , Reliable)
	void MulticastUseProjectionBow();


public:
	//레이저가 닿은 부분에서 호출하게 한다.
	void LightAct(AActor* target , FVector Location);
	UFUNCTION(Server, Reliable)
	void ServerLightAct(AActor* target , FVector Location, bool bIsLight);
	UFUNCTION(NetMulticast , Reliable)
	void MulticastLightAct(AActor* target , FVector Location, bool bIsLight);

	UFUNCTION(NetMulticast , Reliable)
	void SetInteractionFlag(bool isHost , bool isInteraction);


	//�Է� �Ͻ�����
	UFUNCTION(BlueprintCallable)
	void SetPause(bool isPaused);


	//게임플레이 중 액터가 소멸되었을 때 호출.
	virtual void Destroyed();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	//플레이어 캐릭터를 재시작할 게임 모드 클래스 호출.
	void CallRespawnPlayer();

	//Arrow Spawn Loctaion
	UPROPERTY(EditAnywhere , Category = Bow)
	FVector ArrowOffset;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns WeaponComponent subobject **/
	FORCEINLINE class UWeaponComponent* GetWeaponComponent() const { return Weapon_rMesh; }
	/** Returns BowComponent subobject **/
	FORCEINLINE class UBowComponent* GetBowComponent() const { return Bow_lMesh; }


	//�ȷ�Ʈ ���� �������Ʈ�� ȣ���ؾ��ؼ� Public + BlueprintCallable ���� 
	UFUNCTION(BlueprintCallable)
	void StopPalette();

	void WorldMap();

	//����ʵ� ��������
	UFUNCTION(BlueprintCallable)
	void StopWorldMap();

	UFUNCTION(BlueprintCallable)
	void StopSystemMenu();

	void LogoutClient();

	void LoadSaveData(bool isHost);
	UFUNCTION(Server, Reliable)
	void ServerRequestLoadSaveData();

	UFUNCTION(BlueprintCallable)
	void StopLogoutClient();

	//���� ������ ������Ʈ
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerAbility(EPlayerAbility playerAbility);
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerGMState(EPlayerGMState playerGMState);

	/* Ability */
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	EPlayerAbility nowAbility = EPlayerAbility::ENone;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	bool abilityReadyStatus = false;

	// Wand Ability
	UPROPERTY(Replicated , VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	bool WandReadySign;

	// LineTrace: Set Wand Ability Vector
	bool WandLineTrace(float distance);

	UFUNCTION(NetMulticast , Reliable , Category = "UI" , BlueprintCallable)
	void PlayerUIInit();


	UFUNCTION(NetMulticast , Reliable , Category = "UI", BlueprintCallable)
		void MulticastStopLoadingWidget();
	UFUNCTION(Server , Reliable , Category = "UI", BlueprintCallable)
		void ServerStopLoadingWidget();

	UPROPERTY(Replicated, VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	bool KeepAbility;
	UFUNCTION(Server , Reliable , WithValidation , Category = "Ability")
	void ServerSetKeepAbility(bool isKeeping);
	UFUNCTION(NetMulticast , Unreliable , Category = "Ability")
	void MulticastSetKeepAbility(bool isKeeping);

	bool canUseAbility;

private:
	// Character Mesh
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UStaticMeshComponent* HeadMesh;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UStaticMeshComponent* HairAndHatMesh;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UStaticMeshComponent* EyesMesh;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UStaticMeshComponent* MouthMesh;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class USkeletalMeshComponent* CloakMesh;
	

	// Mesh: Weapons & Skills
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UWeaponComponent* Weapon_rMesh;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UBowComponent* Bow_lMesh;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class ULaserNiagaraComponent* Comp_LaserNiagara;

	// Set LineTrace Start Loc
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* SpawnLocSource;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UAssimilationComponent* AssimilationComponent;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class USkeletalMesh* BodyMesh_Light;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UStaticMesh* HeadMesh_Light;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UStaticMesh* HairAndHatMesh_Light;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UStaticMesh* EyesMesh_Light;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UStaticMesh* MouthMesh_Light;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class USkeletalMesh* CloakMesh_Light;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UMaterialInterface* BodyMat_Light;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UMaterialInterface* HairMat_Light;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UMaterialInterface* EyesMat_Light;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UMaterialInterface* CloakMat_Light;


	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class USkeletalMesh* BodyMesh_Dark;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UStaticMesh* HeadMesh_Dark;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UStaticMesh* HairAndHatMesh_Dark;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UStaticMesh* EyesMesh_Dark;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UStaticMesh* MouthMesh_Dark;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class USkeletalMesh* CloakMesh_Dark;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UMaterialInterface* BodyMat_Dark;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UMaterialInterface* HairMat_Dark;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UMaterialInterface* EyesMat_Dark;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UMaterialInterface* CloakMat_Dark;

	int idx = 0;

	// Control CameraBoom Rotation
	float CameraBoomMinPitch = -30.0f;
	float CameraBoomMaxPitch = 30.0f;
};
