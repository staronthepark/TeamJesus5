#pragma once

#include "CoreMinimal.h"
#include "BaseObjectInPool.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "../Player/PlayerCharacter.h"
#include "../MoveToLocationComp.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle)
		bool IsMove = false;

	APlayerCharacter* Player;

	UPROPERTY(Editanywhere)
		UBoxComponent* OverlapComp;

	UPROPERTY(EditAnywhere)
		UMoveToLocationComp* MoveComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Particle)
	UParticleSystemComponent* ParticleSystem;

	UFUNCTION()
		void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void SetActive(bool active) override;
	virtual void BeginPlay() override;
};