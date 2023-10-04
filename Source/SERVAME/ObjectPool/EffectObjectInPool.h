// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObjectInPool.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "EffectObjectInPool.generated.h"


UENUM(BlueprintType)
enum class SelectEffectType : uint8
{
	NONE,
	KNIGHT_DEAD,
};


UCLASS()
class SERVAME_API AEffectObjectInPool : public ABaseObjectInPool
{
	GENERATED_BODY()

	FTimerHandle LifeTimer;

public:
	AEffectObjectInPool();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float LifeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TMap<SelectEffectType, UNiagaraSystem*> GetTypeEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	SelectEffectType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TObjectPtr<UNiagaraComponent> CurrentEffect;

	virtual void SetActive(bool active) override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void ReturnObject()override;

	void ActivateCurrentEffect();
	void DeactivateCurrentEffect();
};
