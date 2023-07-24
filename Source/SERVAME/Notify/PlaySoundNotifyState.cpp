// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaySoundNotifyState.h"

UPlaySoundNotifyState::UPlaySoundNotifyState()
{
}

void UPlaySoundNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	int Num = static_cast<int>(Type);

	BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (BaseCharacter)
	{
		visit_at(GetBoss(MeshComp), BossEnumType.GetIntValue(), [=](auto& val)
			{
				if (val->IsGameStart)
					AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[Num].ObjClass, val->GetActorLocation(), FRotator::ZeroRotator);
			});

	}
}

