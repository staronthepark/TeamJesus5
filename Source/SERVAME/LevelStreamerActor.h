// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "BaseCharacter.h"
#include "LevelStreamerActor.generated.h"

UENUM(BlueprintType)
enum class ELoadType : uint8
{
	ONLYLOAD,
	ONLYUNLOAD,
	BOTH,
};

UCLASS()
class SERVAME_API ALevelStreamerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ALevelStreamerActor();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		TArray<ABaseCharacter*> MonsterList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* OverlapVolume;

	UPROPERTY(EditAnywhere)
		FName LevelToLoad;

	UPROPERTY(EditAnywhere)
		ELoadType LoadType;

	UFUNCTION()
		void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
