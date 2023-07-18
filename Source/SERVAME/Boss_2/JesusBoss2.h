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
	B2_ENUMSTART,
	B2_NOTIHING UMETA(DisplayName = "B2_NOTIHING"),
	B2_FALLTHECROSS UMETA(DisplayName = "B2_FALLTHECROSS"),
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
};

USTRUCT()
struct FBoss2Action : public FTableRowBase
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
	TEnumAsByte<Boss2ActionType> ActionType;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<Boss2DirectionType> ActionDirectionType;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<Boss2BaseAction> SuperAction;

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

	UPROPERTY()
	UBoss2AnimInstance* Boss2AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABoss2AIController* Boss2AIController;

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
	int CurrentCrossCount;
	FTimerHandle CrossTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss2Data")
	FBoss2DataStruct Boss2DataStruct;

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

	Boss2BaseAction Boss2SuperAction;
	Boss2DirectionType PlayerDirection;
	Boss2ActionType CurrentAction;
	Boss2AnimationType CurrentAnimType;

	Boss2ActionTemp CurrentActionTemp{};

	int DecreasePercentageVal = 20;
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
	TMap<ActionType, TFunction<void()>> HitMap;

	TArray<Boss2ActionTemp> MeleeActionArr;
	TArray<Boss2ActionTemp> MeleeTempArr;
	std::vector<int>MeleePercentageVec;

	TArray<Boss2ActionTemp> RangeActionArr;
	TArray<Boss2ActionTemp> RangeTempArr;
	std::vector<int>RangePercentageVec;

	TArray<Boss2ActionTemp> FollowUpActionArr;
	TArray<Boss2ActionTemp> FollowUpTempArr;
	std::vector<int>FollowUpPercentageVec;

	TArray<Boss2ActionTemp> LeftActionArr;
	TArray<Boss2ActionTemp> LeftTempArr;
	std::vector<int>LeftPercentageVec;

	TArray<Boss2ActionTemp> RightActionArr;
	TArray<Boss2ActionTemp> RightTempArr;
	std::vector<int>RightPercentageVec;

	TArray<Boss2ActionTemp> BackActionArr;
	TArray<Boss2ActionTemp> BackTempArr;
	std::vector<int>BackPercentageVec;
	
	/*======================
	*		Override
	======================*/ 
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void HitStop() override;
	virtual void RespawnCharacter() override;
	virtual void IsNotifyActive(bool value) override;
	virtual void PlayExecutionAnimation() override;
	virtual void ActivateLockOnImage(bool value)override;

	/*=====================
	*		Function
	=====================*/
	void SetMetaData();
	Boss2AnimationType GetTypeFromMetaData(UAnimMontage* Montage);
	void ChangeMontageAnimation(Boss2AnimationType Type);
	void ChangeAnimType(Boss2AnimationType Type);
	FBoss2Action* GetActionData(FName Name);
	void DoAttack(float MinRange, float MaxRange, float Dist, bool LockOn, Boss2AnimationType Type, AJesusBoss2* Boss2);
	void DoRangeAttack(float MinRange, float MaxRange, float Dist, bool LockOn, Boss2AnimationType Type, AJesusBoss2* Boss2);
	void InitIsExcute();
	void SetBTAction(Boss2ActionTemp Temp);
	void PlayAttackAnim(Boss2AnimationType Type);

	/*======================
	*		UFUNCTION
	======================*/

	UFUNCTION()
	void AttackHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	/*=====================
			Notify
	=====================*/
	void OnCrossFall();
	void OnStart();
	void OnEnd();
	void CollisionEnableNotify();
	void CollisionDisableNotify();

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
