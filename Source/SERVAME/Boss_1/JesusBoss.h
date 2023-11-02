// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\BaseCharacter.h"
#include "BossAIController.h"
#include "BossAnimInstance.h"
#include "Templates/Atomic.h"
#include "Components/SplineComponent.h"
#include "..\UI\BossCharacterWidget.h"
#include "..\UI\MonsterWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "BossAnimMetaData.h"
#include "Components/SceneComponent.h"
#include "..\Player\PlayerCharacter.h"
#include "Components/PoseableMeshComponent.h"
#include <vector>
#include "..\Manager\SoundManager.h"
#include "CineCameraActor.h"
#include "NavigationSystem.h"
#include "..\UDamageSphereTriggerComp.h"
#include "..\ObjectPool\MonsterSoundObjectInpool.h"
#include "JesusBoss.generated.h"

/**
 * 
 */
UENUM()
enum DirectionType
{
	ERROR UMETA(DisplayName = "ERROR"),
	FOWARD UMETA(DisplayName = "FOWARD"),
	BACK UMETA(DisplayName = "BACK"),
	LEFT UMETA(DisplayName = "LEFT"),
	RIGHT UMETA(DisplayName = "RIGHT"),
	FL UMETA(DisplayName = "FL"),
	FR UMETA(DisplayName = "FR"),
	BL UMETA(DisplayName = "BL"),
	BR UMETA(DisplayName = "BR"),
	EVERYWHERE UMETA(DisplayName = "EVERYWHERE"),
};

UENUM()
enum BossActionType
{
	ENUMSTART,
	NOTIHING UMETA(DisplayName = "NOTIHING"),
	M_DOUBLESLASH UMETA(DisplayName = "M_DOUBLESLASH"),
	M_DARKEXPLOSION UMETA(DisplayName = "M_DARKEXPLOSION"),
	M_UPPERSLASH UMETA(DisplayName = "M_UPPERSLASH"),
	R_THROWSTONE UMETA(DisplayName = "R_THROWSTONE"),
	R_JUMPATTACK UMETA(DisplayName = "R_JUMPATTACK"),
	M_DOWNATTACK UMETA(DisplayName = "M_DOWNATTACK"),
	F_FASTSLASH UMETA(DisplayName = "F_FASTSLASH"),
	F_SLASH UMETA(DisplayName = "F_SLASH"),
	F_FARJUMP UMETA(DisplayName = "F_FARJUMP"),
	F_NEARJUMP UMETA(DisplayName = "F_NEARJUMP"),
	M_GROUNDEXPLOSION UMETA(DisplayName = "M_GROUNDEXPLOSION"),
	R_SPRINT UMETA(DisplayName = "R_SPRINT"),
	LEFT_STEP UMETA(DisplayName = "LEFT_STEP"),
	RIGHT_STEP UMETA(DisplayName = "RIGHT_STEP"),
	ENUMEND,
};

UENUM()
enum BossBaseAction
{
	SUPER_IDLE UMETA(DisplayName = "SUPER_IDLE"),
	SUPER_HIT UMETA(DisplayName = "SUPER_HIT"),
	SUPER_ATTACK UMETA(DisplayName = "SUPER_ATTACK"),
	SUPER_MOVE UMETA(DisplayName = "SUPER_MOVE"),
	SUPER_PARRINGED UMETA(DisplayName = "SUPER_PARRINGED"),
};

UENUM()
enum BossCurrentStatus
{
	NOTIHIG UMETA(DisplayName = "NOTIHIG"),
	HIT UMETA(DisplayName = "HIT"),
	ATTACK UMETA(DisplayName = "ATTACK"),
	MOVE UMETA(DisplayName = "MOVE"),
};

UENUM()
enum BossAttackType
{
	MELEE,
	RANGE,
	FOLLOWUP,
	STEP,
};

