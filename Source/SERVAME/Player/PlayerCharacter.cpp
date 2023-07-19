// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "..\Manager\CombatManager.h"
#include "GameFramework/Actor.h"
#include "..\ObjectPool\ObjectPool.h"
#include "UObject/Class.h"
#include "Engine/EngineTypes.h"
#include "..\Manager\SoundManager.h"
#include "JesusPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "..\UI\PlayerHUD.h"

APlayerCharacter::APlayerCharacter()
{
	PlayerCurAttackIndex = 0;

	AxisX = 1;
	AxisY = 1;

	CanNextAttack = true;
	CurRotateIndex = 0;

	IsPlayer = true;

	Imotal = false;

	PlayerCurAction = PlayerAction::NONE;
	PlayerAttackType = ActionType::ATTACK;
	CurActionType = ActionType::MOVE;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;


	static ConstructorHelpers::FClassFinder<UPlayerHUD> asd(TEXT("/Game/02_Resource/04_UI/01_WBP/01_PlayerUI/WBP_PlayerHUD"));		
	PlayerUIClass = asd.Class;

	static ConstructorHelpers::FClassFinder<UUserSettingUI> asd2(TEXT("/Game/02_Resource/04_UI/01_WBP/05_UserSetting/WBP_UserSettingUI"));
	UserSettingUIClass = asd2.Class;

	LocketSKMesh = CreateDefaultSubobject<USkeletalMeshComponent>("LocketMesh");
	LocketSKMesh->SetupAttachment(GetMesh(), FName("Root"));

	CameraBoom1 = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom1"));
	CameraBoom1->SetupAttachment(GetMesh(), FName("Ref_Bip001-Spine"));
	CameraBoom1->TargetArmLength = 0.5f;
	CameraBoom1->bUsePawnControlRotation = true; 
	
	TargetDetectionCollison = CreateDefaultSubobject<USphereComponent>(TEXT("Enemy Detection Collider"));
	TargetDetectionCollison->SetupAttachment(RootComponent);
	TargetDetectionCollison->SetCollisionProfileName("Weapon");
	TargetDetectionCollison->SetSphereRadius(500.0f);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerWeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), FName("Weapon_bone"));

	ExecutionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Execution Trigger"));
	ExecutionTrigger->SetupAttachment(RootComponent);
	ExecutionTrigger->SetCollisionProfileName("Execution Trigger");

	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Weapon Box"));
	WeaponCollision->SetupAttachment(WeaponMesh);
	WeaponCollision->SetCollisionProfileName("Weapon");

	ParryingCollision1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Parrying Collision"));
	ParryingCollision1->SetupAttachment(WeaponMesh);
	ParryingCollision1->SetCollisionProfileName("Parrying");

	WeaponOverlapStaticMeshCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Box"));
	WeaponOverlapStaticMeshCollision->SetupAttachment(WeaponMesh);
	WeaponOverlapStaticMeshCollision->SetCollisionProfileName("Weapon");

	SwordTrailComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Sword Trail"));
	SwordTrailComp->SetupAttachment(GetMesh(), FName("Weapon_bone"));
	SwordTrailComp->SetCollisionProfileName("Sword Trail");

	PlayerMaxAttackIndex.Add(ActionType::ATTACK, 4);
	PlayerMaxAttackIndex.Add(ActionType::POWERATTACK, 3);

	ForwardRotation.Add(TArray<float>());
	ForwardRotation[0].Add(-45.0f);
	ForwardRotation[0].Add(0.0f);
	ForwardRotation[0].Add(45.0f);

	ForwardRotation.Add(TArray<float>());
	ForwardRotation[1].Add(-90.0f);
	ForwardRotation[1].Add(0.0f);
	ForwardRotation[1].Add(90.0f);

	ForwardRotation.Add(TArray<float>());
	ForwardRotation[2].Add(-135.0f);
	ForwardRotation[2].Add(-180.0f);
	ForwardRotation[2].Add(135.0f);

	HitEffectRotatorList.Add(AnimationType::ATTACK1, FRotator(50, 90.0f, 0));
	HitEffectRotatorList.Add(AnimationType::ATTACK2, FRotator(0, -90.0f, 0.0f));
	HitEffectRotatorList.Add(AnimationType::ATTACK3, FRotator(0.0f, 180.0f, 0.0f));
	HitEffectRotatorList.Add(AnimationType::ATTACK4, FRotator(0, -90.0f, 0.0f));
	HitEffectRotatorList.Add(AnimationType::POWERATTACK1, FRotator(45.0f, 90.0f, 0.0f));
	HitEffectRotatorList.Add(AnimationType::POWERATTACK2, FRotator(45.0f, -90.0f, 0.0f));
	HitEffectRotatorList.Add(AnimationType::POWERATTACK3, FRotator(90.0f, 0.0f, 0.0f));
	HitEffectRotatorList.Add(AnimationType::RUNATTACK, FRotator(0, 90.0f, 0.0f));
	HitEffectRotatorList.Add(AnimationType::RUNPOWERATTACK, FRotator(0.0f, 90.0f, 0.0f));
	HitEffectRotatorList.Add(AnimationType::DODGEATTACK, FRotator(0, 180.0f, 0.0f));

	IntToEnumMap.Add(ActionType::ATTACK, TMap<int32, AnimationType>());
	IntToEnumMap[ActionType::ATTACK].Add(0, AnimationType::ATTACK1);
	IntToEnumMap[ActionType::ATTACK].Add(1, AnimationType::ATTACK2);
	IntToEnumMap[ActionType::ATTACK].Add(2, AnimationType::ATTACK3);
	IntToEnumMap[ActionType::ATTACK].Add(3, AnimationType::ATTACK4);

	IntToEnumMap.Add(ActionType::POWERATTACK, TMap<int32, AnimationType>());
	IntToEnumMap[ActionType::POWERATTACK].Add(0, AnimationType::POWERATTACK1);
	IntToEnumMap[ActionType::POWERATTACK].Add(1, AnimationType::POWERATTACK2);
	IntToEnumMap[ActionType::POWERATTACK].Add(2, AnimationType::POWERATTACK3);

	DodgeDirection.Add(TArray<AnimationType>());
	DodgeDirection[0].Add(AnimationType::BATTLEDODGE);
	DodgeDirection[0].Add(AnimationType::BATTLEDODGE);
	DodgeDirection[0].Add(AnimationType::BATTLEDODGE);

	DodgeDirection.Add(TArray<AnimationType>());
	DodgeDirection[1].Add(AnimationType::BATTLEDODGE);
	DodgeDirection[1].Add(AnimationType::BACKSTEP);
	DodgeDirection[1].Add(AnimationType::BATTLEDODGE);

	DodgeDirection.Add(TArray<AnimationType>());
	DodgeDirection[2].Add(AnimationType::BATTLEDODGE);
	DodgeDirection[2].Add(AnimationType::BATTLEDODGE);
	DodgeDirection[2].Add(AnimationType::BATTLEDODGE);

	MovementAnim.Add(TArray<AnimationType>());
	MovementAnim[0].Add(AnimationType::FORWARDLEFTWALK);
	MovementAnim[0].Add(AnimationType::FORWARDBATTLEWALK);
	MovementAnim[0].Add(AnimationType::FORWARDRIGHTWALK);

	MovementAnim.Add(TArray<AnimationType>());
	MovementAnim[1].Add(AnimationType::LEFTWALK);
	MovementAnim[1].Add(AnimationType::IDLE);
	MovementAnim[1].Add(AnimationType::RIGHTWALK);

	MovementAnim.Add(TArray<AnimationType>());
	MovementAnim[2].Add(AnimationType::BACKLEFTWALK);
	MovementAnim[2].Add(AnimationType::BACKWALK);
	MovementAnim[2].Add(AnimationType::BACKRIGHTWALK);

	PlayerEnumToAnimTypeMap.Add(AnimationType::NONE, PlayerAction::NONE);
	PlayerEnumToAnimTypeMap.Add(AnimationType::IDLE, PlayerAction::NONE);
	PlayerEnumToAnimTypeMap.Add(AnimationType::ENDOFRUN, PlayerAction::NONE);
	PlayerEnumToAnimTypeMap.Add(AnimationType::FORWARDWALK, PlayerAction::RUN);
	PlayerEnumToAnimTypeMap.Add(AnimationType::FORWARDBATTLEWALK, PlayerAction::RUN);
	PlayerEnumToAnimTypeMap.Add(AnimationType::LEFTWALK, PlayerAction::RUN);
	PlayerEnumToAnimTypeMap.Add(AnimationType::RIGHTWALK, PlayerAction::RUN);
	PlayerEnumToAnimTypeMap.Add(AnimationType::BACKWALK, PlayerAction::RUN);
	PlayerEnumToAnimTypeMap.Add(AnimationType::FORWARDLEFTWALK, PlayerAction::RUN);
	PlayerEnumToAnimTypeMap.Add(AnimationType::FORWARDRIGHTWALK, PlayerAction::RUN);
	PlayerEnumToAnimTypeMap.Add(AnimationType::BACKLEFTWALK, PlayerAction::RUN);
	PlayerEnumToAnimTypeMap.Add(AnimationType::BACKRIGHTWALK, PlayerAction::RUN);
	PlayerEnumToAnimTypeMap.Add(AnimationType::SPRINT, PlayerAction::SPRINT);

	PlayerEnumToAnimTypeMap.Add(AnimationType::BATTLEDODGE, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::BASICDODGE, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::BACKSTEP, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::PARRING, PlayerAction::CANTACT);


	PlayerEnumToAnimTypeMap.Add(AnimationType::HITFRONTLEFT, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::HITFRONTRIGHT, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::HITBACKLEFT, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::HITBACKRIGHT, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::HIT, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::SUPERHIT, PlayerAction::CANTACT);

	PlayerEnumToAnimTypeMap.Add(AnimationType::DEAD, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::DEADLOOP, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::HEAL, PlayerAction::CANWALK);
	PlayerEnumToAnimTypeMap.Add(AnimationType::DOOROPEN, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::ENDOFSPRINT, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::SPRINTTURN, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::EXECUTIONBOSS, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::GAMESTARTLOOP, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::GAMESTART, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::DEADLOOP2, PlayerAction::CANTACT);


	PlayerEnumToAnimTypeMap.Add(AnimationType::ENDOFHEAL, PlayerAction::NONE);

	PlayerEnumToAnimTypeMap.Add(AnimationType::ATTACK1, PlayerAction::BEFOREATTACK);
	PlayerEnumToAnimTypeMap.Add(AnimationType::ATTACK2, PlayerAction::BEFOREATTACK);
	PlayerEnumToAnimTypeMap.Add(AnimationType::ATTACK3, PlayerAction::BEFOREATTACK);
	PlayerEnumToAnimTypeMap.Add(AnimationType::ATTACK4, PlayerAction::BEFOREATTACK);
	PlayerEnumToAnimTypeMap.Add(AnimationType::POWERATTACK1, PlayerAction::BEFOREATTACK);
	PlayerEnumToAnimTypeMap.Add(AnimationType::POWERATTACK2, PlayerAction::BEFOREATTACK);
	PlayerEnumToAnimTypeMap.Add(AnimationType::POWERATTACK3, PlayerAction::BEFOREATTACK);
	PlayerEnumToAnimTypeMap.Add(AnimationType::RUNATTACK, PlayerAction::BEFOREATTACK);
	PlayerEnumToAnimTypeMap.Add(AnimationType::RUNPOWERATTACK, PlayerAction::BEFOREATTACK);
	PlayerEnumToAnimTypeMap.Add(AnimationType::DODGEATTACK, PlayerAction::BEFOREATTACK);

	PlayerEnumToAnimTypeMap.Add(AnimationType::SAVESTART, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::SAVELOOP, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::SAVEEND, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::ACTIVESAVEPOINT, PlayerAction::CANTACT);

	NotifyBeginEndEventMap.Add(AnimationType::NONE, TMap<bool, TFunction<void(APlayerCharacter * character)>>());
	NotifyBeginEndEventMap.Add(AnimationType::IDLE, TMap<bool, TFunction<void(APlayerCharacter * character)>>());
	NotifyBeginEndEventMap.Add(AnimationType::FORWARDWALK, TMap<bool, TFunction<void(APlayerCharacter * character)>>());
	NotifyBeginEndEventMap.Add(AnimationType::LEFTWALK, TMap<bool, TFunction<void(APlayerCharacter * character)>>());
	NotifyBeginEndEventMap.Add(AnimationType::RIGHTWALK, TMap<bool, TFunction<void(APlayerCharacter * character)>>());
	NotifyBeginEndEventMap.Add(AnimationType::BACKWALK, TMap<bool, TFunction<void(APlayerCharacter * character)>>());
	NotifyBeginEndEventMap.Add(AnimationType::FORWARDLEFTWALK, TMap<bool, TFunction<void(APlayerCharacter * character)>>());
	NotifyBeginEndEventMap.Add(AnimationType::FORWARDRIGHTWALK, TMap<bool, TFunction<void(APlayerCharacter * character)>>());
	NotifyBeginEndEventMap.Add(AnimationType::BACKLEFTWALK, TMap<bool, TFunction<void(APlayerCharacter * character)>>());
	NotifyBeginEndEventMap.Add(AnimationType::BACKRIGHTWALK, TMap<bool, TFunction<void(APlayerCharacter* character)>>());

	NotifyBeginEndEventMap.Add(AnimationType::BATTLEDODGE, TMap<bool, TFunction<void(APlayerCharacter * character)>>());
	NotifyBeginEndEventMap[AnimationType::BATTLEDODGE].Add(true, [](APlayerCharacter* character)
		{
			character->Imotal = true;
		});
	NotifyBeginEndEventMap[AnimationType::BATTLEDODGE].Add(false, [](APlayerCharacter* character)
		{
			character->PlayerCurAction = PlayerAction::CANATTACK;
			character->Imotal = false;
			character->ComboAttackEnd();
		});

	NotifyBeginEndEventMap.Add(AnimationType::BASICDODGE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::BASICDODGE].Add(true, [](APlayerCharacter* character)
		{
			character->Imotal = true;
		});
	NotifyBeginEndEventMap[AnimationType::BASICDODGE].Add(false, [](APlayerCharacter* character)
		{
			character->PlayerCurAction = PlayerAction::CANATTACK;
			character->Imotal = false;
			character->ComboAttackEnd();
		});

	NotifyBeginEndEventMap.Add(AnimationType::BACKSTEP, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::BACKSTEP].Add(true, [](APlayerCharacter* character)
		{
			character->Imotal = true;

		});
	NotifyBeginEndEventMap[AnimationType::BACKSTEP].Add(false, [](APlayerCharacter* character)
		{
			character->PlayerCurAction = PlayerAction::CANATTACK;
			character->Imotal = false;
			character->ComboAttackEnd();
		});

	NotifyBeginEndEventMap.Add(AnimationType::HEAL, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::HEAL].Add(true, [](APlayerCharacter* character)
		{
		});
	NotifyBeginEndEventMap[AnimationType::HEAL].Add(false, [](APlayerCharacter* character)
		{
			character->PlayerDataStruct.CharacterHp = FMath::Clamp(character->PlayerDataStruct.CharacterHp += character->PlayerDataStruct.PlayerHealValue, 0.0f, character->PlayerDataStruct.CharacterMaxHp);
			character->PlayerHUD->SetHP(character->PlayerDataStruct.CharacterHp / character->PlayerDataStruct.CharacterMaxHp);
			character->GameInstance->DebugLogWidget->T_PlayerHP->SetText(FText::AsNumber(character->PlayerDataStruct.CharacterHp));
		});

	NotifyBeginEndEventMap.Add(AnimationType::ATTACK1, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::ATTACK1].Add(true, [](APlayerCharacter* character)
		{
			UCombatManager::GetInstance().ActivateCollider();
			character->ActivateSMOverlap();
			character->ActivateRightWeapon();
			character->SwordTrailComp->Activate();
			character->CameraShake(character->PlayerDoorCameraShake);
		});

	NotifyBeginEndEventMap[AnimationType::ATTACK1].Add(false, [](APlayerCharacter* character)
		{
			character->DeactivateRightWeapon();
			character->DeactivateSMOverlap();
			character->SwordTrailComp->Deactivate();
		});

	NotifyBeginEndEventMap.Add(AnimationType::POWERATTACK1, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::POWERATTACK1].Add(true, NotifyBeginEndEventMap[AnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[AnimationType::POWERATTACK1].Add(false, [](APlayerCharacter* character)
		{
			character->DeactivateRightWeapon();
			character->DeactivateSMOverlap();			
			character->SwordTrailComp->Deactivate();
		});

	NotifyBeginEndEventMap.Add(AnimationType::ATTACK2, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::ATTACK2].Add(true, NotifyBeginEndEventMap[AnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[AnimationType::ATTACK2].Add(false, NotifyBeginEndEventMap[AnimationType::ATTACK1][false]);

	NotifyBeginEndEventMap.Add(AnimationType::ATTACK3, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::ATTACK3].Add(true, NotifyBeginEndEventMap[AnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[AnimationType::ATTACK3].Add(false, NotifyBeginEndEventMap[AnimationType::ATTACK1][false]);

	NotifyBeginEndEventMap.Add(AnimationType::ATTACK4, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::ATTACK4].Add(true, NotifyBeginEndEventMap[AnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[AnimationType::ATTACK4].Add(false, NotifyBeginEndEventMap[AnimationType::ATTACK1][false]);

	NotifyBeginEndEventMap.Add(AnimationType::RUNATTACK, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::RUNATTACK].Add(true, NotifyBeginEndEventMap[AnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[AnimationType::RUNATTACK].Add(false, NotifyBeginEndEventMap[AnimationType::ATTACK1][false]);

	NotifyBeginEndEventMap.Add(AnimationType::RUNPOWERATTACK, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::RUNPOWERATTACK].Add(true, NotifyBeginEndEventMap[AnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[AnimationType::RUNPOWERATTACK].Add(false, NotifyBeginEndEventMap[AnimationType::ATTACK1][false]);

	NotifyBeginEndEventMap.Add(AnimationType::POWERATTACK2, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::POWERATTACK2].Add(true, NotifyBeginEndEventMap[AnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[AnimationType::POWERATTACK2].Add(false, NotifyBeginEndEventMap[AnimationType::POWERATTACK1][false]);

	NotifyBeginEndEventMap.Add(AnimationType::POWERATTACK3, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::POWERATTACK3].Add(true, NotifyBeginEndEventMap[AnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[AnimationType::POWERATTACK3].Add(false, [](APlayerCharacter* character)
		{
			character->NotifyBeginEndEventMap[AnimationType::POWERATTACK1][false](character);
			character->PlayerCurAttackIndex = 0;
			character->CameraShake(character->PlayerCameraShake);
			character->VibrateGamePad(character->PlayerDataStruct.DamageList[AnimationType::POWERATTACK1].VibrateIntensity,
			character->PlayerDataStruct.DamageList[AnimationType::POWERATTACK1].VibrateDuration);
		});

	NotifyBeginEndEventMap.Add(AnimationType::DODGEATTACK, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::DODGEATTACK].Add(true, NotifyBeginEndEventMap[AnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[AnimationType::DODGEATTACK].Add(false, [](APlayerCharacter* character)
		{
			character->NotifyBeginEndEventMap[AnimationType::ATTACK1][false](character);
			character->PlayerCurAttackIndex = 0;
		});

	NotifyBeginEndEventMap.Add(AnimationType::PARRING, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::PARRING].Add(true, [](APlayerCharacter* character)
		{
			character->ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		});
	NotifyBeginEndEventMap[AnimationType::PARRING].Add(false, [](APlayerCharacter* character)
		{
			character->ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		});

	NotifyBeginEndEventMap.Add(AnimationType::HIT, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::HIT].Add(true, [](APlayerCharacter* character)
		{
			character->PlayerCurAction = PlayerAction::AFTERATTACK;
		});
	NotifyBeginEndEventMap[AnimationType::HIT].Add(false, [](APlayerCharacter* character)
		{

		});

	NotifyBeginEndEventMap.Add(AnimationType::HITFRONTLEFT, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::HITFRONTLEFT].Add(true, [](APlayerCharacter* character)
		{
			character->PlayerCurAction = PlayerAction::AFTERATTACK;
		});
	NotifyBeginEndEventMap[AnimationType::HITFRONTLEFT].Add(false, [](APlayerCharacter* character)
		{

		});

	NotifyBeginEndEventMap.Add(AnimationType::HITFRONTRIGHT, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::HITFRONTRIGHT].Add(true, [](APlayerCharacter* character)
		{
			character->PlayerCurAction = PlayerAction::AFTERATTACK;
		});
	NotifyBeginEndEventMap[AnimationType::HITFRONTRIGHT].Add(false, [](APlayerCharacter* character)
		{

		});

	NotifyBeginEndEventMap.Add(AnimationType::HITBACKLEFT, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::HITBACKLEFT].Add(true, [](APlayerCharacter* character)
		{
			character->PlayerCurAction = PlayerAction::AFTERATTACK;
		});
	NotifyBeginEndEventMap[AnimationType::HITBACKLEFT].Add(false, [](APlayerCharacter* character)
		{

		});

	NotifyBeginEndEventMap.Add(AnimationType::HITBACKRIGHT, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::HITBACKRIGHT].Add(true, [](APlayerCharacter* character)
		{
			character->PlayerCurAction = PlayerAction::AFTERATTACK;
		});
	NotifyBeginEndEventMap[AnimationType::HITBACKRIGHT].Add(false, [](APlayerCharacter* character)
		{

		});


	NotifyBeginEndEventMap.Add(AnimationType::EXECUTIONBOSS, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::EXECUTIONBOSS].Add(true, [](APlayerCharacter* character)
		{
			character->CameraShake(character->PlayerCameraShake);
			character->VibrateGamePad(0.2f, 0.2f);
		});
	NotifyBeginEndEventMap[AnimationType::EXECUTIONBOSS].Add(false, [](APlayerCharacter* character)
		{

		});

	NotifyBeginEndEventMap.Add(AnimationType::SUPERHIT, TMap<bool, TFunction<void(APlayerCharacter * character)>>());
	NotifyBeginEndEventMap[AnimationType::SUPERHIT].Add(true, [](APlayerCharacter* character)
		{
			character->PlayerCurAction = PlayerAction::AFTERATTACK;
		});
	NotifyBeginEndEventMap[AnimationType::SUPERHIT].Add(false, [](APlayerCharacter* character)
		{
		});

	NotifyBeginEndEventMap.Add(AnimationType::DOOROPEN, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::DOOROPEN].Add(false, [](APlayerCharacter* character)
		{
			character->CameraShake(character->PlayerDoorCameraShake);
			character->VibrateGamePad(0.6, 0.2);
		});
	NotifyBeginEndEventMap[AnimationType::DOOROPEN].Add(true, [](APlayerCharacter* character)
		{
			character->CameraShake(character->PlayerDoorCameraShake);
			character->VibrateGamePad(0.6, 0.2);
		});

	NotifyBeginEndEventMap.Add(AnimationType::SAVESTART, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::SAVESTART].Add(false, [](APlayerCharacter* character)
		{
		});
	NotifyBeginEndEventMap[AnimationType::SAVESTART].Add(true, [](APlayerCharacter* character)
		{
			character->PlayerHUD->FadeInAnimation(true);
		});

	NotifyBeginEndEventMap.Add(AnimationType::GAMESTART, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	NotifyBeginEndEventMap[AnimationType::GAMESTART].Add(false, [](APlayerCharacter* character)
		{
			character->LocketSKMesh->SetVisibility(false);
		});
	NotifyBeginEndEventMap[AnimationType::GAMESTART].Add(true, [](APlayerCharacter* character)
		{
		});

	PlayerActionTickMap.Add(PlayerAction::NONE, TMap<ActionType, TFunction<void(APlayerCharacter* character)>>());

	PlayerActionTickMap[PlayerAction::NONE].Add(ActionType::NONE, [](APlayerCharacter* character)
		{
			character->RecoverStamina();
		});

	PlayerActionTickMap[PlayerAction::NONE].Add(ActionType::DODGE, [](APlayerCharacter* character)
		{
			
		});
	PlayerActionTickMap[PlayerAction::NONE].Add(ActionType::ATTACK, PlayerActionTickMap[PlayerAction::NONE][ActionType::DODGE]);
	PlayerActionTickMap[PlayerAction::NONE].Add(ActionType::PARRING, PlayerActionTickMap[PlayerAction::NONE][ActionType::DODGE]);

	PlayerActionTickMap[PlayerAction::NONE].Add(ActionType::MOVE, [](APlayerCharacter* character)
		{
			character->RecoverStamina();
			character->SetPlayerForwardRotAndDir();
			character->SetPlayerRightRotAndDir();
			character->LockOnCameraSettingMap[character->IsLockOn](character);
			//character->LockOnCameraSettingMap[true](character);
			character->PlayerMovement();

		});

	PlayerActionTickMap[PlayerAction::NONE].Add(ActionType::ROTATE, [](APlayerCharacter* character)
		{
			character->SetPlayerForwardRotAndDir();
			character->SetPlayerRightRotAndDir();
		});

	PlayerActionTickMap.Add(PlayerAction::RUN, TMap<ActionType, TFunction<void(APlayerCharacter* character)>>());
	PlayerActionTickMap[PlayerAction::RUN].Add(ActionType::NONE, PlayerActionTickMap[PlayerAction::NONE][ActionType::NONE]);
	PlayerActionTickMap[PlayerAction::RUN].Add(ActionType::DODGE, PlayerActionTickMap[PlayerAction::NONE][ActionType::DODGE]);
	PlayerActionTickMap[PlayerAction::RUN].Add(ActionType::ATTACK, PlayerActionTickMap[PlayerAction::NONE][ActionType::ATTACK]);
	PlayerActionTickMap[PlayerAction::RUN].Add(ActionType::PARRING, PlayerActionTickMap[PlayerAction::NONE][ActionType::PARRING]);
	PlayerActionTickMap[PlayerAction::RUN].Add(ActionType::MOVE, PlayerActionTickMap[PlayerAction::NONE][ActionType::MOVE]);
	PlayerActionTickMap[PlayerAction::RUN].Add(ActionType::HEAL, PlayerActionTickMap[PlayerAction::NONE][ActionType::MOVE]);
	PlayerActionTickMap[PlayerAction::RUN].Add(ActionType::HIT, PlayerActionTickMap[PlayerAction::NONE][ActionType::ATTACK]);

	PlayerActionTickMap.Add(PlayerAction::BEFOREATTACK, TMap<ActionType, TFunction<void(APlayerCharacter* character)>>());
	PlayerActionTickMap[PlayerAction::BEFOREATTACK].Add(ActionType::NONE, PlayerActionTickMap[PlayerAction::NONE][ActionType::NONE]);
	PlayerActionTickMap[PlayerAction::BEFOREATTACK].Add(ActionType::ROTATE, PlayerActionTickMap[PlayerAction::NONE][ActionType::ROTATE]);
	PlayerActionTickMap[PlayerAction::BEFOREATTACK].Add(ActionType::ATTACK, PlayerActionTickMap[PlayerAction::NONE][ActionType::ATTACK]);

	PlayerActionTickMap.Add(PlayerAction::AFTERATTACK, TMap<ActionType, TFunction<void(APlayerCharacter* character)>>());
	PlayerActionTickMap[PlayerAction::AFTERATTACK].Add(ActionType::NONE, PlayerActionTickMap[PlayerAction::NONE][ActionType::NONE]);
	PlayerActionTickMap[PlayerAction::AFTERATTACK].Add(ActionType::DODGE, PlayerActionTickMap[PlayerAction::NONE][ActionType::DODGE]);
	PlayerActionTickMap[PlayerAction::AFTERATTACK].Add(ActionType::ATTACK, PlayerActionTickMap[PlayerAction::NONE][ActionType::ATTACK]);
	PlayerActionTickMap[PlayerAction::AFTERATTACK].Add(ActionType::ROTATE, [](APlayerCharacter* character){});
	PlayerActionTickMap[PlayerAction::AFTERATTACK].Add(ActionType::PARRING, PlayerActionTickMap[PlayerAction::NONE][ActionType::PARRING]);
	PlayerActionTickMap[PlayerAction::AFTERATTACK].Add(ActionType::MOVE, PlayerActionTickMap[PlayerAction::NONE][ActionType::MOVE]);
	PlayerActionTickMap[PlayerAction::AFTERATTACK].Add(ActionType::HEAL, PlayerActionTickMap[PlayerAction::NONE][ActionType::MOVE]);
	PlayerActionTickMap[PlayerAction::AFTERATTACK].Add(ActionType::HIT, PlayerActionTickMap[PlayerAction::NONE][ActionType::ATTACK]);

	

	PlayerActionTickMap.Add(PlayerAction::CANTACT, TMap<ActionType, TFunction<void(APlayerCharacter* character)>>());
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::NONE, [](APlayerCharacter* character)
		{
			if (character->Controller == nullptr)return;
			const FRotator Rotation = character->Controller->GetControlRotation();
			character->YawRotation = FRotator(0, Rotation.Yaw, 0);
			character->PlayerForwardDirection = FRotationMatrix(character->YawRotation).GetUnitAxis(EAxis::X);
			character->YawRotation = FRotator(0, Rotation.Yaw, 0);
			character->SetPlayerForwardRotAndDir();
			character->SetPlayerRightRotAndDir();
		});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::DODGE, [](APlayerCharacter* character)
		{
		});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::PARRING, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::DEAD, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::INTERACTION, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::MOVE, [](APlayerCharacter* character)
		{
		});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::ROTATE, [](APlayerCharacter* character)
		{
		});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::ATTACK, [](APlayerCharacter* character){});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::POWERATTACK, [](APlayerCharacter* character){});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::HIT, [](APlayerCharacter* character){});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::HEAL, [](APlayerCharacter* character){});

	PlayerActionTickMap.Add(PlayerAction::CANATTACK, TMap<ActionType, TFunction<void(APlayerCharacter* character)>>());
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::NONE, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::DODGE, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::PARRING, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::DEAD, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::INTERACTION, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::MOVE, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::ROTATE, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::ATTACK, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::POWERATTACK, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::HIT, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::HEAL, [](APlayerCharacter* character) {});

	PlayerActionTickMap.Add(PlayerAction::CANWALK, TMap<ActionType, TFunction<void(APlayerCharacter* character)>>());
	PlayerActionTickMap[PlayerAction::CANWALK].Add(ActionType::NONE, PlayerActionTickMap[PlayerAction::NONE][ActionType::DODGE]);
	PlayerActionTickMap[PlayerAction::CANWALK].Add(ActionType::DODGE, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::CANWALK].Add(ActionType::PARRING, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::CANWALK].Add(ActionType::DEAD, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::CANWALK].Add(ActionType::INTERACTION, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::CANWALK].Add(ActionType::MOVE, [](APlayerCharacter* character)
		{
			character->LockOnCameraSettingMap[false](character);
			character->SetPlayerForwardRotAndDir();
			character->SetPlayerRightRotAndDir();
			character->PlayerMovement();
		});

	PlayerActionTickMap[PlayerAction::CANWALK].Add(ActionType::HIT, [](APlayerCharacter* character){});
	PlayerActionTickMap[PlayerAction::CANWALK].Add(ActionType::HEAL, [](APlayerCharacter* character)
		{
		});

	PlayerActionTickMap.Add(PlayerAction::SPRINT, TMap<ActionType, TFunction<void(APlayerCharacter* character)>>());
	PlayerActionTickMap[PlayerAction::SPRINT].Add(ActionType::NONE, PlayerActionTickMap[PlayerAction::NONE][ActionType::DODGE]);
	PlayerActionTickMap[PlayerAction::SPRINT].Add(ActionType::DODGE, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::SPRINT].Add(ActionType::PARRING, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::SPRINT].Add(ActionType::DEAD, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::SPRINT].Add(ActionType::INTERACTION, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::SPRINT].Add(ActionType::MOVE, [](APlayerCharacter* character)
		{
			if (!character->DebugMode)
			{
				character->PlayerDataStruct.PlayerStamina = FMath::Clamp(character->PlayerDataStruct.PlayerStamina -= character->PlayerDataStruct.PlayerRunStamina * character->fDeltaTime, 0.0f, 100.0f);
				character->PlayerHUD->SetStamina(character->PlayerDataStruct.PlayerStamina / character->PlayerDataStruct.MaxStamina);
			}	
			
			character->GameInstance->DebugLogWidget->T_PlayerStamina->SetText(FText::AsNumber(character->PlayerDataStruct.PlayerStamina));

			if (character->PlayerDataStruct.PlayerStamina <= 0)
			{
				character->ChangeMontageAnimation(character->MovementAnimMap[character->IsLockOn](character));
				character->SetSpeed(character->SpeedMap[character->IsLockOn][false]);
			}

			character->LockOnCameraSettingMap[false](character);
			character->SetPlayerForwardRotAndDir();
			character->SetPlayerRightRotAndDir();

			if(character->IsLockOn)
			character->YawRotation.Yaw += character->ForwardRotation[character->AxisY][character->AxisX];

			character->PlayerMovement();
		});

	PlayerActionTickMap[PlayerAction::SPRINT].Add(ActionType::HIT, [](APlayerCharacter* character) {});
	PlayerActionTickMap[PlayerAction::SPRINT].Add(ActionType::HEAL, [](APlayerCharacter* character)
		{
		});

	MontageEndEventMap.Add(AnimationType::BATTLEDODGE, [](APlayerCharacter* character)
		{	
			character->SetSpeed(character->SpeedMap[character->IsLockOn][false]);
			character->MontageBlendInTime = 0.1f;
			character->CheckInputKey();
			character->Imotal = false;
		});
	MontageEndEventMap.Add(AnimationType::BASICDODGE, [](APlayerCharacter* character)
		{
			character->SetSpeed(character->SpeedMap[character->IsLockOn][false]);
			character->MontageBlendInTime = 0.1f;
			character->CheckInputKey();
			character->Imotal = false;
		});

	MontageEndEventMap.Add(AnimationType::HEAL, [](APlayerCharacter* character)
		{
			if (character->AnimInstance->BodyBlendAlpha == 1.0f)
			{
				character->SetSpeed(character->SpeedMap[character->IsLockOn][false]);
				character->ChangeMontageAnimation(AnimationType::ENDOFHEAL);
			}
			else
			{
				character->AnimInstance->BodyBlendAlpha = 1.0f;
				character->CheckInputKey();
			}	
		});

	MontageEndEventMap.Add(AnimationType::HIT, [](APlayerCharacter* character)
		{
			character->ComboAttackEnd();
			character->CheckInputKey();
		});
	MontageEndEventMap.Add(AnimationType::HITFRONTLEFT, [](APlayerCharacter* character)
		{
			character->ComboAttackEnd();
			character->CheckInputKey();
		});
	MontageEndEventMap.Add(AnimationType::HITFRONTRIGHT, [](APlayerCharacter* character)
		{
			character->ComboAttackEnd();
			character->CheckInputKey();
		});
	MontageEndEventMap.Add(AnimationType::HITBACKLEFT, [](APlayerCharacter* character)
		{
			character->ComboAttackEnd();
			character->CheckInputKey();
		});
	MontageEndEventMap.Add(AnimationType::HITBACKRIGHT, [](APlayerCharacter* character)
		{
			character->ComboAttackEnd();
			character->CheckInputKey();
		});
	MontageEndEventMap.Add(AnimationType::SUPERHIT, [](APlayerCharacter* character)
		{
			character->ComboAttackEnd();
			character->CheckInputKey();	
			character->Imotal = false;
		});

	MontageEndEventMap.Add(AnimationType::DEAD, [](APlayerCharacter* character)
		{
			//character->AnimInstance->PauseAnimation(character->AnimInstance->GetCurrentActiveMontage());
			character->ChangeMontageAnimation(AnimationType::DEADLOOP);
			character->GetWorldTimerManager().SetTimer(character->DeadTimer, character, &APlayerCharacter::FadeIn, 4.0f);
			character->IsDead = true;
		});

	MontageEndEventMap.Add(AnimationType::PARRING, [](APlayerCharacter* character)
		{
			UGameplayStatics::SetGlobalTimeDilation(character, 1.0f);
			character->ComboAttackEnd();
			character->CheckInputKey();
			character->Imotal = false;
		});

	MontageEndEventMap.Add(AnimationType::ATTACK1, [](APlayerCharacter* character)
		{
			character->ComboAttackEnd();	
			character->CheckInputKey();
		});

	MontageEndEventMap.Add(AnimationType::ATTACK2, MontageEndEventMap[AnimationType::ATTACK1]);
	MontageEndEventMap.Add(AnimationType::ATTACK3, MontageEndEventMap[AnimationType::ATTACK1]);
	MontageEndEventMap.Add(AnimationType::ATTACK4, MontageEndEventMap[AnimationType::ATTACK1]);
	MontageEndEventMap.Add(AnimationType::RUNATTACK, MontageEndEventMap[AnimationType::ATTACK1]);

	MontageEndEventMap.Add(AnimationType::POWERATTACK1, MontageEndEventMap[AnimationType::ATTACK1]);
	MontageEndEventMap.Add(AnimationType::POWERATTACK2, MontageEndEventMap[AnimationType::ATTACK1]);
	MontageEndEventMap.Add(AnimationType::POWERATTACK3, [](APlayerCharacter* character)
		{
			character->MontageEndEventMap[AnimationType::ATTACK1](character);
		});

	MontageEndEventMap.Add(AnimationType::DODGEATTACK, MontageEndEventMap[AnimationType::ATTACK1]);

	MontageEndEventMap.Add(AnimationType::ENDOFHEAL, [](APlayerCharacter* character)
		{
			character->CheckInputKey();
		});

	MontageEndEventMap.Add(AnimationType::SPRINTTURN, [](APlayerCharacter* character)
		{
			if (character->AxisX == 1 && character->AxisY == 1)
			{
				character->SetSpeed(character->SpeedMap[character->IsLockOn][false]);
			}

			character->SetActorRotation(character->YawRotation);
			character->CheckInputKey();
		});

	MontageEndEventMap.Add(AnimationType::RUNATTACK, [](APlayerCharacter* character)
		{
			if (character->AxisX == 1 && character->AxisY == 1)
			{
				character->SetSpeed(character->SpeedMap[character->IsLockOn][false]);
			}

			character->MontageEndEventMap[AnimationType::ATTACK1](character);
		});
	MontageEndEventMap.Add(AnimationType::RUNPOWERATTACK, [](APlayerCharacter* character)
		{
			if (character->AxisX == 1 && character->AxisY == 1)
			{
				character->SetSpeed(character->SpeedMap[character->IsLockOn][false]);
			}
			character->MontageEndEventMap[AnimationType::ATTACK1](character);
		});


	MontageEndEventMap.Add(AnimationType::DOOROPEN, [](APlayerCharacter* character)
		{
			character->ComboAttackEnd();

			character->AxisX = 1;
			character->AxisY = 1;
			character->CheckInputKey();
			character->GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(character, 1.0f);
			character->WeaponMesh->SetVisibility(true);
			character->Imotal = false;
		});


	MontageEndEventMap.Add(AnimationType::BACKSTEP, MontageEndEventMap[AnimationType::BATTLEDODGE]);
	MontageEndEventMap.Add(AnimationType::ENDOFRUN, [](APlayerCharacter* character)
		{
			character->ChangeActionType(ActionType::NONE);
			character->ChangeMontageAnimation(AnimationType::IDLE);
		});
	MontageEndEventMap.Add(AnimationType::ENDOFSPRINT, [](APlayerCharacter* character)
		{
			if (character->AxisX == 1 && character->AxisY == 1)
			{
				character->SetSpeed(character->SpeedMap[character->IsLockOn][false]);
			}
			character->CheckInputKey();			
		});

	MontageEndEventMap.Add(AnimationType::ACTIVESAVEPOINT, [](APlayerCharacter* character)
		{
			character->PlayerHUD->PlayInteractionAnimation(true, EInteractions::rest);
			character->Imotal = false;
			character->ChangeMontageAnimation(AnimationType::ENDOFHEAL);
			character->GetWorld()->GetFirstPlayerController()->EnableInput(character->GetWorld()->GetFirstPlayerController());
		});
	MontageEndEventMap.Add(AnimationType::SAVESTART, [](APlayerCharacter* character)
		{
			character->PlayerHUD->PlayInteractionAnimation(true, EInteractions::close);
			character->ChangeMontageAnimation(AnimationType::SAVELOOP);
			character->RestoreStat();
			character->GetWorldTimerManager().SetTimer(character->DeadTimer, character, &APlayerCharacter::FadeOut, 2.0f);

			
			character->PlayerHUD->PlayExitAnimation(true);

			//character->SaveGameInstance->SaveLoc(character->GetActorLocation());
			//character->SaveGameInstance->SaveRot(character->GetActorRotation());
			//character->SaveGameInstance->SaveHealCount(10);

			//character->SaveGameInstance->Save(character->SaveGameInstance);
		});
	MontageEndEventMap.Add(AnimationType::SAVELOOP, [](APlayerCharacter* character)
		{
			character->ChangeMontageAnimation(AnimationType::SAVELOOP);
		});
	MontageEndEventMap.Add(AnimationType::SAVEEND, [](APlayerCharacter* character)
		{
			character->GetWorld()->GetFirstPlayerController()->EnableInput(character->GetWorld()->GetFirstPlayerController());
			character->PlayerHUD->PlayInteractionAnimation(false);
			character->SpawnLocation = character->GetActorLocation();
			character->SpawnRotation = character->GetActorRotation();
			character->CheckInputKey();
			character->Imotal = false;
		});
	MontageEndEventMap.Add(AnimationType::EXECUTIONBOSS, [](APlayerCharacter* character)
		{
			character->CheckInputKey();
			character->Imotal = false;
			character->TargetCameraBoomLength = character->IsShoulderView ? character->ShoulderViewCameraLength : character->BackViewCameraLength;
		});
	MontageEndEventMap.Add(AnimationType::GAMESTARTLOOP, [](APlayerCharacter* character)
		{
			character->ChangeMontageAnimation(AnimationType::GAMESTARTLOOP);
		});
	MontageEndEventMap.Add(AnimationType::GAMESTART, [](APlayerCharacter* character)
		{
			character->GameInstance->MainMenuWidget->RemoveFromViewport();
			character->PlayerHUD->AddToViewport();
			character->PlayerHUD->ChangeHealCount(character->CurHealCount);
			character->LocketSKMesh->SetVisibility(false);
			character->CheckInputKey();
		});

	MontageEndEventMap.Add(AnimationType::IDLE, [](APlayerCharacter* character)
		{
			character->ChangeMontageAnimation(AnimationType::IDLE);
		});
	MontageEndEventMap.Add(AnimationType::FORWARDWALK, [](APlayerCharacter* character)
		{
			character->ChangeMontageAnimation(AnimationType::FORWARDWALK);
		});
	MontageEndEventMap.Add(AnimationType::FORWARDBATTLEWALK, [](APlayerCharacter* character)
		{
			character->ChangeMontageAnimation(AnimationType::FORWARDBATTLEWALK);
		});
	MontageEndEventMap.Add(AnimationType::LEFTWALK, [](APlayerCharacter* character)
		{
			character->ChangeMontageAnimation(AnimationType::LEFTWALK);
		});
	MontageEndEventMap.Add(AnimationType::RIGHTWALK, [](APlayerCharacter* character)
		{
			character->ChangeMontageAnimation(AnimationType::RIGHTWALK);
		});
	MontageEndEventMap.Add(AnimationType::BACKWALK, [](APlayerCharacter* character)
		{
			character->ChangeMontageAnimation(AnimationType::BACKWALK);
		});
	MontageEndEventMap.Add(AnimationType::FORWARDLEFTWALK, [](APlayerCharacter* character)
		{
			character->ChangeMontageAnimation(AnimationType::FORWARDLEFTWALK);
		});
	MontageEndEventMap.Add(AnimationType::FORWARDRIGHTWALK, [](APlayerCharacter* character)
		{
			character->ChangeMontageAnimation(AnimationType::FORWARDRIGHTWALK);
		});
	MontageEndEventMap.Add(AnimationType::BACKLEFTWALK, [](APlayerCharacter* character)
		{
			character->ChangeMontageAnimation(AnimationType::BACKLEFTWALK);
		});
	MontageEndEventMap.Add(AnimationType::BACKRIGHTWALK, [](APlayerCharacter* character)
		{
			character->ChangeMontageAnimation(AnimationType::BACKRIGHTWALK);
		});

	DodgeAnimationMap.Add(false, [](APlayerCharacter* character)->AnimationType
		{
			if (character->AxisX == 1 && character->AxisY == 1)
			{
				return AnimationType::BACKSTEP;
			}
			else
			{
				if (character->IsLockOn)
				{
					return AnimationType::BATTLEDODGE;
				}
			}
			return AnimationType::BASICDODGE;
		});
	DodgeAnimationMap.Add(true, [](APlayerCharacter* character)->AnimationType
		{
			return character->DodgeDirection[character->AxisY][character->AxisX];
		});

	MovementAnimMap.Add(false, [](APlayerCharacter* character)->AnimationType
		{
			return (character->AxisX == 1 && character->AxisY == 1) ? AnimationType::ENDOFRUN : AnimationType::FORWARDWALK;
		});
	MovementAnimMap.Add(true, [](APlayerCharacter* character)->AnimationType
		{
			return character->MovementAnim[character->AxisY][character->AxisX];
		});


	LockOnCameraSettingMap.Add(true, [](APlayerCharacter* character)
		{
			if(character->IsLockOn)
			character->YawRotation.Yaw = character->GetController()->GetControlRotation().Yaw;
			character->GetCharacterMovement()->bOrientRotationToMovement = false;
		});

	LockOnCameraSettingMap.Add(false, [](APlayerCharacter* character)
		{
			character->GetCharacterMovement()->bOrientRotationToMovement = true;
		});

	InputEventMap.Add(PlayerAction::NONE, TMap<ActionType, TMap<bool, TFunction<void(APlayerCharacter* character)>>>());
	InputEventMap[PlayerAction::NONE].Add(ActionType::DODGE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::NONE].Add(ActionType::ATTACK, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::NONE].Add(ActionType::POWERATTACK, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::NONE].Add(ActionType::PARRING, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::NONE].Add(ActionType::MOVE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::NONE].Add(ActionType::ROTATE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::NONE].Add(ActionType::HEAL, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::NONE].Add(ActionType::INTERACTION, TMap<bool, TFunction<void(APlayerCharacter* character)>>());

	InputEventMap[PlayerAction::NONE][ActionType::DODGE].Add(true, [](APlayerCharacter* character)
		{
			character->Dodge();
		});
	InputEventMap[PlayerAction::NONE][ActionType::DODGE].Add(false, [](APlayerCharacter* character)
		{

		});

	InputEventMap[PlayerAction::NONE][ActionType::ATTACK].Add(true, [](APlayerCharacter* character)
		{
			character->BasicAttack();
		});
	InputEventMap[PlayerAction::NONE][ActionType::ATTACK].Add(false, [](APlayerCharacter* character)
		{

		});

	InputEventMap[PlayerAction::NONE][ActionType::POWERATTACK].Add(true, [](APlayerCharacter* character)
		{
			character->PowerAttack();
		});
	InputEventMap[PlayerAction::NONE][ActionType::POWERATTACK].Add(false, [](APlayerCharacter* character)
		{

		});

	InputEventMap[PlayerAction::NONE][ActionType::PARRING].Add(true, [](APlayerCharacter* character)
		{
			character->Parring();
		});
	InputEventMap[PlayerAction::NONE][ActionType::PARRING].Add(false, [](APlayerCharacter* character)
		{

		});

	InputEventMap[PlayerAction::NONE][ActionType::MOVE].Add(true, [](APlayerCharacter* character)
		{
			character->ChangeActionType(ActionType::MOVE);
			character->ChangeMontageAnimation(character->MovementAnimMap[character->IsLockOn](character));
		});
	InputEventMap[PlayerAction::NONE][ActionType::MOVE].Add(false, [](APlayerCharacter* character)
		{
			character->ChangeMontageAnimation(character->MovementAnimMap[character->IsLockOn](character));
		});

	InputEventMap[PlayerAction::NONE][ActionType::ROTATE].Add(true, [](APlayerCharacter* character)
		{

		});
	InputEventMap[PlayerAction::NONE][ActionType::ROTATE].Add(false, [](APlayerCharacter* character)
		{

		});

	InputEventMap[PlayerAction::NONE][ActionType::HEAL].Add(true, [](APlayerCharacter* character)
		{
			if (character->CurHealCount > 0)
			{
				character->SetSpeed(character->PlayerDataStruct.PlayerWalkSpeed);
				character->UseItem();
			}			
		});
	InputEventMap[PlayerAction::NONE][ActionType::HEAL].Add(false, [](APlayerCharacter* character)
		{

		});

	InputEventMap[PlayerAction::NONE][ActionType::INTERACTION].Add(true, [](APlayerCharacter* character)
		{

		});
	InputEventMap[PlayerAction::NONE][ActionType::INTERACTION].Add(false, [](APlayerCharacter* character)
		{

		});

	InputEventMap.Add(PlayerAction::BEFOREATTACK, TMap<ActionType, TMap<bool, TFunction<void(APlayerCharacter* character)>>>());
	InputEventMap[PlayerAction::BEFOREATTACK].Add(ActionType::DODGE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::BEFOREATTACK].Add(ActionType::ATTACK, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::BEFOREATTACK].Add(ActionType::POWERATTACK, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::BEFOREATTACK].Add(ActionType::PARRING, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::BEFOREATTACK].Add(ActionType::MOVE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::BEFOREATTACK].Add(ActionType::ROTATE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::BEFOREATTACK].Add(ActionType::HEAL, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::BEFOREATTACK].Add(ActionType::INTERACTION, TMap<bool, TFunction<void(APlayerCharacter* character)>>());

	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::DODGE].Add(true,   [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::DODGE].Add(false,  [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::ATTACK].Add(true,  [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::ATTACK].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::POWERATTACK].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::POWERATTACK].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::PARRING].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::PARRING].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::MOVE].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::MOVE].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::ROTATE].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::ROTATE].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::HEAL].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::HEAL].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::INTERACTION].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::INTERACTION].Add(false, [](APlayerCharacter* character) {});

	InputEventMap.Add(PlayerAction::RUN, TMap<ActionType, TMap<bool, TFunction<void(APlayerCharacter* character)>>>());
	InputEventMap[PlayerAction::RUN].Add(ActionType::DODGE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::RUN].Add(ActionType::ATTACK, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::RUN].Add(ActionType::POWERATTACK, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::RUN].Add(ActionType::PARRING, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::RUN].Add(ActionType::MOVE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::RUN].Add(ActionType::ROTATE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::RUN].Add(ActionType::HEAL, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::RUN].Add(ActionType::INTERACTION, TMap<bool, TFunction<void(APlayerCharacter* character)>>());

	InputEventMap[PlayerAction::RUN][ActionType::DODGE].Add(true, [](APlayerCharacter* character)
		{
		});
	InputEventMap[PlayerAction::RUN][ActionType::DODGE].Add(false, [](APlayerCharacter* character)
		{
			if (!character->IsSprint)
			{
				character->Dodge();
			}
		});
	InputEventMap[PlayerAction::RUN][ActionType::ATTACK].Add(true, InputEventMap[PlayerAction::NONE][ActionType::ATTACK][true]);
	InputEventMap[PlayerAction::RUN][ActionType::ATTACK].Add(false, InputEventMap[PlayerAction::NONE][ActionType::ATTACK][false]);
	InputEventMap[PlayerAction::RUN][ActionType::POWERATTACK].Add(true, InputEventMap[PlayerAction::NONE][ActionType::POWERATTACK][true]);
	InputEventMap[PlayerAction::RUN][ActionType::POWERATTACK].Add(false, InputEventMap[PlayerAction::NONE][ActionType::POWERATTACK][false]);
	InputEventMap[PlayerAction::RUN][ActionType::PARRING].Add(true, InputEventMap[PlayerAction::NONE][ActionType::PARRING][true]);
	InputEventMap[PlayerAction::RUN][ActionType::PARRING].Add(false, InputEventMap[PlayerAction::NONE][ActionType::PARRING][false]);
	InputEventMap[PlayerAction::RUN][ActionType::MOVE].Add(true, [](APlayerCharacter* character)
		{
			character->ChangeActionType(ActionType::MOVE);
			if(character->IsLockOn)
			character->ChangeMontageAnimation(character->MovementAnimMap[true](character));
		});
	InputEventMap[PlayerAction::RUN][ActionType::MOVE].Add(false, [](APlayerCharacter* character)
		{
			AnimationType anitype = character->MovementAnimMap[character->IsLockOn](character);
			if (anitype != AnimationType::FORWARDWALK)
			{
				character->ChangeMontageAnimation(anitype);
			}
		});
	InputEventMap[PlayerAction::RUN][ActionType::ROTATE].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::RUN][ActionType::ROTATE].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::RUN][ActionType::HEAL].Add(true, InputEventMap[PlayerAction::NONE][ActionType::HEAL][true]);
	InputEventMap[PlayerAction::RUN][ActionType::HEAL].Add(false, InputEventMap[PlayerAction::NONE][ActionType::HEAL][false]);
	InputEventMap[PlayerAction::RUN][ActionType::INTERACTION].Add(true, InputEventMap[PlayerAction::NONE][ActionType::INTERACTION][true]);
	InputEventMap[PlayerAction::RUN][ActionType::INTERACTION].Add(false, InputEventMap[PlayerAction::NONE][ActionType::INTERACTION][false]);

	InputEventMap.Add(PlayerAction::AFTERATTACK, TMap<ActionType, TMap<bool, TFunction<void(APlayerCharacter* character)>>>());
	InputEventMap[PlayerAction::AFTERATTACK].Add(ActionType::DODGE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::AFTERATTACK].Add(ActionType::ATTACK, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::AFTERATTACK].Add(ActionType::POWERATTACK, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::AFTERATTACK].Add(ActionType::PARRING, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::AFTERATTACK].Add(ActionType::MOVE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::AFTERATTACK].Add(ActionType::ROTATE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::AFTERATTACK].Add(ActionType::HEAL, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::AFTERATTACK].Add(ActionType::INTERACTION, TMap<bool, TFunction<void(APlayerCharacter* character)>>());

	InputEventMap[PlayerAction::AFTERATTACK][ActionType::DODGE].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::DODGE].Add(false, InputEventMap[PlayerAction::RUN][ActionType::DODGE][false]);
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::ATTACK].Add(true,  InputEventMap[PlayerAction::NONE][ActionType::ATTACK][true]);
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::ATTACK].Add(false, InputEventMap[PlayerAction::NONE][ActionType::ATTACK][false]);
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::POWERATTACK].Add(true, InputEventMap[PlayerAction::NONE][ActionType::POWERATTACK][true]);
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::POWERATTACK].Add(false, InputEventMap[PlayerAction::NONE][ActionType::POWERATTACK][false]);
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::PARRING].Add(true, InputEventMap[PlayerAction::NONE][ActionType::PARRING][true]);
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::PARRING].Add(false, InputEventMap[PlayerAction::NONE][ActionType::PARRING][false]);
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::MOVE].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::MOVE].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::ROTATE].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::ROTATE].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::HEAL].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::HEAL].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::INTERACTION].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::INTERACTION].Add(false, [](APlayerCharacter* character) {});

	InputEventMap.Add(PlayerAction::CANWALK, TMap<ActionType, TMap<bool, TFunction<void(APlayerCharacter* character)>>>());
	InputEventMap[PlayerAction::CANWALK].Add(ActionType::DODGE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANWALK].Add(ActionType::ATTACK, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANWALK].Add(ActionType::POWERATTACK, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANWALK].Add(ActionType::PARRING, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANWALK].Add(ActionType::MOVE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANWALK].Add(ActionType::ROTATE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANWALK].Add(ActionType::HEAL, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANWALK].Add(ActionType::INTERACTION, TMap<bool, TFunction<void(APlayerCharacter* character)>>());

	InputEventMap[PlayerAction::CANWALK][ActionType::DODGE].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANWALK][ActionType::DODGE].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANWALK][ActionType::ATTACK].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANWALK][ActionType::ATTACK].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANWALK][ActionType::POWERATTACK].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANWALK][ActionType::POWERATTACK].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANWALK][ActionType::PARRING].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANWALK][ActionType::PARRING].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANWALK][ActionType::MOVE].Add(true, [](APlayerCharacter* character)
		{
			character->SetSpeed(character->PlayerDataStruct.PlayerWalkSpeed);
			character->AnimInstance->BodyBlendAlpha = 0.0f;
		});
	InputEventMap[PlayerAction::CANWALK][ActionType::MOVE].Add(false, [](APlayerCharacter* character)
		{			
			if(character->AxisY == 1 && character->AxisX == 1)
				character->AnimInstance->BodyBlendAlpha = 1.0f;

		});
	InputEventMap[PlayerAction::CANWALK][ActionType::ROTATE].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANWALK][ActionType::ROTATE].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANWALK][ActionType::HEAL].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANWALK][ActionType::HEAL].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANWALK][ActionType::INTERACTION].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANWALK][ActionType::INTERACTION].Add(false, [](APlayerCharacter* character) {});

	InputEventMap.Add(PlayerAction::CANTACT, TMap<ActionType, TMap<bool, TFunction<void(APlayerCharacter* character)>>>());
	InputEventMap[PlayerAction::CANTACT].Add(ActionType::DODGE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANTACT].Add(ActionType::ATTACK, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANTACT].Add(ActionType::POWERATTACK, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANTACT].Add(ActionType::PARRING, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANTACT].Add(ActionType::MOVE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANTACT].Add(ActionType::ROTATE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANTACT].Add(ActionType::HEAL, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANTACT].Add(ActionType::INTERACTION, TMap<bool, TFunction<void(APlayerCharacter* character)>>());

	InputEventMap[PlayerAction::CANTACT][ActionType::DODGE].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANTACT][ActionType::DODGE].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANTACT][ActionType::ATTACK].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANTACT][ActionType::ATTACK].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANTACT][ActionType::POWERATTACK].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANTACT][ActionType::POWERATTACK].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANTACT][ActionType::PARRING].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANTACT][ActionType::PARRING].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANTACT][ActionType::MOVE].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANTACT][ActionType::MOVE].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANTACT][ActionType::ROTATE].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANTACT][ActionType::ROTATE].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANTACT][ActionType::HEAL].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANTACT][ActionType::HEAL].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANTACT][ActionType::INTERACTION].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANTACT][ActionType::INTERACTION].Add(false, [](APlayerCharacter* character) {});

	InputEventMap.Add(PlayerAction::CANATTACK, TMap<ActionType, TMap<bool, TFunction<void(APlayerCharacter* character)>>>());
	InputEventMap[PlayerAction::CANATTACK].Add(ActionType::DODGE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANATTACK].Add(ActionType::ATTACK, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANATTACK].Add(ActionType::POWERATTACK, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANATTACK].Add(ActionType::PARRING, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANATTACK].Add(ActionType::MOVE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANATTACK].Add(ActionType::ROTATE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANATTACK].Add(ActionType::HEAL, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::CANATTACK].Add(ActionType::INTERACTION, TMap<bool, TFunction<void(APlayerCharacter* character)>>());

	InputEventMap[PlayerAction::CANATTACK][ActionType::DODGE].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::DODGE].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::ATTACK].Add(true, [](APlayerCharacter* character)
		{
			if (character->UseStamina(character->PlayerUseStaminaMap[ActionType::ATTACK]))
			{
				character->PlayerCurAttackIndex++;
				character->ChangeActionType(ActionType::ATTACK);
				character->ChangeMontageAnimation(AnimationType::DODGEATTACK);
				character->CanNextAttack = false;
				AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[24].ObjClass, character->GetActorLocation(), FRotator::ZeroRotator);
			}
		});
	InputEventMap[PlayerAction::CANATTACK][ActionType::ATTACK].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::POWERATTACK].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::POWERATTACK].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::PARRING].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::PARRING].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::MOVE].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::MOVE].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::ROTATE].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::ROTATE].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::HEAL].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::HEAL].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::INTERACTION].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::INTERACTION].Add(false, [](APlayerCharacter* character) {});

	InputEventMap.Add(PlayerAction::SPRINT, TMap<ActionType, TMap<bool, TFunction<void(APlayerCharacter* character)>>>());
	InputEventMap[PlayerAction::SPRINT].Add(ActionType::DODGE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::SPRINT].Add(ActionType::ATTACK, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::SPRINT].Add(ActionType::POWERATTACK, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::SPRINT].Add(ActionType::PARRING, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::SPRINT].Add(ActionType::MOVE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::SPRINT].Add(ActionType::ROTATE, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::SPRINT].Add(ActionType::HEAL, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	InputEventMap[PlayerAction::SPRINT].Add(ActionType::INTERACTION, TMap<bool, TFunction<void(APlayerCharacter* character)>>());

	InputEventMap[PlayerAction::SPRINT][ActionType::DODGE].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::SPRINT][ActionType::DODGE].Add(false, [](APlayerCharacter* character)
		{
			if (character->CurActionType == ActionType::MOVE && 
				character->AnimInstance->PlayerAnimationType != AnimationType::ENDOFSPRINT &&
				character->AnimInstance->PlayerAnimationType != AnimationType::HEAL)
			{
				character->Run();
			}
		});
	InputEventMap[PlayerAction::SPRINT][ActionType::ATTACK].Add(true, [](APlayerCharacter* character)
		{
			if (character->UseStamina(character->PlayerUseStaminaMap[ActionType::POWERATTACK]))
			{				
				character->PlayerCurAttackIndex++;
				character->ChangeActionType(ActionType::ATTACK);
				character->ChangeMontageAnimation(AnimationType::RUNPOWERATTACK);
				character->CanNextAttack = false;
				AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[25].ObjClass, character->GetActorLocation(), FRotator::ZeroRotator);
			}
		});
	InputEventMap[PlayerAction::SPRINT][ActionType::ATTACK].Add(false, InputEventMap[PlayerAction::NONE][ActionType::ATTACK][false]);
	InputEventMap[PlayerAction::SPRINT][ActionType::POWERATTACK].Add(true, [](APlayerCharacter* character)
		{
			if (character->UseStamina(character->PlayerUseStaminaMap[ActionType::POWERATTACK]))
			{
				character->PlayerCurAttackIndex++;
				character->ChangeActionType(ActionType::ATTACK);
				character->ChangeMontageAnimation(AnimationType::RUNPOWERATTACK);
				character->CanNextAttack = false;
				AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[25].ObjClass, character->GetActorLocation(), FRotator::ZeroRotator);
			}
		});
	InputEventMap[PlayerAction::SPRINT][ActionType::POWERATTACK].Add(false, InputEventMap[PlayerAction::NONE][ActionType::POWERATTACK][false]);
	InputEventMap[PlayerAction::SPRINT][ActionType::PARRING].Add(true, InputEventMap[PlayerAction::NONE][ActionType::PARRING][true]);
	InputEventMap[PlayerAction::SPRINT][ActionType::PARRING].Add(false, InputEventMap[PlayerAction::NONE][ActionType::PARRING][false]);
	InputEventMap[PlayerAction::SPRINT][ActionType::MOVE].Add(true, [](APlayerCharacter* character)
		{
		});
	InputEventMap[PlayerAction::SPRINT][ActionType::MOVE].Add(false, [](APlayerCharacter* character)
		{
			if (character->AxisX == 1 && character->AxisY == 1)
			{
				character->ChangeMontageAnimation(AnimationType::ENDOFSPRINT);
			}
		});
	InputEventMap[PlayerAction::SPRINT][ActionType::ROTATE].Add(true, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::SPRINT][ActionType::ROTATE].Add(false, [](APlayerCharacter* character) {});
	InputEventMap[PlayerAction::SPRINT][ActionType::HEAL].Add(true, InputEventMap[PlayerAction::NONE][ActionType::HEAL][true]);
	InputEventMap[PlayerAction::SPRINT][ActionType::HEAL].Add(false,InputEventMap[PlayerAction::NONE][ActionType::HEAL][false]);
	InputEventMap[PlayerAction::SPRINT][ActionType::INTERACTION].Add(true,  InputEventMap[PlayerAction::NONE][ActionType::INTERACTION][true]);
	InputEventMap[PlayerAction::SPRINT][ActionType::INTERACTION].Add(false, InputEventMap[PlayerAction::NONE][ActionType::INTERACTION][false]);

	PlayerEventFuncMap.Add(AnimationType::SUPERHIT, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	PlayerEventFuncMap[AnimationType::SUPERHIT].Add(true, [](APlayerCharacter* character)
		{
			if (character->PlayerDataStruct.CharacterHp <= 0)
			{
				character->GetWorldTimerManager().SetTimer(character->DeadTimer, character, &APlayerCharacter::FadeIn, 2.0f);
				character->PlayerDead(true);
			}
		});
	PlayerEventFuncMap[AnimationType::SUPERHIT].Add(false, [](APlayerCharacter* character)
		{
		});

	PlayerEventFuncMap.Add(AnimationType::EXECUTIONBOSS, TMap<bool, TFunction<void(APlayerCharacter* character)>>());
	PlayerEventFuncMap[AnimationType::EXECUTIONBOSS].Add(true, [](APlayerCharacter* character)
		{
			character->ComboAttackEnd();
			character->ExecutionCharacter->TakeDamage(character->PlayerDataStruct.PlayerExecutionFirstDamage, character->CharacterDamageEvent, nullptr, character);
			character->VibrateGamePad(0.4f, 0.4f);
			AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[31].ObjClass, character->ExecutionCharacter->GetActorLocation() + FVector(0, 0, 20.0f), FRotator::ZeroRotator);
		});
	PlayerEventFuncMap[AnimationType::EXECUTIONBOSS].Add(false, [](APlayerCharacter* character)
		{
			UCombatManager::GetInstance().HitMonsterInfoArray.AddUnique(character->ExecutionCharacter);
			character->ExecutionCharacter->TakeDamage(character->PlayerDataStruct.PlayerExecutionSecondDamage, character->CharacterDamageEvent, nullptr, character);
			character->VibrateGamePad(0.4f, 0.4f);
			AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[31].ObjClass, character->ExecutionCharacter->GetActorLocation() + FVector(0, 0, 20.0f), FRotator::ZeroRotator);
		});}

