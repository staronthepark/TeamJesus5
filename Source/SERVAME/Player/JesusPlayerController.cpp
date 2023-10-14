#include "..\Boss_1\JesusBoss.h"
#include "Kismet/GameplayStatics.h"
#include "../JesusSaveGame.h"
#include "GameFramework/GameUserSettings.h"
#include "JesusPlayerController.h"

void AJesusPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bAutoManageActiveCameraTarget = false;

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;

	ChangeTargetMouseValue = 3.0f;

	character = Cast<APlayerCharacter>(GetPawn());

	GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	TimeSpeed = 1.0f;

	PlayerSkillType = 2;

	IsGuideRender = false;
	//ConsoleCommand("t.maxfps 60");

	
	CameraYawRotateEventMap.Add(false, [&](float Val)
		{
			AddYawInput(Val);
		});
	CameraYawRotateEventMap.Add(true, [&](float Val)
		{
			float MouseValue = character->IsInputPad ? .1f : ChangeTargetMouseValue;
			ChangeTargetEventMap[FMath::Abs(Val) > MouseValue && character->ChangeTargetTime >= 0.5f][Val < 0]();
		});

	ChangeTargetEventMap.Add(false, TMap<bool, TFunction<void()>>());
	ChangeTargetEventMap[false].Add(false, [&]()
		{
		});
	ChangeTargetEventMap[false].Add(true, [&]()
		{
		});
	ChangeTargetEventMap.Add(true, TMap<bool, TFunction<void()>>());
	ChangeTargetEventMap[true].Add(false, [&]()
		{
			character->ChangeTarget(CameraDirection::RIGHT);
		});
	ChangeTargetEventMap[true].Add(true, [&]()
		{
			character->ChangeTarget(CameraDirection::LEFT);
		});

	CameraPitchRotateEventMap.Add(false, [&](float Val)
		{
			AddPitchInput(Val);
		});

	CameraPitchRotateEventMap.Add(true, [&](float Val)
		{
		});
}

void AJesusPlayerController::YawInput(float Val)
{	
	CameraYawRotateEventMap[character->IsLockOn](Val);
}

void AJesusPlayerController::PitchInput(float Val)
{
	CameraPitchRotateEventMap[character->IsLockOn](Val);
}

void AJesusPlayerController::AddYawInput(float Val)
{
	if(character->AnimInstance->PlayerAnimationType != AnimationType::EXECUTIONBOSS)
	{
		Super::AddYawInput(Val * GameInstance->PlayerOptionSetting.DPI * character->fDeltaTime);
	}
}

void AJesusPlayerController::AddPitchInput(float Val)
{
	if (character->AnimInstance->PlayerAnimationType != AnimationType::EXECUTIONBOSS)
		Super::AddPitchInput(Val * GameInstance->PlayerOptionSetting.DPI * character->fDeltaTime);
}

void AJesusPlayerController::TimeFast()
{
	TimeSpeed = FMath::Clamp(TimeSpeed += 0.1f, 0.0f, 1.0f);
	UGameplayStatics::SetGlobalTimeDilation(this, TimeSpeed);
}

void AJesusPlayerController::TimeSlow()
{
	TimeSpeed = FMath::Clamp(TimeSpeed -= 0.1f, 0.0f, 1.0f);
	UGameplayStatics::SetGlobalTimeDilation(this, TimeSpeed);
}

void AJesusPlayerController::OriginTime()
{
	TimeSpeed = 1.0f;
	UGameplayStatics::SetGlobalTimeDilation(this, TimeSpeed);
}

void AJesusPlayerController::PauseTime()
{
	TimeSpeed = 0.0f;
	UGameplayStatics::SetGlobalTimeDilation(this, TimeSpeed);
}

void AJesusPlayerController::PressAttack()
{
	IsInputKeyDown(EKeys::LeftMouseButton) ? character->SetInputType(false) : character->SetInputType(true);
	character->InputEventMap[character->PlayerCurAction][ActionType::ATTACK][true]();
}

void AJesusPlayerController::UnPressAttack()
{
	character->InputEventMap[character->PlayerCurAction][ActionType::ATTACK][false]();
}

