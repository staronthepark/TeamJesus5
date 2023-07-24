#include "..\Boss_1\JesusBoss.h"
#include "Kismet/GameplayStatics.h"
#include "JesusPlayerController.h"

void AJesusPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bAutoManageActiveCameraTarget = false;

	bShowMouseCursor = true;

	ChangeTargetMouseValue = 3.0f;

	character = Cast<APlayerCharacter>(GetPawn());

	GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	TimeSpeed = 1.0f;

	PlayerSkillType = 2;

	
	CameraYawRotateEventMap.Add(false, [&](float Val)
		{
			AddYawInput(Val);
		});
	CameraYawRotateEventMap.Add(true, [&](float Val)
		{
			ChangeTargetEventMap[FMath::Abs(Val) > ChangeTargetMouseValue && character->ChangeTargetTime >= 0.5f][Val < 0]();
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
			bShowMouseCursor = false;
			character->UserSettingUI->RemoveFromViewport();
			return;
		}
	}

	if (character->AnimInstance->PlayerAnimationType == AnimationType::GAMESTART ||
		character->AnimInstance->PlayerAnimationType == AnimationType::GAMESTARTLOOP)
	{
		return;
	}

	if (character->AnimInstance->PlayerAnimationType == AnimationType::SAVELOOP && IsInputKeyDown(EKeys::Gamepad_FaceButton_Right))
	{
		character->ChangeMontageAnimation(AnimationType::SAVEEND);
		character->PlayerHUD->PlayExitAnimation(false);
		return;
	}

	character->GetWorldTimerManager().SetTimer(character->SprintStartTimer, character, &APlayerCharacter::SetSprint, 0.2f);
	character->InputEventMap[character->PlayerCurAction][ActionType::DODGE][true]();
}

void AJesusPlayerController::UnPressDodge()
{
	if (character->AnimInstance->PlayerAnimationType == AnimationType::GAMESTART || 
		character->AnimInstance->PlayerAnimationType == AnimationType::GAMESTARTLOOP)
	{
		return;
	}
	character->GetWorldTimerManager().ClearTimer(character->SprintStartTimer);
	if (character->IsSprint)
	{
		character->GetWorldTimerManager().SetTimer(character->SprintEndTimer, character, &APlayerCharacter::EndSprint, 0.1f);
	}
	else
	{
		character->InputEventMap[character->PlayerCurAction][ActionType::DODGE][false]();
	}
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
}

void AJesusPlayerController::PressUseItem()
{
	character->InputEventMap[character->PlayerCurAction][ActionType::HEAL][true]();
}

void AJesusPlayerController::UnPressUseItem()
{
	character->InputEventMap[character->PlayerCurAction][ActionType::HEAL][false]();
}

void AJesusPlayerController::ViewLog()
{
	if (GameInstance->DebugLogWidget->IsInViewport())
	{
		character->DebugMode = false;
		GameInstance->DebugLogWidget->RemoveFromViewport();
	}
	else
	{
		character->DebugMode = true;
		GameInstance->DebugLogWidget->AddToViewport();
	}
}

void AJesusPlayerController::PressLockon()
{	
	if (GameInstance->MainMenuWidget->IsInViewport())
	{
		bShowMouseCursor = false;
		character->UserSettingUI->RemoveFromViewport();
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

	if(character->CurActionType == ActionType::MOVE)
	character->TargetCameraBoomLength = character->IsShoulderView ? character->ShoulderViewCameraLength : character->BackViewCameraLength;
}

void AJesusPlayerController::UnPressMoveBack()
{
	if (character->AxisY == 2)
	{
		character->AxisY = 1;
		character->InputEventMap[character->PlayerCurAction][ActionType::MOVE][false]();
	}
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

void AJesusPlayerController::OpenMenu()
{
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
			bShowMouseCursor = true;
			character->UserSettingUI->AddToViewport();			
		}
		else
		{
			SetPause(false);
			bShowMouseCursor = false;
			character->UserSettingUI->RemoveFromViewport();
		}
	}
}

void AJesusPlayerController::CloseMenu()
{
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
			bShowMouseCursor = false;
			character->UserSettingUI->RemoveFromViewport();
		}
	}
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

	InputComponent->BindAction("MoveForward", IE_Pressed, this, &AJesusPlayerController::PressMoveForward);
	InputComponent->BindAction("MoveForward", IE_Released, this, &AJesusPlayerController::UnPressMoveForward);

	InputComponent->BindAction("MoveBack", IE_Pressed, this, &AJesusPlayerController::PressMoveBack);
	InputComponent->BindAction("MoveBack", IE_Released, this, &AJesusPlayerController::UnPressMoveBack);

	InputComponent->BindAction("MoveRight", IE_Pressed, this, &AJesusPlayerController::PressMoveRight);
	InputComponent->BindAction("MoveRight", IE_Released, this, &AJesusPlayerController::UnPressMoveRight);

	InputComponent->BindAction("MoveLeft", IE_Pressed, this, &AJesusPlayerController::PressMoveLeft);
	InputComponent->BindAction("MoveLeft", IE_Released, this, &AJesusPlayerController::UnPressMoveLeft);

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
}