APlayerCharacter::~APlayerCharacter()
{
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	DebugMode = false;
	LocketSKMesh->SetVisibility(true);

	TArray<UActorComponent*> asd = GetComponentsByTag(UActorSequenceComponent::StaticClass(), FName("BossExecution"));
	
	BossExecutionSequence = Cast<UActorSequenceComponent>(asd[0]);
	BossExecutionSequncePlayer = BossExecutionSequence->GetSequencePlayer();
	
	asd = GetComponentsByTag(UActorSequenceComponent::StaticClass(), FName("Start"));
	GameStartSequence = Cast<UActorSequenceComponent>(asd[0]);
	GameStartSequncePlayer = GameStartSequence->GetSequencePlayer();
	
	asd = GetComponentsByTag(UActorSequenceComponent::StaticClass(), FName("Parrying"));
	BossParryingSequncePlayer = Cast<UActorSequenceComponent>(asd[0])->GetSequencePlayer();

	if (IsValid(PlayerUIClass))
	{
		PlayerHUD = Cast<UPlayerHUD>(CreateWidget(GetWorld(), PlayerUIClass));		
	}


	if (IsValid(PlayerUIClass))
	{
		UserSettingUI = Cast<UUserSettingUI>(CreateWidget(GetWorld(), UserSettingUIClass));
		//UserSettingUI->AddToViewport();
	}

	CurHealCount = PlayerDataStruct.MaxHealCount;

	SpeedMap.Add(false, TMap<bool, float>());
	SpeedMap[false].Add(false, PlayerDataStruct.CharacterOriginSpeed);
	SpeedMap[false].Add(true,  PlayerDataStruct.PlayerRunSpeed);

	SpeedMap.Add(true, TMap<bool, float>());
	SpeedMap[true].Add(false, PlayerDataStruct.PlayerLockOnMoveSpeed);
	SpeedMap[true].Add(true, PlayerDataStruct.PlayerRunSpeed);

	GameInstance->InitInstance();
	GameInstance->InitDefaultSetting();
	GameInstance->MainMenuWidget->StartButton->OnClicked.AddDynamic(this, &APlayerCharacter::PlayStartAnimation);
	
	GameInstance->MainMenuWidget->AddToViewport();

	TargetSocketOffset = CameraBoom1->SocketOffset;

	Imotal = false;
	TargetCompArray.Empty();
	TargetComp = nullptr;

	RotSpeed = 10.0f;

	GetCharacterMovement()->MaxWalkSpeed = PlayerDataStruct.CharacterOriginSpeed;
	
	bUseControllerRotationYaw = false;

	GameInstance->DebugLogWidget->T_PlayerHP->SetText(FText::AsNumber(PlayerDataStruct.CharacterHp));
	AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstance->PlayerAnimationType = AnimationType::IDLE;
	ChangeMontageAnimation(AnimationType::GAMESTARTLOOP);

	CurBulletCount = PlayerDataStruct.MaxBulletCount;

	RotSpeed = 10.0f;

	LockOnCameraSettingMap[true](this);

	FollowCamera = Cast<UCameraComponent>(GetComponentByClass(UCameraComponent::StaticClass()));

	ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SwordTrailComp->Deactivate();
	DeactivateSMOverlap();
	DeactivateRightWeapon();

	AnimInstance->BodyBlendAlpha = 1.0f;

	GameStartSequncePlayer->Play();
	GameStartSequncePlayer->Pause();

	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnWeaponOverlapBegin);
	ExecutionTrigger->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnExecutionOverlapBegin);
	ExecutionTrigger->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnExecutionOverlapEnd);
	ParryingCollision1->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnParryingOverlapBegin);
	WeaponOverlapStaticMeshCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnSMOverlapBegin);
	WeaponOverlapStaticMeshCollision->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnSMOverlapEnd);
	TargetDetectionCollison->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnEnemyDetectionBeginOverlap);
	TargetDetectionCollison->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnEnemyDetectionEndOverlap);

	ShoulderView(IsShoulderView);
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UCombatManager::GetInstance().MonsterInfoArray.Empty();
	UCombatManager::GetInstance().HitMonsterInfoArray.Empty();
}

