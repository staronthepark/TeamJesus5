// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacter.h"
#include "JesusPlayerController.generated.h"

/**
 *
 */
UCLASS()
class SERVAME_API AJesusPlayerController : public APlayerController
{
	GENERATED_BODY()

private:

	UPROPERTY()
	UJesusGameInstance * GameInstance;

	float TimeSpeed;
	float ChangeTargetMouseValue;

	int PlayerSkillType;


public:
	APlayerCharacter* character;

	UPROPERTY()
		ULevelSequencePlayer* CurrentSequncePlayer;


	TMap<bool, TFunction<void(float Val)>>CameraYawRotateEventMap;
	TMap<bool, TMap<bool, TFunction<void()>>>ChangeTargetEventMap;

	TMap<bool, TFunction<void(float Val)>>CameraPitchRotateEventMap;

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	virtual void AddYawInput(float Val)   override;
	virtual void AddPitchInput(float Val) override;

	void YawInput(float Val);
	void PitchInput(float Val);

	void PressAttack();
	void UnPressAttack();

	void PressDodge();
	void UnPressDodge();

	void PressLockon();

	void PressParring();
	void UnPressParring();

	void PressMoveForward();
	void UnPressMoveForward();

	void PressMoveLeft();
	void UnPressMoveLeft();

	void PressMoveRight();
	void UnPressMoveRight();

	void ChangeView();

	void PressMoveBack();
	void UnPressMoveBack();

	void PressUseItem();
	void UnPressUseItem();

	void PressSprint();
	void UnPressSprint();

	void PressGrab();

	void PressPowerAttack();
	void UnPressPowerAttack();

	void PressInteraction();
	void UnPressInteraction();

	void ViewLog();

	void OpenMenu();
	void CloseMenu();
	void TimeFast();
	void TimeSlow();
	void OriginTime();
	void PauseTime();
};