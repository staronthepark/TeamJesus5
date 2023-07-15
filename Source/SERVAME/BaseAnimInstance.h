// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

UCLASS()
class SERVAME_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UBaseAnimInstance() {}

	void PauseAnimation(UAnimMontage* Montage) { Montage_Pause(Montage); }

	void PlayMontage(UAnimMontage* Montage) { Montage_Play(Montage); }

	void StopMontage(UAnimMontage* Montage) { Montage_Stop(0.25f, Montage); }

	void ResumeMontage(UAnimMontage* Montage) { Montage_Resume(Montage); }

	void JumpMotnageSection(FName section, UAnimMontage* montage) { Montage_JumpToSection(section, montage); }
	FName GetMontageSectionName(int32 section) { return FName(*FString::Printf(TEXT("Attack%d"), section)); }

};