void APlayerCharacter::UseItem()
{
	CurHealCount--;
	PlayerHUD->ChangeHealCount(CurHealCount);
	ChangeActionType(ActionType::MOVE);
	ChangeMontageAnimation(AnimationType::HEAL);
	if (AxisX != 1 || AxisY != 1)
	{
		AnimInstance->BodyBlendAlpha = 0.0f;
	}
	else
	{
		AnimInstance->BodyBlendAlpha = 1.0f;
	}
}

void APlayerCharacter::SetPlayerForwardRotAndDir()
{
	PlayerForwardDirection = FVector::ZeroVector;
	if (AxisY != 1)
	{		
		if (Controller == nullptr)return;
		const FRotator Rotation = Controller->GetControlRotation();
		YawRotation = FRotator(0, Rotation.Yaw, 0);
		PlayerForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		if (!IsLockOn)
		{
			YawRotation = FRotator(0, Rotation.Yaw + ForwardRotation[AxisY][AxisX], 0);
		}
		else
		{
			YawRotation = FRotator(0, Rotation.Yaw, 0);
		}
	}
}

void APlayerCharacter::SetPlayerRightRotAndDir()
{
	PlayerRightDirection = FVector::ZeroVector;
	if (AxisX != 1)
	{
				if (Controller == nullptr)return;
const FRotator Rotation = Controller->GetControlRotation();
		YawRotation = FRotator(0, Rotation.Yaw, 0);
		PlayerRightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		if (!IsLockOn)
		{
			YawRotation = FRotator(0, Rotation.Yaw + ForwardRotation[AxisY][AxisX], 0);
		}
		else
		{
			YawRotation = FRotator(0, Rotation.Yaw, 0);
		}
	}
}

