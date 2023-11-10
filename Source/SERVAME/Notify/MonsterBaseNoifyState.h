// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "..\Monster\Jamsig\Jamsig.h"
#include "..\Monster\Knight\KinghtMonster.h"
#include "..\Monster\Nun\NunMonster.h"
#include <tuple>
#include <assert.h>
#include "MonsterBaseNoifyState.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UMonsterBaseNoifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	ABaseCharacter* BaseCharacter;

	std::tuple<ANunMonster*, AJamsig*, AKinghtMonster*>GetMonster(USkeletalMeshComponent* MeshComp);

	UPROPERTY(EditAnywhere)
	TEnumAsByte<NotifyMonsterType> MonsterEnumType;
};

template <size_t I>
struct visit_monster_impl
{
	template <typename T, typename F>
	static void visit(T& tup, size_t idx, F fun)
	{
		if (idx == I - 1)
		{
			fun(std::get<I - 1>(tup));
		}
		else
			visit_monster_impl<I - 1>::visit(tup, idx, fun);
	}
};

template <>
struct visit_monster_impl<0>
{
	template <typename T, typename F>
	static void visit(T& tup, size_t idx, F fun) { assert(false); }
};

template <typename F, typename... Ts>
void visit_monster_at(std::tuple<Ts...> const& tup, size_t idx, F fun)
{
	visit_monster_impl<sizeof...(Ts)>::visit(tup, idx, fun);
}
