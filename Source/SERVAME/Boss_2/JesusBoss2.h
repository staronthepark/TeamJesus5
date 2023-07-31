// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\BaseCharacter.h"
#include "Boss2AIController.h"
#include "Boss2AnimInstance.h"
#include "Boss2AnimMetaData.h"
#include "Components/PoseableMeshComponent.h"
#include "..\Player\PlayerCharacter.h"
#include "NavigationSystem.h"
#include "Components/WidgetComponent.h"
#include "..\UI\MonsterWidget.h"
#include <vector>
#include "JesusBoss2.generated.h"

UENUM()
enum Boss2DirectionType
{
	B2_ERROR UMETA(DisplayName = "ERROR"),
	B2_FOWARD UMETA(DisplayName = "FOWARD"),
	B2_BACK UMETA(DisplayName = "BACK"),
	B2_LEFT UMETA(DisplayName = "LEFT"),
	B2_RIGHT UMETA(DisplayName = "RIGHT"),
	B2_FL UMETA(DisplayName = "FL"),
	B2_FR UMETA(DisplayName = "FR"),
	B2_BL UMETA(DisplayName = "BL"),
	B2_BR UMETA(DisplayName = "BR"),
	B2_EVERYWHERE UMETA(DisplayName = "EVERYWHERE"),
};

UENUM()
enum Boss2ActionType
{
	B2_ENUMSTART = 0,
	B2_NOTIHING UMETA(DisplayName = "B2_NOTIHING"),
	B2_FALLTHECROSS UMETA(DisplayName = "B2_FALLTHECROSS"),
	B2_SLASH UMETA(DisplayName = "B2_SLASH"),
	B2_DOWNSMASH UMETA(DisplayName = "B2_DOWNSMASH"),
	B2_DOUBLESMASH UMETA(DisplayName = "B2_DOUBLESMASH"),
	B2_SCREAMATTACK UMETA(DisplayName = "B2_SCREAMATTACK"),
	B2_HEADATTACK UMETA(DisplayName = "B2_HEADATTACK"),
	B2_CHARGE UMETA(DisplayName = "B2_CHARGE"),
	B2_FALLTHECROSS_LEFT UMETA(DisplayName = "B2_FALLTHECROSS_LEFT"),
	B2_FALLTHECROSS_RIGHT UMETA(DisplayName = "B2_FALLTHECROSS_RIGHT"),
	B2_FALLTHECROSS_BACK UMETA(DisplayName = "B2_FALLTHECROSS_BACK"),
	B2_HEADING UMETA(DisplayName="B2_HEADING"),
	B2_VOMITFALL UMETA(DisplayNAme = "B2_VOMITFALL"),
	B2_ELBOWSPIN UMETA(DisplayNAme = "B2_ELBOWSPIN"),
	B2_HUNTJUMP UMETA(DisplayNAme = "B2_HUNTJUMP"),
	B2_JUMPEXPLOSION UMETA(DisplayNAme = "B2_JUMPEXPLOSION"),
	B2_THROWSTONE UMETA(DisplayNAme = "B2_THROWSTONE"),
	B2_ENUMEND,
};

UENUM()
enum Boss2BaseAction
{
	B2_SUPER_IDLE UMETA(DisplayName = "SUPER_IDLE"),
	B2_SUPER_HIT UMETA(DisplayName = "SUPER_HIT"),
	B2_SUPER_ATTACK UMETA(DisplayName = "SUPER_ATTACK"),
	B2_SUPER_MOVE UMETA(DisplayName = "SUPER_MOVE"),
	B2_SUPER_PARRINGED UMETA(DisplayName = "SUPER_PARRINGED"),
};

UENUM()
enum Boss2AttackType
{
	B2_MELEE,
	B2_RANGE,
	B2_FOLLOWUP,
	B2_LEFTATK,
	B2_RIGHTATK,
	B2_BACKATK,
	NONE,
};

UENUM()
enum Boss2CollisionType
{
	HEAD,
	LEFTARM,
	RIGHTARM,
	CHARGE,
};

USTRUCT()
struct FBoss2Action : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Speed = 60.f;

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
	TEnumAsByte<Boss2ActionType> ActionType;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<Boss2DirectionType> ActionDirectionType;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<Boss2BaseAction> SuperAction = Boss2BaseAction::B2_SUPER_ATTACK;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<Boss2AttackType> AttackType;
};

struct Boss2ActionTemp
{
public:
	float Speed;
	int Percentage;
	Boss2ActionType ActionType;
	Boss2DirectionType ActionDirection;
	Boss2BaseAction SuperAction;
	UAnimMontage* ActionMontage;
	Boss2AttackType AttackType;
	bool IsAddPercentage = false;
	bool CanParring;
	bool CanContinuity;
	bool HitCancel;
	float Distance;
	bool IsExcute = false;