void APlayerCharacter::EndSprint()
{
	InputEventMap[PlayerAction::SPRINT][ActionType::DODGE][false](this);
	IsSprint = false;
}

void APlayerCharacter::RotatePlayer()
{
	SetActorRotation(FMath::Lerp(GetActorRotation(), YawRotation, RotSpeed * fDeltaTime));
}

void APlayerCharacter::ResumeMontage()
{
	if (MontageMap.Contains(AnimInstance->PlayerAnimationType))
		AnimInstance->ResumeMontage(MontageMap[AnimInstance->PlayerAnimationType]);
}

void APlayerCharacter::HitStop()
{
	Super::HitStop();

	if (MontageMap.Contains(AnimInstance->PlayerAnimationType))
		AnimInstance->PauseAnimation(MontageMap[AnimInstance->PlayerAnimationType]);
}

void APlayerCharacter::Dodge()
{
	if( UseStamina(PlayerUseStaminaMap[ActionType::DODGE]))
	{
		AnimInstance->StopMontage(MontageMap[AnimInstance->PlayerAnimationType]);
		if (MontageEndEventMap.Contains(AnimInstance->PlayerAnimationType))
			MontageEndEventMap[AnimInstance->PlayerAnimationType](this);

		DeactivateRightWeapon();
		DeactivateSMOverlap();
		ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ComboAttackEnd();
		SwordTrailComp->Deactivate();
		CanNextAttack = false;
		LockOnCameraSettingMap[false](this);
		UCombatManager::GetInstance().ActivateCollider();
		SetPlayerForwardRotAndDir();
		SetPlayerRightRotAndDir();

		if (IsLockOn)
		{
			YawRotation += FRotator(0, ForwardRotation[AxisY][AxisX], 0);
		}
		CameraBoom1->CameraLagSpeed = 10.0f;

		ChangeActionType(ActionType::DODGE);
		
			
		ChangeMontageAnimation(DodgeAnimationMap[false](this));
	}
}

