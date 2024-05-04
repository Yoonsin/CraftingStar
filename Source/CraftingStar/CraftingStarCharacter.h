// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "CustomEnum.h"
#include "Net/UnrealNetwork.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
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

	//�ȷ�Ʈ
	class UUserWidget* PaletteWidgetRef;
	FTimerHandle HoldTimerHandle;
	float PaletteCnt;

	//�����
	class UUserWidget* WorldMapWidgetRef;
	class UUserWidget* SystemMenuWidgetRef;
	class UUserWidget* LoadingWidgetRef;

	/* Ability */
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	EPlayerAbility nowAbility = EPlayerAbility::ENone;
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	bool abilityReadyStatus = false;

	// Telekinesis
	UPrimitiveComponent* selectedTarget;
	void Telekinesis();

	
public:
	ACraftingStarCharacter();
	class UUserWidget* LogOutClientWidgetRef;
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	



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

	// Ability
	void ActivateAbility();
	void DeactivateAbility();
	void ActivateAbility2();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AnimMontage)
	class UAnimMontage* AbilityMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AnimMontage)
	class UAnimMontage* DeactiveAbilityMontage;
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category = AnimMontage)
	class UAnimMontage* ProjectionTwoHandedMontage;
	
	// Anim Replicate
	UFUNCTION(Server, Reliable, WithValidation, Category = "CraftingStar Character")
	void ServerAbility(bool abilityState);
	UFUNCTION(NetMulticast, Unreliable, Category = "CraftingStar Character")
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
	void ServerLightAct(AActor* target , FVector Location, bool isHost);
	UFUNCTION(NetMulticast , Reliable)
	void MulticastLightAct(AActor* target , FVector Location, bool isHost);

	


	//�Է� �Ͻ�����
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

	FORCEINLINE class UWeaponComponent* GetWeaponComponent() const { return Weapon_rMesh; }

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

	UFUNCTION(BlueprintCallable)
	void StopLogoutClient();

	//���� ������ ������Ʈ
	UFUNCTION(BlueprintCallable)
		void UpdatePlayerAbility(EPlayerAbility playerAbility);
	UFUNCTION(BlueprintCallable)
		void UpdatePlayerGMState(EPlayerGMState playerGMState);

	// LineTrace: Set Wand Ability Vector
	bool WandLineTrace(float distance);


	UFUNCTION(NetMulticast , Reliable , Category = "UI", BlueprintCallable)
		void MulticastStopLoadingWidget();
	UFUNCTION(Server , Reliable , Category = "UI", BlueprintCallable)
		void ServerStopLoadingWidget();


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
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))

	// Mesh: Weapons & Skills
	class UWeaponComponent* Weapon_rMesh;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class UBowComponent* Bow_lMesh;
	UPROPERTY(EditAnywhere , BlueprintReadOnly , Category = Ability , meta = ( AllowPrivateAccess = "true" ))
	class ULaserNiagaraComponent* Comp_LaserNiagara;

	// Set LineTrace Start Loc
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* SpawnLocSource;



	bool KeepAbility;
	bool canUseAbility;

	int idx = 0;
};
