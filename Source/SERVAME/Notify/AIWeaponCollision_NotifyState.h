// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Boss_2\JesusBoss2.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AIWeaponCollision_NotifyState.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UAIWeaponCollision_NotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	TObjectPtr<AJesusBoss2> Boss2;

public:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<Boss2CollisionType> CollisionType;
	UPROPERTY(EditAnywhere)
	bool Left;
	UPROPERTY(EditAnywhere)
	bool Right;
};
