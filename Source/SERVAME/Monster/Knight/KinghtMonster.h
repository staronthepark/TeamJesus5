// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\..\BaseCharacter.h"
#include "..\..\Player\PlayerCharacter.h"
#include "..\..\UI\MonsterHPUI.h"
#include "..\..\UI\MonsterWidget.h"
#include "Components/WidgetComponent.h"
#include "KnightAnimInstance.h"
#include "KnightController.h"
#include "KinghtMonster.generated.h"

UENUM(BlueprintType)
enum class KnightStateType : uint8
{
	NONE,
	BEFOREATTACK,
	AFTERATTACK,
	CANTACT,
};

UENUM(BlueprintType)
enum class KnightAttackType : uint8
{
	MELEE,
	RANGE,
};

UENUM(BlueprintType)
enum class KnightActionType : uint8
{
	NONE,
	DODGE,
	ATTACK,
	POWERATTACK,
	MOVE,
	HIT,
	DEAD,
};


USTRUCT(BlueprintType)
struct FKnightSkillInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Percent;
};

USTRUCT(BlueprintType)
struct FKnightDataStruct : public FCharacterBaseDataStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PatrolSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotateSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LockOnWalkSpeed;
};


UCLASS()
class SERVAME_API AKinghtMonster : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AKinghtMonster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FKnightDataStruct KnightDataStruct;
	
	UPROPERTY()
	UKnightAnimInstance* AnimInstance;

	UPROPERTY(EditAnyWhere, Category = "MontageMap")
	TMap<KnightAnimationType, UAnimMontage*> MontageMap;

	UPROPERTY(EditAnyWhere, Category = "SkillInfoMap")
	TMap<KnightAnimationType, FKnightSkillInfo>SkillInfoMap;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UKnightAttackTriggerComp* AttackTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UKnightAttackTriggerComp* DashAttackTrigger;

	FTimerHandle HpTimer;

	TArray<UBoxComponent*> AttackOverlapArray;

	AKnightController* KnightController;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	APlayerCharacter* PlayerCharacter;

	FVector PatrolLocation;
	FRotator YawRotation;
	FRotator TargetRotation;

	KnightStateType StateType;
	KnightActionType ActionType;

	KnightAnimationType AnimationType;
	KnightAnimationType AttackAnimationType;

	UStaticMeshComponent* SwordMeshComp;

	UPROPERTY(EditAnyWhere,Category = "KnockBackTime")
	float KnockBackTime = 0.2f;
	UPROPERTY(EditAnyWhere, Category = "KnockBackDelayTime")
	float KnockBackDelayTime = 1.f;

	float WeaponOpacity;
	float MeshOpacity;

	float CurrentDistance = 0.f;
	float DiagonalSpeed = 0.f;

	int32 MonsterMoveEventIndex;
	int32 MonsterRandomMove;

	bool IsDetect;
	bool TracePlayer;

	bool IsInterpStart;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsKnockBack = false;

private:
	TMap<KnightAnimationType, KnightStateType> AnimTypeToStateType;
	TMap<KnightStateType, TMap<KnightActionType, TFunction<void()>>> MonsterActionEventMap;
	TMap<KnightAnimationType, TMap<bool, TFunction<void()>>> NotifyBeginEndEventMap;
	TMap<KnightActionType, TFunction<void()>> MonsterTickEventMap;
	TMap<int, TFunction<void()>> MonsterMoveMap;
	TMap<bool, TFunction<void()>> CheckDIstanceMap;
	TMap<KnightAnimationType, TFunction<void(float percent)>> SetActionByRandomMap;
	TMap<KnightAnimationType, TFunction<void()>>MontageEndEventMap;
	TMap<KnightAttackType, TFunction<void()>>TargetDetectEventMap;

	FTimerHandle KnockBackTimerHandle;
	FTimerHandle KnockBackDelayTimerHandle;

	//Notify
	void InterpStart();
	void InterpEnd();
	void KnockBackStart();
	void KnockBackEmd();

public:
	void ChangeMontageAnimation(KnightAnimationType type);
	void ChangeActionType(KnightActionType type);

	void InterpMove();

	void DeactivateHpBar();

	void ActivateAttackTrigger();
	void DeactivateAttackTrigger();

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

	void StartAttackTrigger(KnightAnimationType AttackAnimType);
	void EndAttackTrigger(KnightAnimationType AttackAnimType);

	void Rotate();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void CheckMontageEndNotify() override;

	virtual void PlayExecutionAnimation();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void ActivateLockOnImage(bool value) override;

	virtual void IsNotifyActive(bool value) override;
	virtual void RespawnCharacter() override;


	virtual void ResumeMontage() override;

	virtual void HitStop() override;
};