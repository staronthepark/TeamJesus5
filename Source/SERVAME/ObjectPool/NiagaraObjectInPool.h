#pragma once

#include "CoreMinimal.h"
#include "../ObjectPool/BaseObjectInPool.h"
#include "../Player/PlayerCharacter.h"
#include "../MoveToLocationComp.h"
#include "NiagaraObjectInPool.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API ANiagaraObjectInPool : public ABaseObjectInPool
{
	GENERATED_BODY()

		ANiagaraObjectInPool();
private:
	FTimerHandle LifeTimer;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle)
		float LifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle)
		bool IsMove = false;

	APlayerCharacter* Player;

	UPROPERTY(Editanywhere)
		UBoxComponent* OverlapComp;

	UPROPERTY(EditAnywhere)
		UMoveToLocationComp* MoveComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Particle)
		UNiagaraComponent* ParticleSystem;

	UFUNCTION()
		void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void SetActive(bool active) override;
	virtual void BeginPlay() override;
};
