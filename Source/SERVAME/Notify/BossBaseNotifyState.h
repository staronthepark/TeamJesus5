// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "..\Boss_1\JesusBoss.h"
#include "..\Boss_2\JesusBoss2.h"
#include "..\Player\PlayerCharacter.h"
#include <tuple>
#include <assert.h>
#include "BossBaseNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UBossBaseNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
protected:	
	ABaseCharacter* BaseCharacter;

	std::tuple<AJesusBoss*, AJesusBoss2*>GetBoss(USkeletalMeshComponent* MeshComp);

	UPROPERTY(EditAnywhere)
	TEnumAsByte<BossType> BossEnumType;
};

//��Ÿ�ӿ� �ʱ�ȭ �Ǵ� ������ Tuple ���� ������ �� ���Ұ�.
template <size_t I>
struct visit_impl
{
	template <typename T, typename F>
	static void visit(T& tup, size_t idx, F fun)
	{
		if (idx == I - 1)
		{
			fun(std::get<I - 1>(tup));
		}
		else
			visit_impl<I - 1>::visit(tup, idx, fun);
	}
};

//���ø� Ư��ȭ
//Ʃ���� ��� ��ȸ�ؼ� 0�� �Ǿ��µ��� ã�� ���ϸ� assert
template <>
struct visit_impl<0>
{
	template <typename T, typename F>
	static void visit(T& tup, size_t idx, F fun) { assert(false); }
};

template <typename F, typename... Ts>
void visit_at(std::tuple<Ts...> const& tup, size_t idx, F fun)
{
	visit_impl<sizeof...(Ts)>::visit(tup, idx, fun);
}