void APlayerCharacter::RestoreStat()
{
	PlayerDataStruct.PlayerStamina = PlayerDataStruct.MaxStamina;
	PlayerHUD->SetStamina(PlayerDataStruct.PlayerStamina / PlayerDataStruct.MaxStamina);
	PlayerDataStruct.CharacterHp = PlayerDataStruct.CharacterMaxHp;
	PlayerHUD->SetHP(PlayerDataStruct.CharacterHp / PlayerDataStruct.CharacterMaxHp);
	CurHealCount = PlayerDataStruct.MaxHealCount;
	PlayerHUD->ChangeHealCount(CurHealCount);

	CurBulletCount = PlayerDataStruct.MaxBulletCount;
	
}

void APlayerCharacter::LockOn()
{
	if (AnimInstance->PlayerAnimationType == AnimationType::EXECUTIONBOSS || 
		AnimInstance->PlayerAnimationType == AnimationType::SAVEEND)return;
	IsLockOn = !IsLockOn;

	if (IsLockOn)
	{
		GetCompsInScreen(TargetCompArray);
		GetFirstTarget();
		if (TargetComp == nullptr)
		{
			IsLockOn = false;
			return;
		}

		Cast<ABaseCharacter>(TargetComp->GetOwner())->ActivateLockOnImage(true);

		if (AnimInstance->PlayerAnimationType != AnimationType::HEAL)
		{
			SetSpeed(SpeedMap[IsLockOn][IsSprint]);
		}

		CurRotateIndex = 1;
	}
	else
	{
		if(TargetComp != nullptr)
		Cast<ABaseCharacter>(TargetComp->GetOwner())->ActivateLockOnImage(false);
		TargetComp = nullptr;
		if (AnimInstance->PlayerAnimationType != AnimationType::HEAL)
			SetSpeed(SpeedMap[IsLockOn][IsSprint]);
		CurRotateIndex = 0;
	}

	if (CurActionType == ActionType::MOVE && AnimInstance->PlayerAnimationType != AnimationType::ENDOFSPRINT && 
		AnimInstance->PlayerAnimationType != AnimationType::HEAL)
	{
		CheckInputKey();
	}
}

