// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include ".\Manager\JesusGameInstance.h"
#include ".\Player\PlayerCharacter.h"
#include "BoxTriggerComp.h"
#include "DamageBoxTriggerComp.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SERVAME_API UDamageBoxTriggerComp : public UBoxComponent
{
	GENERATED_BODY()
public:
	UDamageBoxTriggerComp();

public:
	virtual void BeginPlay();

	bool IsActiveHit;
	int Count;
	

	TArray<ABaseCharacter*>OverlapActorArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DamageTime;

	UPROPERTY()
		UJesusGameInstance* GameInstance;

	APlayerCharacter* Character;

//	FDamageEvent DamageEvent;
	FTimerHandle Timer;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ActiveHit();
};