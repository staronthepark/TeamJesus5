// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerAnimInstance.h"
#include "Components/CanvasPanelSlot.h"
#include "Containers/Queue.h"
#include "..\UI\PlayerHUD.h"
#include "..\UI\UserSettingUI.h"
#include "..\JesusSaveGame.h"
#include "Particles/ParticleSystemComponent.h"
#include "ActorSequenceComponent.h"
#include "ActorSequencePlayer.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class PlayerAction : uint8
{
	NONE,
	AFTERATTACK,
	BEFOREATTACK,
	CANWALK,
	CANATTACK,
	RUN,
	SPRINT,
	CANTACT,
};

UENUM(BlueprintType)
enum class ActionType : uint8
{
	NONE,
	DODGE,
	ATTACK,
	POWERATTACK,
	PARRING,
	MOVE,
	ROTATE,
	HEAL,
	HIT,
	INTERACTION,
	DEAD,
};

UENUM(BlueprintType)
enum class CameraDirection : uint8
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

USTRUCT(BlueprintType)
struct FPlayerDamageInfo
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float VibrateIntensity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float VibrateDuration;
};

USTRUCT(BlueprintType)
struct FPlayerCharacterDataStruct : public FCharacterBaseDataStruct
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<AnimationType, FPlayerDamageInfo>DamageList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxHealCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxStamina;  
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayerStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayerRunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayerWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayerLockOnMoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StaminaRecovery;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayerHealValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayerRunStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxBulletCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayerExecutionFirstDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayerExecutionSecondDamage;


};

UCLASS()
class SERVAME_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
	APlayerCharacter();
	~APlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


private:
	//UPROPERTY()
	//UJesusSaveGame* SaveGameInstance;
	//UPROPERTY()
	//UJesusSaveGame* LoadGameInstance;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", Meta = (AllowPrivateAccess = true))
	UCameraComponent* FollowCamera;

	FVector CamExecutionDirection;

	int32 CurRotateIndex;

	float DiagonalSpeed;
	float RotSpeed;

	FVector PlayerForwardDirection;
	FVector PlayerRightDirection;

	TArray<TArray<float>> ForwardRotation;
	TArray<TArray<AnimationType>> MovementAnim;
	TArray<TArray<AnimationType>> DodgeDirection;

	TMap<AnimationType, PlayerAction>PlayerEnumToAnimTypeMap;
	TMap<AnimationType, TMap<bool, TFunction<void(APlayerCharacter* character)>>> NotifyBeginEndEventMap;
	TMap<PlayerAction, TMap<ActionType, TFunction<void(APlayerCharacter* character)>>> PlayerActionTickMap;
	TMap<AnimationType, TFunction<void(APlayerCharacter* character)>>MontageEndEventMap;

	TMap<ActionType, TMap< int32, AnimationType>>IntToEnumMap;

	TMap<AnimationType, FRotator> HitEffectRotatorList;

	TMap<bool, TFunction<AnimationType (APlayerCharacter* character)>> DodgeAnimationMap;
	TMap<bool, TFunction<AnimationType(APlayerCharacter* character)>> MovementAnimMap;