void APlayerCharacter::GetFirstTarget()
{
	TargetComp = nullptr;
	float Distance;
	float ClosestDistance = 999999999;
	FVector CompLocation;
	FVector CameraLocation = FollowCamera->GetComponentLocation();
	for (int32 i = 0; i < TargetCompInScreenArray.Num(); i++)
	{
		CompLocation = TargetCompInScreenArray[i]->GetComponentLocation();
		Distance = FVector::DistSquared(CameraLocation, CompLocation);
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			if(TargetCompInScreenArray[i]->GetOwner()->IsActorTickEnabled() || TargetCompInScreenArray[i]->GetOwner()->ActorHasTag("Static"))
			TargetComp = TargetCompInScreenArray[i];
		}
	}
}

void APlayerCharacter::GetCompsInScreen(TArray<UPrimitiveComponent*>Array)
{
	FVector2D ScreenLocation;
	FVector CompLocation;
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)return;
	float ViewportSizeX = GEngine->GameViewport->Viewport->GetSizeXY().X;
	float ViewprotSizeY = GEngine->GameViewport->Viewport->GetSizeXY().Y;

	TargetCompInScreenArray.Empty();

	for (int32 i = 0; i < Array.Num(); i++)
	{
		if (!Array[i]->GetOwner()->IsActorTickEnabled())continue;
		CompLocation = Array[i]->GetComponentLocation();
		if (PlayerController->ProjectWorldLocationToScreen(CompLocation, ScreenLocation))
		{
			if (ScreenLocation.X > 0 && ScreenLocation.X < ViewportSizeX
				&& ScreenLocation.Y > 0 && ScreenLocation.Y < ViewprotSizeY)
			{
				TargetCompInScreenArray.AddUnique(Array[i]);				
			}
		}
	}
}