USTRUCT()
struct FBossAction : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere)
	int Percentage;

	UPROPERTY(EditAnywhere)
	bool CanParring;

	UPROPERTY(EditAnywhere)
	bool CanContinuity;

	UPROPERTY(EditAnywhere)
	bool HitCancel;

	UPROPERTY(EditAnywhere)
	float Distance;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<BossActionType> ActionType;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<DirectionType> ActionDirectionType;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<BossBaseAction> SuperAction;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<BossAttackType> AttackType;
};

struct BossActionTemp
{
public:
	float Speed;
	int Percentage;
	BossActionType ActionType;
	DirectionType ActionDirection;
	BossBaseAction SuperAction;
	UAnimMontage* ActionMontage;
	BossAttackType AttackType;
	bool IsAddPercentage = false;
	bool CanParring;
	bool CanContinuity;
	bool HitCancel;
	float Distance;
	bool IsExcute = false;

	bool operator==(const BossActionTemp& other) const
	{
		if (ActionType == other.ActionType)
			return true;
		else
			return false;
	}
};

USTRUCT(BlueprintType)
struct FBossDataStruct : public FCharacterBaseDataStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<BossAnimationType, int64>DamageList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxGrrogyGauge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentGrrogyGauge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DropSoulCount;
};

DECLARE_MULTICAST_DELEGATE(FOnHit);

UCLASS()
class SERVAME_API AJesusBoss : public ABaseCharacter
{
	GENERATED_BODY()//
public:
	AJesusBoss();
	~AJesusBoss();

	BossBaseAction BossSuperAction;
	DirectionType PlayerDirection;
	BossCurrentStatus CurrentStatus;
	BossActionType CurrentAction;
	BossAnimationType CurrentAnimType;

	FOnHit OnHit;

	UMonsterWidget* MonsterLockOnWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* LockOnWidget;

	UPROPERTY()
	UBossAnimInstance* BossAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossData")
	FBossDataStruct BossDataStruct;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Pawn)
	bool IsAttacking = false;

	UPROPERTY(VisibleAnywhere, Category = Pawn)
	bool IsLockOn = false;

	UPROPERTY(EditAnywhere, Category = Pawn)
	float DetectRange = 0.0f;

	UPROPERTY(EditAnywhere, Category = Pawn)
	float RangeAtk = 700.f;

	UPROPERTY(EditAnywhere, Category = Pawn)
	float MaxAtkRange = 2500.f;