	bool operator==(const Boss2ActionTemp& other) const
	{
		if (ActionType == other.ActionType)
			return true;
		else
			return false;
	}
};

USTRUCT(BlueprintType)
struct FBoss2DataStruct : public FCharacterBaseDataStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<Boss2AnimationType, int64>DamageList;
};

UCLASS()
class SERVAME_API AJesusBoss2 : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJesusBoss2();
	~AJesusBoss2();

	UPROPERTY()
	UBoss2AnimInstance* Boss2AnimInstance;

	UPROPERTY(EditAnywhere, Category = "Cross")
	int CrossCount;
	UPROPERTY(EditAnywhere, Category = "Cross")
	float CrossMaxRange;
	UPROPERTY(EditAnywhere, Category = "Cross")
	float SpawnDelay;
	UPROPERTY(EditAnywhere, Category = "Cross")
	float SpawnHeight;
	UPROPERTY(EditAnywhere, Category = "Cross")
	FVector X_MinMax;
	UPROPERTY(EditAnywhere, Category = "Cross")
	FVector Y_MinMax;
	UPROPERTY(EditAnywhere, Category = "Cross")
	FVector Z_MinMax;
	UPROPERTY(EditAnywhere, Category = "Cross")
	float SpawnTime;
	UPROPERTY(EditAnywhere, Category = "Cross")
	float DelayBetweenCross;
	int CurrentCrossCount;
	FTimerHandle CrossTimerHandle;
	FTimerHandle CrossSpawnTimerHandle;
	TQueue<ABaseObjectInPool*> CrossQueue;

	UPROPERTY(EditAnywhere, Category = "BoneRatationVal")
	FRotator HeadBoneRotVal;
	UPROPERTY(EditAnywhere, Category = "BoneRatationVal")
	FRotator LArmBoneRotVal;
	UPROPERTY(EditAnywhere, Category = "BoneRatationVal")
	FRotator RArmBoneRotVal;

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
	FTimerHandle BoneRotateTimerHandle;
	TMap<Boss2BoneRotateType, TFunction<void()>> BoneMap;

	UPROPERTY(EditAnywhere, Category = "Vomit")
	int VomitCount = 5;
	UPROPERTY(EditAnywhere, Category = "Vomit")
	float delay = 5;
	UPROPERTY(EditAnywhere, Category = "Vomit")
	float VomitMaxRange;
	FTimerHandle VomitTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss2Data")
	FBoss2DataStruct BossDataStruct;

	UPROPERTY(EditAnywhere, Category = "BossPatternDT")
	UDataTable* Boss2Actions;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Boss2)
	bool IsAttacking = false;
	UPROPERTY(VisibleAnywhere, Category = Boss2)
	bool IsLockOn = false;
	UPROPERTY(EditAnywhere, Category = Boss2)
	float RangeAtk = 700.f;
	UPROPERTY(EditAnywhere, Category = Boss2)
	float MaxAtkRange = 2500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* Boss2HitCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> LeftAtkCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> RightAtkCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> AreaAtkPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> HeadAtkCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> ChargeAtkCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> HeadHitCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> RightArmHitCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> LeftArmHitCollision;

	Boss2BaseAction Boss2SuperAction;
	Boss2DirectionType PlayerDirection;
	Boss2ActionType CurrentAction;
	Boss2AnimationType CurrentAnimType;

	Boss2ActionTemp CurrentActionTemp{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DecreasePercentageVal")
	int DecreasePercentageVal = 10;

	int HitCount;
	float Damage;

	bool CanAttack = false;
	bool ChangeSuperAction = false;
	bool CanMove;
	bool IsPlayerAlive;
	bool AttackLockOn;
	bool IsDead = false;
	bool IsAttackMontageEnd = true;
	bool IsMontagePlay = false;
	bool IsExplosionPattern = false;
	bool IsRunArrived = false;
	bool IsGameStart = false;
	bool IsArrived;
	bool IsStartBoneRot;
	bool JumpMoveStart;

	FVector LastPlayerLoc;

	TAtomic<bool>IsStart = false;
	TAtomic<bool>IsEnd = false;
	
	TTuple<bool, bool> StartEnd;

	ABoss2AIController* AIController;

	UPROPERTY()
	UAnimMontage* EndedMontage;
	UPROPERTY()
	UAnimMontage* StartMontage;

	UPROPERTY()
	const UEnum* Boss2ActionEnum;

	UPROPERTY()
	UMonsterWidget* MonsterLockOnWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* LockOnWidget;

	TObjectPtr<APlayerCharacter> PlayerCharacter;

	TObjectPtr<class AStoneObjectInPool> StonePoolObj;

	/*=====================
	*		Map
	=====================*/
	UPROPERTY(EditAnyWhere, Category = "Boss2MontageMap")
	TMap<Boss2AnimationType, UAnimMontage*> Boss2MontageMap;

	TMap<Boss2AnimationType, TFunction<void(AJesusBoss2* Boss)>>MontageStartMap;
	TMap<Boss2AnimationType, TFunction<void(AJesusBoss2* Boss)>>MontageEndMap;
	TMap<Boss2ActionType, TFunction<void(AJesusBoss2* Boss)>>BossAttackMap;
	TMap<Boss2AttackType, TFunction<void(float Dist, float Time, UAnimMontage* Montage)>> ActionEndMap;
	TMap<Boss2AttackType, TFunction<Boss2ActionTemp()>> GetRandomPatternMap;
	TMap<Boss2AttackType, TFunction<void(Boss2ActionTemp* Temp)>> AddArrMap;
	TMap<Boss2AttackType, TFunction<void(Boss2ActionTemp* Temp)>> ChangePercentageMap;
	TMap<Boss2AttackType, TFunction<void()>> InitPercentageMap;
	TMap<Boss2CollisionType, TFunction<void(bool OnOff)>> CollisionMap;

	TArray<Boss2ActionTemp> MeleeActionArr;
	TArray<Boss2ActionTemp> MeleeTempArr;
	std::vector<int>MeleePercentageVec;

	TArray<Boss2ActionTemp> RangeActionArr;
	TArray<Boss2ActionTemp> RangeTempArr;
	std::vector<int>RangePercentageVec;

	TArray<Boss2ActionTemp> FollowUpActionArr;
	TArray<Boss2ActionTemp> FollowUpTempArr;
	std::vector<int>FollowUpPercentageVec;

	//TArray<Boss2ActionTemp> LeftActionArr;
	//TArray<Boss2ActionTemp> LeftTempArr;
	//std::vector<int>LeftPercentageVec;

	//TArray<Boss2ActionTemp> RightActionArr;
	//TArray<Boss2ActionTemp> RightTempArr;
	//std::vector<int>RightPercentageVec;

	//TArray<Boss2ActionTemp> BackActionArr;
	//TArray<Boss2ActionTemp> BackTempArr;
	//std::vector<int>BackPercentageVec;
	
	/*======================
	*		Override
	======================*/ 
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void HitStop() override;
	virtual void ResumeMontage() override;
	virtual void RespawnCharacter() override;
	virtual void IsNotifyActive(bool value) override;
	virtual void PlayExecutionAnimation() override;
	virtual void ActivateLockOnImage(bool value) override;
	virtual void ActivateHitCollision() override;

	/*=====================
	*		Function
	=====================*/
	void SetMetaData();
	Boss2AnimationType GetTypeFromMetaData(UAnimMontage* Montage);
	void ChangeMontageAnimation(Boss2AnimationType Type);
	void ChangeAnimType(Boss2AnimationType Type);
	FBoss2Action* GetActionData(FName Name);
	void DoTypeAttack(float MinRange, float MaxRange, float Dist, bool LockOn, Boss2AnimationType Type, AJesusBoss2* Boss2, TArray<Boss2ActionTemp> &arr, Boss2AttackType AtkType);
	void InitIsExcute();
	void SetBTAction(Boss2ActionTemp Temp);
	void PlayAttackAnim(Boss2AnimationType Type);
	void PlayMoveMontage();
	void RotateToPlayerInterp();
	void SetLockOn() { AttackLockOn = true; }
	void SetLockOff() { AttackLockOn = false; LastPlayerLoc = PlayerCharacter->GetActorLocation(); }
	FVector Lerp(const FVector& start, const FVector& end, const float t);
	Boss2ActionTemp GetRandomPattern(float Dist);
	void SpawnInit();
	void StartBoneRot();
	void ReSetBoneRot();
	void OffHitCollision();
	void SlerpJump();
	void SlerpJumpEnd();
	void JumpMove();

	/*======================
	*		UFUNCTION
	======================*/
	UFUNCTION()
	void AttackHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void SetBoneHead(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void SetBoneLArm(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void SetBoneRArm(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void GetEndedMontage(UAnimMontage* Montage, bool bInterrupted);


	/*=====================
			Notify
	=====================*/
	void OnCrossFall();
	void OnVomitFall();
	void OnStart();
	void OnEnd();
	void LockOn();
	void LockOff();
	void ThrowStone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

public:	

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
