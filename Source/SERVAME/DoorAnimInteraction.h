// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimationInteraction.h"
#include "DoorAnimInstance.h"
#include "CineCameraActor.h"
#include "../SERVAME/BoxTriggerComp.h"
#include "DoorAnimInteraction.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API ADoorAnimInteraction : public AAnimationInteraction
{
	GENERATED_BODY()

public:
	ADoorAnimInteraction() {
		CloseDoorComp = CreateDefaultSubobject<UBoxComponent>("CloseDoorComp");
//		OpenDoorComp = CreateDefaultSubobject<UBoxComponent>("OpenDoorComp");
		SceneComp = CreateDefaultSubobject<USceneComponent>("SceneComp");

		RootComponent = SceneComp;

		MeshComp->SetupAttachment(SceneComp);
	//	OpenDoorComp->SetupAttachment(MeshComp);
		CloseDoorComp->SetupAttachment(MeshComp);
	}

	UPROPERTY()
	UDoorAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool DisableTriggerWhenStart;
	bool DisableTriggerWhenStart2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool PlayerPlayDoorOpenAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		DoorAnimationType DoorAnimType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* CloseDoorComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* OpenDoorComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComp;

	ACineCameraActor* CineCameraActor;

	UBoxTriggerComp* BoxComp;


	ALevelSequenceActor* LevelSequenceActor;
	UPROPERTY(EditAnywhere)
		ULevelSequence* BossRoomDoorOpenSequence;


	UPROPERTY()
		ULevelSequencePlayer* BossRoomDoorOpenSequncePlayer;

	UFUNCTION()
		void OnCloseDoorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndSequence();

	UFUNCTION()
		void OnOpenDoorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	virtual void Load(bool value) override;
	virtual void Init()override;
	virtual void BeginPlay()override;
	virtual void BeginTriggerEvent()override;
	virtual void EndTriggerEvent()override;
	virtual void EnableEvent()override;


};