//#define CREATE_SPAWNPOS(idx) USceneComponent* SpawnPos_##idx = CreateDefaultSubobject<USceneComponent>(typeid(this).name());
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USceneComponent*> SpawnPosArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* GroundExplosionPos;

	UPROPERTY(EditAnywhere, Category = Pawn)
	USplineComponent* Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* BossWeaponMesh;


	ALevelSequenceActor* LevelSequenceActor;
	UPROPERTY(EditAnywhere)
		ULevelSequence* BossRoomDoorOpenSequence;


	UPROPERTY()
		ULevelSequencePlayer* BossRoomDoorOpenSequncePlayer;

	ACineCameraActor* CineCameraActor;

	UPROPERTY(EditAnyWhere, Category = "BossMontageMap")
	TMap<BossAnimationType, UAnimMontage*> BossMontageMap;

	UPROPERTY(EditAnywhere, Category = "BossPatternDT")
	UDataTable* BossActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* DarkExplosionCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* BossHitCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* DamageSphereTriggerComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> AreaAtkPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* ParringTrailComp;


	UFUNCTION()
		void EndSequence();

	//UPROPERTY()
	//TSubclassOf<UBossUI> BossUIClass;

	//UPROPERTY()
	//UBossUI* BossUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* BossPhase2Mesh;
	bool ChangeToPhase2 = false;

	UPROPERTY(EditAnywhere, Category = "BoneRatationVal")
	FRotator BoneRotVal;
	UPROPERTY(EditAnywhere, Category = "BoneRatationVal")
	float Time;
	UPROPERTY(EditAnywhere, Category = "BoneRatationVal")
	float Speed;
	UPROPERTY(EditAnywhere, Category = "BoneRatationVal")
	float DeltaSeconds;
	UPROPERTY(EditAnywhere, Category = "BoneRatationVal")
	float ReturnSpeed;
	UPROPERTY(EditAnywhere, Category = "BoneRatationVal")
	float ReturnDeltaSeconds;

	UPROPERTY()
	const UEnum* BossActionEnum;

	TMap<BossAnimationType, TFunction<void(AJesusBoss* Boss)>>MontageStartMap;
	TMap<BossAnimationType, TFunction<void(AJesusBoss* Boss)>>MontageEndMap;
	TMap<BossActionType, TFunction<void(AJesusBoss* Boss)>>BossAttackMap;
	TMap<BossAttackType, TFunction<void(float Dist, float Time, UAnimMontage* Montage)>> ActionEndMap;
	TMap<BossAttackType, TFunction<BossActionTemp()>> GetRandomPatternMap;
	TMap<BossAttackType, TFunction<void(BossActionTemp *Temp)>> AddArrMap;
	TMap<BossAttackType, TFunction<void(BossActionTemp *Temp)>> ChangePercentageMap;
	TMap<BossAttackType, TFunction<void()>> InitPercentageMap;
	TMap<BossAnimationType, FRotator> HitEffectRotatorList;
	TMap<ActionType, TFunction<void()>> HitMap;

	TMap<bool, TFunction<void()>> CheckBoolNExcuteFunctionMap;

	UPROPERTY(EditAnywhere, Category = Pawn)
	float ThrowSpeed;
	
	float AttackRadius;

	int64 Damage;
	float AccumulateDamage;
	float StartPos = 0.f;
	float fDeltaTime = 0.f;
	float ExplosionRange = 0.f;
	float JumpMoveVal = 50.f;
	float MinWalkTime;
	int DecreasePercentageVal = 20;
	int GroundExplosionCnt = 0;
	int DealTimePercent;
	int RandomWalk;
	int CurrentWalkCount;
	int HitCount;
	FVector LastPlayerLoc;
	FVector ParticleLoc;

	//앞잡 보여주기 위해 플레이어에서 패링 성공여부 확인하기 위한 임시변수.
	bool IsParriged;

	bool CanAttack = false;
	bool CanMove;
	bool IsActionEnd;
	bool IsMontagePlay = false;
	bool IsExplosion = false;
	bool DoStep = false;
	bool ChangeSuperAction = false;
	bool AttackLockOn;
	bool JumpMoveStart = false;
	bool IsMoveStart = false;
	bool IsAttackMontageEnd = true;
	bool IsHalfHp;
	bool IsShowHalfHp = false;
	bool DealTime;
	bool IsPlayerAlive;
	bool IsArrived;
	bool IsRunArrived;
	bool IsStartBoneRot = false;
	bool IsExecution = false;
	bool IsExplosionPattern = false;
	bool IsFirstExecution = false;
	bool IsExecutionEnd = false;
	bool IsHitStun = false;
	bool IsDead = false;
	bool Push2PhasePattern = false;
	bool IsGameStart = false;

	TTuple<bool, bool> StartEnd;

	TAtomic<bool>IsThrow = false;
	TAtomic<bool>IsStart = false;
	TAtomic<bool>IsEnd = false;
	TAtomic<bool>CheckAttack2 = false;

	ABossAIController* AIController;
	APlayerCharacter* PlayerCharacter;

	TArray<BossActionTemp> MeleeActionArr;
	TArray<BossActionTemp> MeleeTempArr;
	std::vector<int>MeleePercentageVec;

	TArray<BossActionTemp> RangeActionArr;
	TArray<BossActionTemp> RangeTempArr;
	std::vector<int>RangePercentageVec;
	
	TArray<BossActionTemp> FollowUpActionArr;
	TArray<BossActionTemp> FollowUpTempArr;
	std::vector<int>FollowUpPercentageVec;

	TArray<BossActionTemp> StepArr;

	BossActionTemp CurrentActionTemp{};

	FTimerHandle TimerHandle;
	FTimerHandle HitTimerHandle;
	FTimerHandle ChangePlayerLocTimerHandle;
	FTimerHandle FadeInTimerHandle;


	UPROPERTY()
	UAnimMontage* EndedMontage;
	UPROPERTY()
	UAnimMontage* StartMontage;

	/*=====================
	*		Function
	=====================*/
	void SetMetaData();
	void ChangeHitStatus();
	void PlayMoveMontage();
	void CheckBossDie();
	void ChangeMontageAnimation(BossAnimationType Type);
	void ChangeAnimType(BossAnimationType Type);
	void RotateToPlayer();
	void RotateToPlayerInterp();
	void JumpMove();
	void Attack(BossAnimationType Type);
	void DoAttack(float MinRange, float MaxRange, float Dist, bool LockOn, BossAnimationType Type, AJesusBoss* Boss);
	void DoRangeAttack(float MinRange, float MaxRange, float Dist, bool LockOn, BossAnimationType Type, AJesusBoss* Boss);
	void DoExplosionAttack(float MinRange, float MaxRange, float Dist, bool LockOn, BossAnimationType Type, AJesusBoss* Boss);
	BossAnimationType GetTypeFromMetaData(UAnimMontage* Montage);
	FBossAction* GetActionData(FName Name);
	FVector Lerp(const FVector& start, const FVector& end, const float t);
	void SpawnInit();
	void SetBTAction(BossActionTemp Temp);
	int GetRandomNum(int Min, int Max);
	void DoRandomStep();
	void GroundExplosionCheck();
	void InitIsExcute();
	void IdleMontageEndInit(BossAnimationType Type);
	void MoveMontageEndInit(BossAnimationType Type);
	void CheckDealTime();
	void StartBoneRot();
	void ReSetBoneRot();
	void SetSplineCurve();
	void InitHitCount(); 
	void PlayMonsterSoundInPool(EMonsterAudioType AudioType);

	/*=====================
			Notify
	=====================*/
	void TempRangeNotify();
	void CollisionEnableNotify();
	void CollisionDisableNotify();
	void TrailOn();
	void TrailOff();
	void ChangeDamage();
	void LevelStart();
	void AttackCheck();
	void SetLockOn();
	void SetLockOff();
	void ExplosionNotify();
	void SlerpJump();
	void SlerpJumpEnd();
	void OnStart();
	void OnEnd();
	void SetParticleLastLoc();
	void ReturnStun();
	void ExecutionNotify();

	/*=====================
			DebugLog
	=====================*/
	void TestLogStr(FString Str) { UE_LOG(LogTemp, Warning, TEXT("%s"), *Str); }
	void ActionEndMapTestLog(FString Str, float f);

	/*======================
	*		UFUNCTION
	======================*/
	UFUNCTION()
	void GetEndedMontage(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void AttackHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DarkExplosionHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void GroundExplosionHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSMOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnSMOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnParryingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	/*======================
	*		Override
	======================*/
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void HitStop() override;
	virtual void ResumeMontage() override;
	virtual void RespawnCharacter() override;
	virtual void IsNotifyActive(bool value) override;
	virtual void PlayExecutionAnimation() override;
	virtual void ActivateLockOnImage(bool value, UPrimitiveComponent* comp) override;
	virtual void Stun()override;
	virtual bool IsAlive()override;
	virtual void ActivateSMOverlap(); 
	virtual void DeactivateSMOverlap();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

private:
	/*=========================
	*		Template
	=========================*/
	template <typename T>
	int MakeRandom(std::vector<T> Prob)
	{
		std::random_device rd;
		std::mt19937_64 gen(rd());

		std::discrete_distribution<T> D{ Prob.begin(), Prob.end() };

		return D(gen);
	}
};
