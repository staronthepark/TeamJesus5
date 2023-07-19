// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "..\Boss_1\JesusBoss.h"
#include "..\Boss_2\JesusBoss2.h"
#include "..\Player\PlayerCharacter.h"
#include <tuple>
#include <assert>
#include "AttackCheckNotifyState.generated.h"

/**
 * 
 */

UCLASS()
class SERVAME_API UAttackCheckNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
		
	AJesusBoss* Boss;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	std::tuple<AJesusBoss*, AJesusBoss2*>Test(USkeletalMeshComponent* MeshComp);

public:
	UPROPERTY(EditAnywhere)
	float AttackRadius;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<BossType> BossEnumType;


	void asdf(AJesusBoss* a) {}
};

//런타임에 초기화 되는 변수는 Tuple 값을 가져올 때 사용불가.
template <size_t I>
struct visit_impl
{
    template <typename T>
    static T visit(T& tup, size_t idx)
    {
		if (idx == I - 1)
			return std::get<I - 1>(tup);
			//fun(std::get<I - 1>(tup));
        else 
			visit_impl<I - 1>::visit(tup, idx);
    }
};

//튜플을 모두 순회해서 0이 되었는데도 찾지 못하면 assert
template <>
struct visit_impl<0>
{
    template <typename T>
    static void visit(T& tup, size_t idx) { assert(false); }
};

template <typename... Ts>
void visit_at(std::tuple<Ts...> const& tup, size_t idx)
{
    visit_impl<sizeof...(Ts)>::visit(tup, idx);
}