public:
	bool DebugMode;
	bool CanExecution;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSpringArm")
		USpringArmComponent* CameraBoom1;

	UPROPERTY()
		TSubclassOf<UPlayerHUD> PlayerUIClass;

	UPROPERTY()
		UPlayerHUD* PlayerHUD;

	UPROPERTY()
		UActorSequenceComponent* BossExecutionSequence;
	
	UPROPERTY()
		UActorSequenceComponent* GameStartSequence;
	
	UPROPERTY()
		UActorSequencePlayer* BossExecutionSequncePlayer;
	
	UPROPERTY()
		UActorSequencePlayer* GameStartSequncePlayer;
	
	UPROPERTY()
		UActorSequencePlayer* BossParryingSequncePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* ExecutionTrigger;

	UPROPERTY()
		UCanvasPanelSlot* LockOnImageSlot;

	UPROPERTY()
		TSubclassOf<UUserSettingUI> UserSettingUIClass;

	UPROPERTY()
		UUserSettingUI* UserSettingUI;

	ABaseCharacter* ExecutionCharacter;

	TArray<UPrimitiveComponent*>TargetCompArray;
	TArray<UPrimitiveComponent*>TargetCompInScreenArray;
	UPrimitiveComponent* TargetComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf< UCameraShakeBase> PlayerDoorCameraShake;

	TMap<AnimationType, TMap<bool, TFunction<void(APlayerCharacter* character)>>> PlayerEventFuncMap;
	TMap<bool, TFunction<void(APlayerCharacter* character)>> PlayerAttackFuncMap;
	TMap<PlayerAction, TMap<ActionType, TMap<bool, TFunction<void(APlayerCharacter* character)>>>>InputEventMap;

	FVector ExecuteDirection;
	FVector ExecuteLocation;

	int32 CurBulletCount;
	int32 CurHealCount;

	float ChangeTargetTime;	

	FTimerHandle SprintStartTimer;
	FTimerHandle SprintEndTimer;
	FTimerHandle DeadTimer;

	UPROPERTY()
		UPlayerAnimInstance* AnimInstance;


	ActionType CurActionType;
	ActionType PlayerAttackType;


	FRotator YawRotation;

	FVector TargetDirection;
	FRotator Difference;

	int32 PlayerCurAttackIndex;

	int32 AxisX;
	int32 AxisY;
	
	bool CanNextAttack;
	bool IsSprint;
	bool IsInteraction;
	bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MontageBlendInTime;

	float ForwardMovementValue;
	float RightMovementValue;
	float HeadYawRoation;

	PlayerAction PlayerCurAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData")
	FPlayerCharacterDataStruct PlayerDataStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMeshComponent* LocketSKMesh;

	UPROPERTY(EditAnyWhere, Category = "MontageMap")
	TMap<AnimationType,UAnimMontage*> MontageMap;

	UPROPERTY(EditAnyWhere, Category = "StaminaMap")
	TMap<ActionType, float>PlayerUseStaminaMap;

	UPROPERTY()
		TMap<AnimationType, int32>PlayerDamageList;
	UPROPERTY()
		TMap<ActionType, int32>PlayerMaxAttackIndex;

	TMap<bool, TMap<bool, float>>SpeedMap;

	TMap<bool, TFunction<void(APlayerCharacter* character)>> LockOnCameraSettingMap;

	UPROPERTY()
		TArray<UBoxComponent*> ForwardOverlap;

	UPROPERTY(EditAnyWhere, Category = "CameraViewSetting")
		bool IsShoulderView;
	UPROPERTY(EditAnyWhere, Category = "CameraViewSetting")
		float TargetCameraBoomLength;
	UPROPERTY(EditAnyWhere, Category = "CameraViewSetting")
		float BackViewCameraLength;
	UPROPERTY(EditAnyWhere, Category = "CameraViewSetting")
		float ShoulderViewCameraLength;

	UPROPERTY(EditAnyWhere, Category = "CameraViewSetting")
		FVector TargetSocketOffset;
	UPROPERTY(EditAnyWhere, Category = "CameraViewSetting")
		FVector ShoulderViewSocketOffset;
	UPROPERTY(EditAnyWhere, Category = "CameraViewSetting")
		FVector BackViewSocketOffset;

public:

	void GetCompsInScreen(TArray<UPrimitiveComponent*>Array);
	void GetFirstTarget();
	void ChangeTarget(CameraDirection Direction);

	void ShoulderView(bool Value);

	void SetInputType(bool IsPad);

	void UseItem();

	void LookTarget();

	void EndSprint();

	void SetPlayerForwardRotAndDir();
	void SetPlayerRightRotAndDir();

	void RotatePlayer();
	void PlayerMovement();

	void Parring();

	void Attack();

	void BasicAttack();

	void PowerAttack();

	void FadeIn();

	void ComboAttackEnd();

	void ComboAttackStart();

	void Dodge();

	void RestoreStat();

	void LockOn();

	void Sprint();
	void Run();

	void RecoverStamina();

	virtual void IsNotifyActive(bool value) override;

	virtual void CheckMontageEndNotify() override;

	bool UseStamina(float value);

	void CheckInputKey();

	void SetSpeed(float speed);

	void ChangeMontageAnimation(AnimationType type);

	void ChangeActionType(ActionType type);

	void ChangePlayerAction(PlayerAction action);
	
	void SetSprint();

	void FadeOut();


	UFUNCTION()
		void PlayStartAnimation();

	virtual void ResumeMontage() override;

	virtual void HitStop() override;

	virtual void BeforeAttackNotify(bool value) override;

	virtual void AfterAttackNotify(bool value) override;	

	virtual void Tick(float DeltaTime) override;

	virtual void RespawnCharacter() override;

	virtual void EventNotify(bool value);

	virtual void PlayExecutionAnimation() override;

	UFUNCTION()
	void OnEnemyDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEnemyDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSMOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnSMOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnExecutionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnExecutionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnParryingOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetCameraLocation(FVector Location);

	void PlayerDead(bool IsFly);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;

	virtual void ActivateRightWeapon() override;
	virtual void DeactivateRightWeapon() override;
	virtual void ActivateSMOverlap() override;
	virtual void DeactivateSMOverlap() override;
};