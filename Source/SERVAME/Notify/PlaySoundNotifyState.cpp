// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaySoundNotifyState.h"

UPlaySoundNotifyState::UPlaySoundNotifyState()
{
}

void UPlaySoundNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	int Num = static_cast<int>(Type);

	Boss = Cast<AJesusBoss>(MeshComp->GetOwner());

	if (Boss && Boss->BossAnimInstance->IsStart)
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[Num].ObjClass, Boss->GetActorLocation(), FRotator::ZeroRotator);
}