void AJesusPlayerController::PressDodge()
{
	if (!GameInstance->MainMenuWidget->IsInViewport())
	{
		if (character->PlayerHUD->IsRender())
		{
			character->PlayerHUD->PlayAnimations(EGuides::dodge, false);
			return;
		}

		if (character->UserSettingUI->IsInViewport())
		{
			SetPause(false);
			SetInputMode(FInputModeGameOnly());
			bShowMouseCursor = false;
			character->UserSettingUI->RemoveFromParent();
			return;
		}
	}

	if (character->AnimInstance->PlayerAnimationType == AnimationType::SAVELOOP && IsInputKeyDown(EKeys::Gamepad_FaceButton_Right))
	{
		character->ChangeMontageAnimation(AnimationType::SAVEEND);		
		character->PlayerHUD->PlayExitAnimation(false);
		return;
	}

	character->InputEventMap[character->PlayerCurAction][ActionType::DODGE][true]();
}

void AJesusPlayerController::UnPressDodge()
{
}

void AJesusPlayerController::PressPowerAttack()
{
	IsInputKeyDown(EKeys::Gamepad_RightTrigger) ? character->SetInputType(true) : character->SetInputType(false);
	character->InputEventMap[character->PlayerCurAction][ActionType::POWERATTACK][true]();
}

void AJesusPlayerController::UnPressPowerAttack()
{
	character->InputEventMap[character->PlayerCurAction][ActionType::POWERATTACK][false]();
}

void AJesusPlayerController::PressParring()
{
	IsInputKeyDown(EKeys::LeftControl) ? character->SetInputType(false) : character->SetInputType(true);
	character->InputEventMap[character->PlayerCurAction][ActionType::PARRING][true]();
}

void AJesusPlayerController::UnPressParring()
{
	character->InputEventMap[character->PlayerCurAction][ActionType::PARRING][false]();
	character->IsGrab = false;
}

void AJesusPlayerController::PressUseItem()
{
	character->InputEventMap[character->PlayerCurAction][ActionType::HEAL][true]();
}

void AJesusPlayerController::UnPressUseItem()
{
	character->InputEventMap[character->PlayerCurAction][ActionType::HEAL][false]();
}

void AJesusPlayerController::PressTab()
{
	if (!IsGuideRender)
	{
		character->PlayerHUD->PlayAnimations(EGuides::keyguide, true);
		IsGuideRender = true;
	}
	else
	{
		character->PlayerHUD->PlayAnimations(EGuides::keyguide, false);
		IsGuideRender = false;
	}
}

void AJesusPlayerController::PressSprint()
{
	if(!character->IsGrab)
	character->SetSprint();
}

void AJesusPlayerController::UnPressSprint()
{
	if (!character->IsGrab)
		character->InputEventMap[PlayerAction::SPRINT][ActionType::DODGE][false]();
}

void AJesusPlayerController::PressGrab()
{
	character->InputEventMap[character->PlayerCurAction][ActionType::SHIELD][true]();
}

void AJesusPlayerController::UnPressGrab()
{
	if(character->IsGrab)
	character->InputEventMap[character->PlayerCurAction][ActionType::SHIELD][false]();

}

void AJesusPlayerController::ViewLog()
{
	if (GameInstance->DebugLogWidget->IsInViewport())
	{
		character->DebugMode = false;
		GameInstance->DebugLogWidget->RemoveFromParent();
	}
	else
	{
		character->DebugMode = true;
		GameInstance->DebugLogWidget->AddToViewport();
	}
}

void AJesusPlayerController::Save()
{
	UJesusSaveGame::GetInstance().Delete();

	//UJesusSaveGame::GetInstance().Save(character, character->GameInstance);
}

void AJesusPlayerController::Load()
{
	UJesusSaveGame::GetInstance().Load(character, character->GameInstance);

	character->LoadMap();
}

