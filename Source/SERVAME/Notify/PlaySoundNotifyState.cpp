// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaySoundNotifyState.h"

UPlaySoundNotifyState::UPlaySoundNotifyState()
{
}

void UPlaySoundNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (BaseCharacter)
	{
		if (IsRandom)
		{
			//몬스터 Hidden 되면 소리 안나는지 테스트 해보기
			visit_at(GetBoss(MeshComp), BossEnumType.GetIntValue(), [=](auto& val)
				{
						if (val->IsGameStart && !val->IsDead && !val->IsHidden())
						val->PlayMonsterRandomSoundInPool(Range_Start, Range_End);
				});
		}
		else
		{
			visit_at(GetBoss(MeshComp), BossEnumType.GetIntValue(), [=](auto& val)
				{
					if (val->IsGameStart && !val->IsDead && !val->IsHidden())
						val->PlayMonsterSoundInPool(Type);
				});
		}
	}
}

