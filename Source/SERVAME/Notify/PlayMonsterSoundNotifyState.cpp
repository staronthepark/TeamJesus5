// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayMonsterSoundNotifyState.h"


UPlayMonsterSoundNotifyState::UPlayMonsterSoundNotifyState()
{

}

void UPlayMonsterSoundNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (BaseCharacter)
	{
		if (IsRandom)
		{
			visit_monster_at(GetMonster(MeshComp), MonsterEnumType.GetIntValue(), [=](auto& val)
				{
					if (val->IsGameStart)
						val->PlayMonsterRandomSoundInPool(Range_Start, Range_End);
				});
		}
		else
		{
			visit_monster_at(GetMonster(MeshComp), MonsterEnumType.GetIntValue(), [=](auto& val)
				{
					if (val->IsGameStart)
						val->PlayMonsterSoundInPool(Type);
				});
		}
	}
}