void AJesusPlayerController::PressLockon()
{	
	if (GameInstance->MainMenuWidget->IsInViewport())
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
		character->UserSettingUI->RemoveFromParent();
	}

	if (character->AnimInstance->PlayerAnimationType == AnimationType::SAVELOOP)
	{
		character->ChangeMontageAnimation(AnimationType::SAVEEND);
		character->PlayerHUD->PlayExitAnimation(false);
		return;
	}
	character->LockOn();
}

void AJesusPlayerController::PressInteraction()
{
	if (character->PlayerCurAction == PlayerAction::NONE
		|| character->PlayerCurAction == PlayerAction::RUN ||
		character->PlayerCurAction == PlayerAction::SPRINT)
	{
		character->IsInteraction = true;
		if (character->PlayerHUD->IsRender())
		{
			//UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
			character->PlayerHUD->PlayAnimations(EGuides::dodge, false);
			return;
		}
	}
}

void AJesusPlayerController::UnPressInteraction()
{
	character->IsInteraction = false;
}

void AJesusPlayerController::PressMoveForward()
{
	if (character->AxisY == 2 && character->AxisX == 1 && character->AnimInstance->PlayerAnimationType == AnimationType::SPRINT)
	{
		character->ChangeActionType(ActionType::NONE);
		character->ChangeMontageAnimation(AnimationType::ENDOFSPRINT);
	}

	IsInputKeyDown(EKeys::W) ? character->SetInputType(false) : character->SetInputType(true);
	character->AxisY = 0;
	character->ForwardMovementValue = 1.0f;
	character->InputEventMap[character->PlayerCurAction][ActionType::MOVE][true]();
}

void AJesusPlayerController::UnPressMoveForward()
{
	if (character->AxisY == 0)
	{
		character->AxisY = 1;	
		character->InputEventMap[character->PlayerCurAction][ActionType::MOVE][false]();
	}
}

void AJesusPlayerController::PressMoveBack()
{
	if (character->AxisY == 0 && character->AxisX == 1 && character->AnimInstance->PlayerAnimationType == AnimationType::SPRINT)
	{
		character->ChangeActionType(ActionType::NONE);
		character->ChangeMontageAnimation(AnimationType::ENDOFSPRINT);
	}

	IsInputKeyDown(EKeys::S) ? character->SetInputType(false) : character->SetInputType(true);
	character->AxisY = 2;
	character->ForwardMovementValue = -1.0f;
	character->InputEventMap[character->PlayerCurAction][ActionType::MOVE][true]();

	if(character->CurActionType == ActionType::MOVE && !character->IsGrab)
	character->TargetCameraBoomLength = character->IsShoulderView ? character->ShoulderViewCameraLength : character->BackViewCameraLength;
}

void AJesusPlayerController::UnPressMoveBack()
{
	if (character->AxisY == 2)
	{
		character->AxisY = 1;
		character->InputEventMap[character->PlayerCurAction][ActionType::MOVE][false]();
	}

	if(!character->IsGrab)
	character->TargetCameraBoomLength = character->IsShoulderView ? character->ShoulderViewCameraLength : character->BackViewCameraLength;
}

void AJesusPlayerController::PressMoveLeft()
{
	if (character->AxisX == 2 && character->AxisY == 1 && character->AnimInstance->PlayerAnimationType == AnimationType::SPRINT)
	{
		character->ChangeActionType(ActionType::NONE);
		character->ChangeMontageAnimation(AnimationType::ENDOFSPRINT);
	}

	IsInputKeyDown(EKeys::A) ? character->SetInputType(false) : character->SetInputType(true);
	character->AxisX = 0;
	character->RightMovementValue = -1.0f;
	character->InputEventMap[character->PlayerCurAction][ActionType::MOVE][true]();
}

void AJesusPlayerController::UnPressMoveLeft()
{
	if (character->AxisX == 0)
	{
		character->AxisX = 1;
		character->InputEventMap[character->PlayerCurAction][ActionType::MOVE][false]();
	}	
}

