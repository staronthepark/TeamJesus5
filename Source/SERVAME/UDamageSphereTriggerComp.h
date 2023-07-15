// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include ".\Manager\JesusGameInstance.h"
#include ".\Boss_1\JesusBoss.h"
#include ".\Player\PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "UDamageSphereTriggerComp.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API UUDamageSphereTriggerComp : public USphereComponent
{
	GENERATED_BODY()
public:
	UUDamageSphereTriggerComp();

	bool IsActiveHit;
	int Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageTime;

	UPROPERTY()
	UJesusGameInstance* GameInstance;

	AJesusBoss* Boss;
	APlayerCharacter* Character;

//	FDamageEvent DamageEvent;
	FTimerHandle Timer;

	virtual void BeginPlay();

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ActiveHit();
};