void APlayerCharacter::ChangeTarget(CameraDirection CamDirection)
{
	if (AnimInstance->PlayerAnimationType == AnimationType::EXECUTIONBOSS)return;
	ChangeTargetTime = 0.0f;
	GetCompsInScreen(TargetCompArray);
	FVector CameraLocation = CameraBoom1->GetComponentLocation();
	FVector TargetDir = (TargetComp->GetComponentLocation() - CameraLocation).GetSafeNormal();
	TArray<UPrimitiveComponent*> TargetArray;

	for (int32 i = 0; i < TargetCompInScreenArray.Num(); i++)
	{
		if (TargetComp == TargetCompInScreenArray[i])continue;
		FVector Direction = (TargetCompInScreenArray[i]->GetComponentLocation() - CameraLocation).GetSafeNormal();
		FVector Cross = FVector::CrossProduct(TargetDir, Direction);

		if ((CamDirection == CameraDirection::LEFT && Cross.Z < 0.f)
			|| (CamDirection == CameraDirection::RIGHT && Cross.Z > 0.f))
		{
			TargetArray.Add(TargetCompInScreenArray[i]);
		}
	}


	if (TargetArray.Num() < 1)return;
	int32 TargetIdx = 0;
	for (int32 i = 1; i < TargetArray.Num(); i++)
	{
		float TargetDot = FVector::DotProduct(TargetDir, (TargetArray[TargetIdx]->GetComponentLocation() - CameraLocation).GetSafeNormal());
		float NextDot = FVector::DotProduct(TargetDir, (TargetArray[i]->GetComponentLocation() - CameraLocation).GetSafeNormal());

		if (NextDot > TargetDot)
			TargetIdx = i;
	}

	TargetComp = TargetArray[TargetIdx];
	TargetArray.Empty();
}

void APlayerCharacter::ShoulderView(bool Value)
{
	IsShoulderView = Value;
	if (IsShoulderView)
	{
		TargetSocketOffset = ShoulderViewSocketOffset;
		TargetCameraBoomLength = ShoulderViewCameraLength;

	}
	else
	{
		TargetSocketOffset = BackViewSocketOffset;
		TargetCameraBoomLength = BackViewCameraLength;
	}
}

void APlayerCharacter::SetInputType(bool IsPad)
{
	PlayerHUD->SetGamepad(IsPad);
}

void APlayerCharacter::Sprint()
{
	SetSpeed(SpeedMap[IsLockOn][true]);
	ChangeMontageAnimation(AnimationType::SPRINT);
}

void APlayerCharacter::Run()
{
	SetSpeed(SpeedMap[IsLockOn][false]);
	InputEventMap[PlayerAction::NONE][ActionType::MOVE][true](this);
}

void APlayerCharacter::RecoverStamina()
{
	PlayerDataStruct.PlayerStamina = FMath::Clamp(PlayerDataStruct.PlayerStamina += fDeltaTime * PlayerDataStruct.StaminaRecovery, 0.0f, 100.0f);
	PlayerHUD->SetStamina(PlayerDataStruct.PlayerStamina / PlayerDataStruct.MaxStamina);
	GameInstance->DebugLogWidget->T_PlayerStamina->SetText(FText::AsNumber(PlayerDataStruct.PlayerStamina));
}

bool APlayerCharacter::UseStamina(float value)
{
	if (DebugMode == true)return true;
	if (PlayerDataStruct.PlayerStamina > 0)
	{
		PlayerDataStruct.PlayerStamina = FMath::Clamp(PlayerDataStruct.PlayerStamina -= value, 0.0f, 100.0f);
		PlayerHUD->DecreaseStaminaGradual(this, PlayerDataStruct.PlayerStamina / PlayerDataStruct.MaxStamina);
		GameInstance->DebugLogWidget->T_PlayerStamina->SetText(FText::AsNumber(PlayerDataStruct.PlayerStamina));

		return true;
	}
	return false;
}

void APlayerCharacter::CheckInputKey()
{
	if (AxisX != 1  || AxisY != 1)
	{
		if(AxisY == 2)
		TargetCameraBoomLength = 350.0f;

		ChangeActionType(ActionType::MOVE);
		if (!IsSprint)
		{
			Run();
		}
		else
		{
			Sprint();
		}
	}
	else if(AnimInstance->PlayerAnimationType != AnimationType::ENDOFHEAL)
	{
		SetSpeed(SpeedMap[IsLockOn][false]);
		ChangeActionType(ActionType::NONE);
		ChangeMontageAnimation(AnimationType::IDLE);
	}
}

void APlayerCharacter::SetSpeed(float speed)
{
	if (speed == PlayerDataStruct.PlayerWalkSpeed)
	{
		RotSpeed = 1.0f;
	}
	else
	{
		RotSpeed = 5.0f;
	}
	CameraBoom1->CameraLagSpeed = speed * 0.02f;
	GetCharacterMovement()->MaxWalkSpeed = speed;
}

void APlayerCharacter::BeforeAttackNotify(bool value)
{
	if(value)
	{

		ChangePlayerAction(PlayerAction::CANTACT);
		ChangeActionType(ActionType::NONE);
	}
	else
	{
		if (PlayerAttackType == ActionType::ATTACK)
			AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[24].ObjClass, GetActorLocation(), FRotator::ZeroRotator);
		else
			AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[25].ObjClass, GetActorLocation(), FRotator::ZeroRotator);

		ChangePlayerAction(PlayerAction::CANTACT);
		ChangeActionType(ActionType::ATTACK);
	}
}

void APlayerCharacter::AfterAttackNotify(bool value)
{
	if (value == true)
	{
		ChangePlayerAction(PlayerAction::AFTERATTACK);
		ChangeActionType(ActionType::ATTACK);
		CanNextAttack = true;
	}
}

void APlayerCharacter::IsNotifyActive(bool value)
{
	if (NotifyBeginEndEventMap.Contains(AnimInstance->PlayerAnimationType))
	{
		if (NotifyBeginEndEventMap[AnimInstance->PlayerAnimationType].Contains(value))
		{
			NotifyBeginEndEventMap[AnimInstance->PlayerAnimationType][value](this);
		}
	}
}

void APlayerCharacter::CheckMontageEndNotify()
{
	if (MontageEndEventMap.Contains(AnimInstance->PlayerAnimationType))
	{
		MontageEndEventMap[AnimInstance->PlayerAnimationType](this);
	}
}


void APlayerCharacter::LookTarget()
{
	if (TargetComp == nullptr)
	{
		return;
	}
	TargetDirection = TargetComp->GetComponentLocation() - FVector(0, 0, 100) - GetActorLocation();
	Difference = FRotationMatrix::MakeFromX(TargetDirection).Rotator();
	GetController()->SetControlRotation(FMath::Lerp(GetController()->GetControlRotation(), Difference, fDeltaTime * 2.0f));

	if (AnimInstance->PlayerAnimationType != AnimationType::BATTLEDODGE
		&& AnimInstance->PlayerAnimationType != AnimationType::SPRINT
		&& AnimInstance->PlayerAnimationType != AnimationType::DEADLOOP
		&& AnimInstance->PlayerAnimationType != AnimationType::DEADLOOP2)
		YawRotation.Yaw = GetController()->GetControlRotation().Yaw;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	fDeltaTime = DeltaTime;
	ChangeTargetTime += fDeltaTime;

	
	PlayerActionTickMap[PlayerCurAction][CurActionType](this);

	LookTarget();

	if (AnimInstance->PlayerAnimationType == AnimationType::EXECUTIONBOSS)
	{
		TargetDirection = ExecutionCharacter->GetActorLocation() - GetActorLocation();
		Difference = FRotationMatrix::MakeFromX(TargetDirection).Rotator();
		YawRotation = Difference;

		TargetDirection = ExecutionCharacter->GetActorLocation() - FollowCamera->GetComponentLocation();
		FRotator CameraRotation = FRotationMatrix::MakeFromX(TargetDirection).Rotator();
		FollowCamera->SetWorldRotation(FMath::Lerp(FollowCamera->GetComponentRotation(), CameraRotation, DeltaTime * 5.0f));
	}
	else if(AnimInstance->PlayerAnimationType != AnimationType::GAMESTART
		&& AnimInstance->PlayerAnimationType != AnimationType::GAMESTARTLOOP && IsLockOn)
	{
		FollowCamera->SetWorldRotation(FMath::Lerp(FollowCamera->GetComponentRotation(), GetWorld()->GetFirstPlayerController()->GetControlRotation(), DeltaTime));
	}

	RotatePlayer();

	SetActorRotation(FRotator(0, GetActorRotation().Yaw, 0));
	FollowCamera->SetWorldRotation(FRotator(FollowCamera->GetComponentRotation().Pitch, FollowCamera->GetComponentRotation().Yaw, 0));
	CameraBoom1->TargetArmLength = FMath::Lerp(CameraBoom1->TargetArmLength, TargetCameraBoomLength, DeltaTime * 2.0f);
	CameraBoom1->SocketOffset = FMath::Lerp(CameraBoom1->SocketOffset, TargetSocketOffset, DeltaTime * 2.0f);


	if (!IsShoulderView)
	{
		CameraBoom1->SetWorldLocation(GetActorLocation());
	}
}

void APlayerCharacter::RespawnCharacter()
{
	Super::RespawnCharacter();
	GetWorldTimerManager().SetTimer(DeadTimer, this, &APlayerCharacter::FadeOut, 4.0f);
	ASoundManager::GetInstance().StartBGMSound();

	AnimInstance->BodyBlendAlpha = 1.0f;

	Imotal = false;
	TargetCompArray.Empty();

	CameraBoom1->SetWorldRotation(FRotator::ZeroRotator);
	YawRotation = SpawnRotation;
	ChangeActionType(ActionType::NONE);
	ChangeMontageAnimation(AnimationType::IDLE);

	RestoreStat();

	for (int32 i = 0; i < UCombatManager::GetInstance().MonsterInfoArray.Num(); i++)
	{
		UCombatManager::GetInstance().MonsterInfoArray[i]->RespawnCharacter();
	}
	SetSpeed(SpeedMap[false][false]);
}