void AJesusPlayerController::PressMoveRight()
{
	if (character->AxisX == 0 && character->AxisY == 1 && character->AnimInstance->PlayerAnimationType == AnimationType::SPRINT)
	{
		character->ChangeActionType(ActionType::NONE);
		character->ChangeMontageAnimation(AnimationType::ENDOFSPRINT);
	}

	IsInputKeyDown(EKeys::D) ? character->SetInputType(false) : character->SetInputType(true);
	character->AxisX = 2;
	character->RightMovementValue = 1.0f;
	character->InputEventMap[character->PlayerCurAction][ActionType::MOVE][true]();
}

void AJesusPlayerController::UnPressMoveRight()
{
	if (character->AxisX == 2)
	{
		character->AxisX = 1;
		character->InputEventMap[character->PlayerCurAction][ActionType::MOVE][false]();
	}
}

void AJesusPlayerController::ChangeView()
{
	character->ShoulderView(!character->IsShoulderView);
}

void AJesusPlayerController::PressSkill()
{
	character->InputEventMap[character->PlayerCurAction][ActionType::SKILL][true]();
}

void AJesusPlayerController::UnPressSkill()
{
	character->InputEventMap[character->PlayerCurAction][ActionType::SKILL][false]();
}

void AJesusPlayerController::OpenMenu()
{
	if (CurrentSequncePlayer)
	{
		CurrentSequncePlayer->Stop();
		character->PlayerHUD->SetVisibility(ESlateVisibility::HitTestInvisible);
		character->SetActorHiddenInGame(false);
		CurrentSequncePlayer = nullptr;
		return;
	}
	if (!GameInstance->MainMenuWidget->IsInViewport())
	{
		if (character->PlayerHUD->IsRender())
		{
			//UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
			character->PlayerHUD->PlayAnimations(EGuides::dodge, false);
			return;
		}
		if (!character->UserSettingUI->IsInViewport())
		{
			SetPause(true);
			character->UserSettingUI->AddToViewport();
			SetInputMode(FInputModeUIOnly());
			bShowMouseCursor = true;
		}
		else
		{
			SetPause(false);
			SetInputMode(FInputModeGameOnly());
			bShowMouseCursor = false;
			character->UserSettingUI->RemoveFromParent();
		}
	}
}

void AJesusPlayerController::CloseMenu()
{
	if (CurrentSequncePlayer)
	{
		CurrentSequncePlayer->Stop();
		character->SetActorHiddenInGame(false);
		character->PlayerHUD->SetVisibility(ESlateVisibility::HitTestInvisible);
		CurrentSequncePlayer = nullptr;
		return;
	}

	if (!GameInstance->MainMenuWidget->IsInViewport())
	{
		if (character->PlayerHUD->IsRender())
		{
			//UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
			character->PlayerHUD->PlayAnimations(EGuides::dodge, false);
			return;
		}
		if (character->UserSettingUI->IsInViewport())
		{
			SetPause(false);
			character->UserSettingUI->RemoveFromParent();
			SetInputMode(FInputModeGameOnly());
			bShowMouseCursor = false;
		}
	}
}

void AJesusPlayerController::Boss2()
{
	character->MoveSpawnLocation(FVector(-6912.570360, -60620.187377, 40.0f));
}

void AJesusPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UE_LOG(LogTemp, Warning, TEXT("!@#!@#!@#"));
	
	InputComponent->BindAxis("Turn", this, &AJesusPlayerController::YawInput);
	InputComponent->BindAxis("LookUp", this, &AJesusPlayerController::PitchInput);

	InputComponent->BindAction("BasicAttack", IE_Pressed, this, &AJesusPlayerController::PressAttack);
	InputComponent->BindAction("BasicAttack", IE_Released, this, &AJesusPlayerController::UnPressAttack);
	InputComponent->BindAction("Dodge", IE_Pressed, this, &AJesusPlayerController::PressDodge);
	InputComponent->BindAction("Dodge", IE_Released, this, &AJesusPlayerController::UnPressDodge);

	InputComponent->BindAction("LockOn", IE_Pressed, this, &AJesusPlayerController::PressLockon);

	InputComponent->BindAction("ViewLog", IE_Pressed, this, &AJesusPlayerController::ViewLog);

	InputComponent->BindAction("Parring", IE_Pressed, this, &AJesusPlayerController::PressParring);
	InputComponent->BindAction("Parring", IE_Released, this, &AJesusPlayerController::UnPressParring);

	InputComponent->BindAction("MoveForward", IE_Pressed, this, &AJesusPlayerController::PressMoveForward).bExecuteWhenPaused = true;
	InputComponent->BindAction("MoveForward", IE_Released, this, &AJesusPlayerController::UnPressMoveForward).bExecuteWhenPaused = true;

	InputComponent->BindAction("MoveBack", IE_Pressed, this, &AJesusPlayerController::PressMoveBack).bExecuteWhenPaused = true;
	InputComponent->BindAction("MoveBack", IE_Released, this, &AJesusPlayerController::UnPressMoveBack).bExecuteWhenPaused = true;

	InputComponent->BindAction("MoveRight", IE_Pressed, this, &AJesusPlayerController::PressMoveRight).bExecuteWhenPaused = true;
	InputComponent->BindAction("MoveRight", IE_Released, this, &AJesusPlayerController::UnPressMoveRight).bExecuteWhenPaused = true;

	InputComponent->BindAction("MoveLeft", IE_Pressed, this, &AJesusPlayerController::PressMoveLeft).bExecuteWhenPaused = true;
	InputComponent->BindAction("MoveLeft", IE_Released, this, &AJesusPlayerController::UnPressMoveLeft).bExecuteWhenPaused = true;

	InputComponent->BindAction("UseItem", IE_Pressed, this, &AJesusPlayerController::PressUseItem);
	InputComponent->BindAction("UseItem", IE_Released, this, &AJesusPlayerController::UnPressUseItem);

	InputComponent->BindAction("PowerAttack", IE_Pressed, this, &AJesusPlayerController::PressPowerAttack);
	InputComponent->BindAction("PressShift", IE_Released, this, &AJesusPlayerController::UnPressPowerAttack);

	InputComponent->BindAction("TimeSlow", IE_Pressed, this, &AJesusPlayerController::TimeSlow);
	InputComponent->BindAction("TimeFast", IE_Pressed, this, &AJesusPlayerController::TimeFast);
	InputComponent->BindAction("OriginTime", IE_Released, this, &AJesusPlayerController::OriginTime);
	InputComponent->BindAction("TImePause", IE_Released, this, &AJesusPlayerController::PauseTime);

	InputComponent->BindAction("Interaction", IE_Pressed, this, &AJesusPlayerController::PressInteraction);
	InputComponent->BindAction("Interaction", IE_Released, this, &AJesusPlayerController::UnPressInteraction);

	InputComponent->BindAction("OpenMenu", IE_Pressed, this, &AJesusPlayerController::OpenMenu).bExecuteWhenPaused = true;
	InputComponent->BindAction("CloseMenu", IE_Pressed, this, &AJesusPlayerController::CloseMenu).bExecuteWhenPaused = true;

	InputComponent->BindAction("ChangeView", IE_Pressed, this, &AJesusPlayerController::ChangeView);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &AJesusPlayerController::PressSprint).bExecuteWhenPaused = true;
	InputComponent->BindAction("Sprint", IE_Released, this, &AJesusPlayerController::UnPressSprint).bExecuteWhenPaused = true;

	InputComponent->BindAction("Grab", IE_Pressed, this, &AJesusPlayerController::PressGrab);
	InputComponent->BindAction("Grab", IE_Released, this, &AJesusPlayerController::UnPressGrab);

	InputComponent->BindAction("Guide", IE_Pressed, this, &AJesusPlayerController::PressTab);

	InputComponent->BindAction("Boss2", IE_Pressed, this, &AJesusPlayerController::Boss2);

	InputComponent->BindAction("Save", IE_Pressed, this, &AJesusPlayerController::Save);
	InputComponent->BindAction("Load", IE_Pressed, this, &AJesusPlayerController::Load);

	InputComponent->BindAction("Skill", IE_Pressed, this, &AJesusPlayerController::PressSkill);
	InputComponent->BindAction("Skill", IE_Released, this, &AJesusPlayerController::UnPressSkill);

}