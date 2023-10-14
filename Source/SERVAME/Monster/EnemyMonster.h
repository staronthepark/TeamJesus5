// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\BaseCharacter.h"
#include "MonsterController.h"
#include "..\Player\PlayerCharacter.h"
#include "MonsterAnimInstance.h"
#include "..\UI\MonsterWidget.h"
#include "../UI/MonsterHPUI.h"
#include "Components/WidgetComponent.h"
#include "../DoorAnimInteraction.h"
#include "EnemyMonster.generated.h"

UENUM(BlueprintType)
enum class MonsterStateType : uint8
{
	NONE,
	BEFOREATTACK,
	AFTERATTACK,
	CANTACT,
};

UENUM(BlueprintType)
enum class MonsterActionType : uint8
{
	NONE,
	DODGE,
	ATTACK,
	POWERATTACK,
	MOVE,
	RUN,
	SPRINT,
	HIT,
	DEAD,
};

UENUM(BlueprintType)
enum class MonsterAttackType : uint8
{
	MELEE,
	RANGE,
};

UENUM(BlueprintType)
enum class MonsterType : uint8
{
	COMMONMELEE,
	COMMONRANGE,
	ELETEMELEE,
	ELETERANGE,
	BOSS,
	TUTORIAL,
	KNIGHT,
	NUN,
	ILLUSION_NUN,
	DEADBODYOFKNIGHT,
	PERSISTENTKNIGHT,
	ELITEKNIGHT,
};

USTRUCT(BlueprintType)
struct FSkillInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Percent;
};

USTRUCT(BlueprintType)
struct FMonsterDataStruct : public FCharacterBaseDataStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PatrolSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotateSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LockOnWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DropSoulCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunSpeed;
};

UCLASS()
class SERVAME_API AEnemyMonster : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

	AEnemyMonster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	MonsterType MyMonsterType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	MonsterAttackType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMonsterDataStruct MonsterDataStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ADoorAnimInteraction* OpenDoor;

	UPROPERTY()
	UMonsterAnimInstance* AnimInstance;

	UPROPERTY(EditAnyWhere, Category = "MontageMap")
	TMap<MonsterAnimationType, UAnimMontage*> MontageMap;

	UPROPERTY(EditAnyWhere, Category = "SkillInfoMap")
		TMap<MonsterAnimationType, FSkillInfo>SkillInfoMap;

	TMap<bool, TFunction<void()>>RotateMap;
	TMap<int, TFunction<void()>>RandomRotateMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UWidgetComponent* HpWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UWidgetComponent* LockOnWidget;

	UPROPERTY()
		UMonsterHPUI* MonsterHPWidget;

	UPROPERTY()
		UMonsterWidget* MonsterLockOnWidget;

		FTimerHandle HpTimer;

	TArray<UBoxComponent*> AttackOverlapArray;

	AMonsterController* MonsterController;
	APlayerCharacter* PlayerCharacter;

	FVector PatrolLocation;
	FRotator YawRotation;
	FRotator TargetRotation;

	MonsterStateType StateType;
	MonsterActionType ActionType;
	
	MonsterAnimationType AnimationType;
	MonsterAnimationType AttackAnimationType;

	USkeletalMeshComponent* SkeletalMeshComp;
	UStaticMeshComponent* SwordMeshComp;

	UPROPERTY(EditAnyWhere, Category = "AttackRange")
	float AttackRange = 100.f;

	float WeaponOpacity;
	float MeshOpacity;

	float CurrentDistance;	
	float DiagonalSpeed;

	float OpactiyDeltaTime;
	
	int32 MonsterMoveEventIndex;
	int32 MonsterRandomMove;
	int CircleIndexCount = 1;
	int PatrolIndexCount = 0;

	bool IsDetect;
	bool TracePlayer;
	bool IsOverlap = false;
	bool IsAttacking = false;
	bool IsDie = false;
	bool CanRotate = true;

	bool asd = false;
	bool asd2 = false;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsPatrol = false;

	AActor* otherActor;

	FTimerHandle MonsterDeadTimer;

protected:
	TMap<int, TFunction<void()>> MonsterMoveMap;
	TMap<MonsterAnimationType, TFunction<void()>>MontageEndEventMap;
	TMap<MonsterAnimationType, MonsterStateType> AnimTypeToStateType;
	TMap<MonsterAnimationType, TMap<bool, TFunction<void()>>> NotifyBeginEndEventMap;
	TMap<MonsterActionType, TFunction<void()>> MonsterTickEventMap;
	TMap<bool, TFunction<void()>> CheckDIstanceMap;
	TMap<MonsterAnimationType, TFunction<void(float percent)>> SetActionByRandomMap;
	TMap<MonsterAttackType, TFunction<void()>>TargetDetectEventMap;

public:
	int GetRandNum(int Min, int Max);

	virtual void ChangeMontageAnimation(MonsterAnimationType type);
	void ChangeActionType(MonsterActionType type);

	void DeactivateHpBar();

	void ActivateHpBar();

	void TickOverlap();

	UFUNCTION()
	void OnTargetDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTargetDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSMOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSMOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnParryingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void StartAttackTrigger(MonsterAnimationType AttackAnimType);
	virtual void EndAttackTrigger(MonsterAnimationType AttackAnimType);

	void ShotProjectile();

	void Rotate();

	virtual void ParryingStun();
	virtual void Stun() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual float Die(float Dm);
	
	virtual void CheckMontageEndNotify() override;

	virtual void PlayExecutionAnimation() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void ActivateLockOnImage(bool value, UPrimitiveComponent* comp) override;
	virtual bool IsAlive()override;

	virtual void IsNotifyActive(bool value) override;
	virtual void RespawnCharacter() override;


	virtual void ResumeMontage() override;

	virtual void MonsterHitStop();
	virtual void HitStop() override;
};