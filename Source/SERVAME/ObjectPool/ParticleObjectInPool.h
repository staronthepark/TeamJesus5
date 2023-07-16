#pragma once

#include "CoreMinimal.h"
#include "BaseObjectInPool.h"
#include "Particles/ParticleSystemComponent.h"
#include "ParticleObjectInPool.generated.h"

UCLASS()
class SERVAME_API AParticleObjectInPool : public ABaseObjectInPool
{
	GENERATED_BODY()

	AParticleObjectInPool();

private:
	FTimerHandle LifeTimer;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle)
	float LifeTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Particle)
	UParticleSystemComponent* ParticleSystem;

	virtual void SetActive(bool active) override;
};