// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include ".\Monster\Nun\NunMonster.h"
#include ".\Player\PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "NunDamageSphereTriggerComp.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UNunDamageSphereTriggerComp : public USphereComponent
{
	GENERATED_BODY()
public:
	UNunDamageSphereTriggerComp();

	bool IsActiveHit;
	int Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageTime;

	ANunMonster* Nun;
	APlayerCharacter* Character;

	FDamageEvent DamageEvent;
	FTimerHandle Timer;

	virtual void BeginPlay();

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ActiveHit();
};
