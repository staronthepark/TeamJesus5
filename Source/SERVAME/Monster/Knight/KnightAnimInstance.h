// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\..\BaseAnimInstance.h"
#include "KnightAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInterpStart);
DECLARE_MULTICAST_DELEGATE(FOnInterpEnd);
DECLARE_MULTICAST_DELEGATE(FOnKnockBackStart);
DECLARE_MULTICAST_DELEGATE(FOnKnockBackEnd);
DECLARE_MULTICAST_DELEGATE(FOnSpawningBegin);
DECLARE_MULTICAST_DELEGATE(FOnSpawningEnd);
DECLARE_MULTICAST_DELEGATE(FOnCanHitCancle);
DECLARE_MULTICAST_DELEGATE(FOnCantHitCancle);
DECLARE_MULTICAST_DELEGATE(FOnCanRotate);
DECLARE_MULTICAST_DELEGATE(FOnCantRotate);


UCLASS()
class SERVAME_API UKnightAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

	UFUNCTION()
	void AnimNotify_InterpStart();

	UFUNCTION()
	void AnimNotify_InterpEnd();

	UFUNCTION()
	void AnimNotify_KnockBackStart();
		
	UFUNCTION()
	void AnimNotify_KnockBackEnd();

	UFUNCTION()
	void AnimNotify_SpawningBegin();

	UFUNCTION()
	void AnimNotify_SpawningEnd();

	UFUNCTION()
	void AnimNotify_OnHitCancle();

	UFUNCTION()
	void AnimNotify_OffHitCancle();

	UFUNCTION()
	void AnimNotify_OnRotate();

	UFUNCTION()
	void AnimNotify_OffRotate();

public:
	UKnightAnimInstance();	
	
	FOnInterpStart InterpStart;
	FOnInterpEnd InterpEnd;
	FOnKnockBackStart KnockBackStart;
	FOnKnockBackEnd KnockBackEnd;
	FOnSpawningBegin SpawningBegin;
	FOnSpawningEnd SpawningEnd;
	FOnCanHitCancle CanHitCancle;
	FOnCantHitCancle CantHitCancle;
	FOnCanRotate CanRotate;
	FOnCanRotate CantRotate;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float BlendSpeed = 0.f;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float BlendDirection = 0.f;
};