void APlayerCharacter::EventNotify(bool value)
{
	if (PlayerEventFuncMap.Contains(AnimInstance->PlayerAnimationType))
	{
		PlayerEventFuncMap[AnimInstance->PlayerAnimationType][value](this);
	}
}

void APlayerCharacter::PlayExecutionAnimation()
{
	if (!IsLockOn)
	{
		LockOn();
	}

	//ExecuteDirection = ExecutionCharacter->GetActorLocation() - GetActorLocation();
	//ExecuteDirection.Normalize();

	//ExecuteLocation = GetActorLocation() - ExecuteDirection * 50.0f;
	//SetActorLocation(ExecuteLocation);

	DeactivateRightWeapon();
	DeactivateSMOverlap();
	SwordTrailComp->Deactivate();
	ComboAttackEnd();
	ChangeMontageAnimation(AnimationType::EXECUTIONBOSS);
	ExecutionCharacter->PlayExecutionAnimation();
	BossExecutionSequncePlayer->Play();

	Imotal = true;
}

void APlayerCharacter::Parring()
{
	if (UseStamina(PlayerUseStaminaMap[ActionType::PARRING]))
	{
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[24].ObjClass, GetActorLocation(), FRotator::ZeroRotator);
		ChangeActionType(ActionType::PARRING);
		ChangeMontageAnimation(AnimationType::PARRING);
	}
}

void APlayerCharacter::OnEnemyDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TargetCompArray.Add(OtherComp);
}

void APlayerCharacter::OnEnemyDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{	
}

void APlayerCharacter::PlayerMovement()
{
	DiagonalSpeed = PlayerDataStruct.PlayerRunSpeed * sqrt(0.5f) + PlayerDataStruct.PlayerRunSpeed * sqrt(0.5f);
		
	AddMovementInput(PlayerForwardDirection, ForwardMovementValue * DiagonalSpeed * fDeltaTime);
	AddMovementInput(PlayerRightDirection, RightMovementValue * DiagonalSpeed * fDeltaTime);
}

void APlayerCharacter::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	HitStop();

	CameraShake(PlayerCameraShake);

	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[17].ObjClass, GetActorLocation(), FRotator::ZeroRotator);

	if (PlayerDataStruct.DamageList.Contains(AnimInstance->PlayerAnimationType))
	{
		OtherActor->TakeDamage(PlayerDataStruct.DamageList[AnimInstance->PlayerAnimationType].Damage, CharacterDamageEvent, nullptr, this);
		VibrateGamePad(PlayerDataStruct.DamageList[AnimInstance->PlayerAnimationType].VibrateIntensity, PlayerDataStruct.DamageList[AnimInstance->PlayerAnimationType].VibrateDuration);
	}	
	UCombatManager::GetInstance().HitMonsterInfoArray.AddUnique(Cast<ABaseCharacter>(OtherActor));

	if (HitEffectRotatorList.Contains(AnimInstance->PlayerAnimationType))
	{
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[0].ObjClass, OverlappedComponent->GetComponentLocation(), YawRotation - HitEffectRotatorList[AnimInstance->PlayerAnimationType]);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[1].ObjClass, OverlappedComponent->GetComponentLocation(), YawRotation - HitEffectRotatorList[AnimInstance->PlayerAnimationType]);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[3].ObjClass, OverlappedComponent->GetComponentLocation(), YawRotation - HitEffectRotatorList[AnimInstance->PlayerAnimationType]);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[3].ObjClass, OverlappedComponent->GetComponentLocation(), YawRotation - HitEffectRotatorList[AnimInstance->PlayerAnimationType]);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[1].ObjClass, OverlappedComponent->GetComponentLocation(), YawRotation - HitEffectRotatorList[AnimInstance->PlayerAnimationType]);

		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[5].ObjClass, OverlappedComponent->GetComponentLocation(), YawRotation - HitEffectRotatorList[AnimInstance->PlayerAnimationType]);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[31].ObjClass, OtherActor->GetActorLocation() + FVector(0, 0, 20.0f), FRotator::ZeroRotator);
	}		
}

void APlayerCharacter::OnSMOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SwordVFXSpawn();
}


void APlayerCharacter::OnSMOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GetWorldTimerManager().ClearTimer(SMOverlapTimerHandler);
}

void APlayerCharacter::OnExecutionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CanExecution = true;
	ExecutionCharacter = Cast<ABaseCharacter>(OtherActor);
}

void APlayerCharacter::OnExecutionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CanExecution = false;
}


void APlayerCharacter::OnParryingOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsLockOn)
	{
		LockOn();
	}

	UGameplayStatics::SetGlobalTimeDilation(this, .5f);
	Imotal = true;
	BossParryingSequncePlayer->Play();
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[15].ObjClass, GetActorLocation(), FRotator(90, 180, 0));
	ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayerCharacter::SetCameraLocation(FVector Location)
{
}


void APlayerCharacter::ActivateRightWeapon()
{
	Super::ActivateRightWeapon();
}

void APlayerCharacter::ActivateSMOverlap()
{
	Super::ActivateSMOverlap();
}

void APlayerCharacter::DeactivateRightWeapon()
{
	Super::DeactivateRightWeapon();
}

void APlayerCharacter::DeactivateSMOverlap()
{
	Super::DeactivateSMOverlap();
}

void APlayerCharacter::Attack()
{
	if (ExecutionCharacter && CanExecution)
	{
		if (ExecutionCharacter->IsStun && ExecutionCharacter->CanExecution)
		{
			PlayExecutionAnimation();
			return;
		}		
	}

	if (CanNextAttack)
	{
		if (UseStamina(PlayerUseStaminaMap[ActionType::ATTACK]))
		{
			Imotal = false;
			PlayerAttackType = ActionType::ATTACK;
			ComboAttackStart();
			CanNextAttack = false;
		}
	}
}

void APlayerCharacter::BasicAttack()
{
	Attack();
}

void APlayerCharacter::PowerAttack()
{
	if (ExecutionCharacter && CanExecution)
	{
		if (ExecutionCharacter->IsStun)
		{
			PlayExecutionAnimation();
			return;
		}
	}

	if (CanNextAttack)
	{
		if (UseStamina(PlayerUseStaminaMap[ActionType::POWERATTACK]))
		{
			Imotal = false;
			PlayerAttackType = ActionType::POWERATTACK;
			ComboAttackStart();
			CanNextAttack = false;
		}
	}
}

void APlayerCharacter::FadeIn()
{
	PlayerHUD->FadeInAnimation(true);
	GetWorldTimerManager().SetTimer(DeadTimer, this, &APlayerCharacter::RespawnCharacter, 2.0f);
}

void APlayerCharacter::ComboAttackEnd()
{	
	PlayerCurAttackIndex = 0;
	CanNextAttack = true;
}

void APlayerCharacter::ComboAttackStart()
{	
	ChangeActionType(ActionType::ROTATE);

	if (PlayerCurAttackIndex >= PlayerMaxAttackIndex[PlayerAttackType])
	{
		if (PlayerAttackType == ActionType::POWERATTACK &&
			PlayerCurAttackIndex == 3)
		{
			PlayerCurAttackIndex = 1;
		}
		else
		{
			PlayerCurAttackIndex = 0;
		}		
	}
	ChangeMontageAnimation(IntToEnumMap[PlayerAttackType][PlayerCurAttackIndex++]);
}

void APlayerCharacter::ChangeMontageAnimation(AnimationType type)
{
	AnimInstance->StopMontage(MontageMap[AnimInstance->PlayerAnimationType]);
	MontageMap[type]->BlendIn = MontageBlendInTime;		
	AnimInstance->PlayMontage(MontageMap[type]);
	MontageBlendInTime = 0.2f;
	AnimInstance->PlayerAnimationType = type;
	ChangePlayerAction(PlayerEnumToAnimTypeMap[type]);
}

void APlayerCharacter::ChangeActionType(ActionType type)
{
	CurActionType = type;
}

void APlayerCharacter::ChangePlayerAction(PlayerAction action)
{
	PlayerCurAction = action;
}

void APlayerCharacter::SetSprint()
{
	IsSprint = true;
	if (CurActionType == ActionType::MOVE
		&& AnimInstance->PlayerAnimationType != AnimationType::HEAL && PlayerCurAction != PlayerAction::CANTACT)
	{
		Sprint();
	}
}

void APlayerCharacter::FadeOut()
{
	PlayerHUD->FadeInAnimation(false);
	GetWorld()->GetFirstPlayerController()->EnableInput(GetWorld()->GetFirstPlayerController());
}

void APlayerCharacter::PlayStartAnimation()
{
	GameStartSequncePlayer->Play();
	MontageBlendInTime = 0.0f;
	ChangeMontageAnimation(AnimationType::GAMESTART);
	AJesusPlayerController* controller = Cast<AJesusPlayerController>(GetWorld()->GetFirstPlayerController());
	controller->bShowMouseCursor = false;
	LocketSKMesh->GetAnimInstance()->Montage_Play(MontageMap[AnimationType::NONE]);
}

void APlayerCharacter::PlayerDead(bool IsFly)
{
	if (IsLockOn)
	{
		LockOn();
	}
	if (TargetComp)
	{
		Cast<ABaseCharacter>(TargetComp->GetOwner())->ActivateLockOnImage(false);
	}
	GetWorld()->GetFirstPlayerController()->DisableInput(GetWorld()->GetFirstPlayerController());
	Imotal = true;
	PlayerHUD->PlayDied(true);
	ChangeActionType(ActionType::DEAD);
	MontageBlendInTime = 0.0f;
	IsFly ? ChangeMontageAnimation(AnimationType::DEADLOOP2) : ChangeMontageAnimation(AnimationType::DEAD);
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Imotal || DebugMode)
	{
		return 0;
	}

	Imotal = false;
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


	PlayerDataStruct.CharacterHp -= DamageAmount;
	PlayerHUD->DecreaseHPGradual(this, PlayerDataStruct.CharacterHp / PlayerDataStruct.CharacterMaxHp);
	GameInstance->DebugLogWidget->T_PlayerHP->SetText(FText::AsNumber(PlayerDataStruct.CharacterHp));
	VibrateGamePad(0.2f, 0.2f);
	CameraShake(PlayerCameraShake);
	AnimInstance->BodyBlendAlpha = 1.0f;

	FVector HitDir = DamageCauser->GetActorLocation() - GetActorLocation();
	if (DamageAmount > 50)
	{
		YawRotation = HitDir.Rotation();
		ChangeMontageAnimation(AnimationType::SUPERHIT);
		Imotal = true;
		DeactivateRightWeapon();
		DeactivateSMOverlap();
		ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ComboAttackEnd();
		SwordTrailComp->Deactivate();
		if (PlayerDataStruct.CharacterHp <= 0)
			ASoundManager::GetInstance().PlaySoundWithCymbalSound(3);

		return DamageAmount;
	}

	if (DamageAmount > 10)
	{
		UCombatManager::GetInstance().ActivateCollider();

		AnimInstance->StopMontage(MontageMap[AnimInstance->PlayerAnimationType]);
		if (MontageEndEventMap.Contains(AnimInstance->PlayerAnimationType))
			MontageEndEventMap[AnimInstance->PlayerAnimationType](this);

		DeactivateRightWeapon();
		DeactivateSMOverlap();
		ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ComboAttackEnd();
		SwordTrailComp->Deactivate();		

		ChangeActionType(ActionType::HIT);
		float DotProduct = FVector::DotProduct(GetActorRotation().Vector(), HitDir);
		FVector CrossProduct = FVector::CrossProduct(GetActorRotation().Vector(), HitDir);

		float AngleInRadians = FMath::Acos(DotProduct / (GetActorRotation().Vector().Size() * HitDir.Size()));
		float AngleInDegrees = FMath::RadiansToDegrees(AngleInRadians);

		if (CrossProduct.Z > 0.0f)
		{
			if (AngleInDegrees >= 90)
			{
				ChangeMontageAnimation(AnimationType::HITBACKRIGHT);
			}
			else
			{
				ChangeMontageAnimation(AnimationType::HITFRONTRIGHT);
			}
		}
		else if (CrossProduct.Z < 0.0f)
		{
			if (AngleInDegrees >= 90)
			{
				ChangeMontageAnimation(AnimationType::HITBACKLEFT);
			}
			else
			{
				ChangeMontageAnimation(AnimationType::HITFRONTLEFT);
			}
		}
	}

	if (PlayerDataStruct.CharacterHp <= 0)
	{
		ASoundManager::GetInstance().PlaySoundWithCymbalSound(3);
		PlayerDead(false);
	}
	return DamageAmount;
}
