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
#include "../JesusSaveGame.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "..\UI\PlayerHUD.h"
#include "../LevelLightingManager.h"
#include <SERVAME/UI/PlayerSoulStatUI.h>

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
	CameraBoom1->SetupAttachment(GetMesh(), FName("Ref_SIMON_COM"));
	CameraBoom1->TargetArmLength = 0.5f;
	CameraBoom1->bUsePawnControlRotation = true; 

	HeadBoneLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Head Bone Location"));
	HeadBoneLocation->SetupAttachment(GetMesh(), FName("Ref_Bip001-Head"));

	TargetDetectionCollison = CreateDefaultSubobject<USphereComponent>(TEXT("Enemy Detection Collider"));
	TargetDetectionCollison->SetupAttachment(RootComponent);
	TargetDetectionCollison->SetCollisionProfileName("Weapon");
	TargetDetectionCollison->SetSphereRadius(500.0f);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerWeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), FName("Weapon_bone"));

	ShieldMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield Mesh"));
	ShieldMeshComp->SetupAttachment(GetMesh(), FName("POINT_SHIELD"));

	ShieldAttackOverlap = CreateDefaultSubobject<UBoxComponent>(TEXT("Shield Attack Overlap"));
	ShieldAttackOverlap->SetupAttachment(GetMesh());

	ExecutionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Execution Trigger"));
	ExecutionTrigger->SetupAttachment(RootComponent);
	ExecutionTrigger->SetCollisionProfileName("Execution Trigger");

	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Weapon Box"));
	WeaponCollision->SetupAttachment(WeaponMesh);
	WeaponCollision->SetCollisionProfileName("Weapon");

	SkillCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Skill Collision"));
	SkillCollisionComp->SetupAttachment(WeaponMesh);
	SkillCollisionComp->SetCollisionProfileName("Skill Collision");

	ParryingCollision1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Parrying Collision"));
	ParryingCollision1->SetupAttachment(GetMesh());
	ParryingCollision1->SetCollisionProfileName("Parrying");

	WeaponOverlapStaticMeshCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Box"));
	WeaponOverlapStaticMeshCollision->SetupAttachment(WeaponMesh);
	WeaponOverlapStaticMeshCollision->SetCollisionProfileName("Weapon");

	SwordTrailComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Sword Trail"));
	SwordTrailComp->SetupAttachment(GetMesh(), FName("Weapon_bone"));
	SwordTrailComp->SetCollisionProfileName("Sword Trail");

	SkillTrailComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SKill Trail"));
	SkillTrailComp->SetupAttachment(GetMesh(), FName("Weapon_bone"));
	SkillTrailComp->SetCollisionProfileName("Skill Trail");

	SkillAuraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SKill Aura"));
	SkillAuraComp->SetupAttachment(GetMesh(), FName("Weapon_bone"));
	SkillAuraComp->SetCollisionProfileName("Skill Aura");

	ShieldEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Shield Effect Comp"));
	ShieldEffectComp->SetupAttachment(GetMesh(), FName("POINT_SHIELD"));
	ShieldEffectComp->SetCollisionProfileName("Shield Effect Comp");


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

	SkillRotateLUT.Add(-45.0f);
	SkillRotateLUT.Add(0.0f);
	SkillRotateLUT.Add(45.0f);

	HitEffectRotatorList.Add(AnimationType::ATTACK1, FRotator(50, 90.0f, 0)   );
	HitEffectRotatorList.Add(AnimationType::ATTACK2, FRotator(0, -90.0f, 0.0f));
	HitEffectRotatorList.Add(AnimationType::ATTACK3, FRotator(0.0f, 180.0f, 0.0f));
	HitEffectRotatorList.Add(AnimationType::ATTACK4, FRotator(0, -90.0f, 0.0f));
	HitEffectRotatorList.Add(AnimationType::POWERATTACK1, FRotator(45.0f, 90.0f, 0.0f));
	HitEffectRotatorList.Add(AnimationType::POWERATTACK2, FRotator(45.0f, -90.0f, 0.0f));
	HitEffectRotatorList.Add(AnimationType::POWERATTACK3, FRotator(90.0f, 0.0f, 0.0f));
	HitEffectRotatorList.Add(AnimationType::RUNATTACK, FRotator(0, 90.0f, 0.0f));
	HitEffectRotatorList.Add(AnimationType::RUNPOWERATTACK, FRotator(0.0f, 90.0f, 0.0f));
	HitEffectRotatorList.Add(AnimationType::DODGEATTACK, FRotator(0, 180.0f, 0.0f));
	HitEffectRotatorList.Add(AnimationType::SKILL1, FRotator(50, 90.0f, 0)   );
	HitEffectRotatorList.Add(AnimationType::SKILL2, FRotator(0, -90.0f, 0.0f));

	IntToEnumMap.Add(ActionType::ATTACK, TMap<int32, AnimationType>());
	IntToEnumMap[ActionType::ATTACK].Add(0, AnimationType::ATTACK1);
	IntToEnumMap[ActionType::ATTACK].Add(1, AnimationType::ATTACK2);
	IntToEnumMap[ActionType::ATTACK].Add(2, AnimationType::ATTACK3);
	IntToEnumMap[ActionType::ATTACK].Add(3, AnimationType::ATTACK4);

	IntToEnumMap.Add(ActionType::POWERATTACK, TMap<int32, AnimationType>());
	IntToEnumMap[ActionType::POWERATTACK].Add(0, AnimationType::POWERATTACK1);
	IntToEnumMap[ActionType::POWERATTACK].Add(1, AnimationType::POWERATTACK2);
	IntToEnumMap[ActionType::POWERATTACK].Add(2, AnimationType::POWERATTACK3);

	IntToEnumMap.Add(ActionType::SKILL, TMap<int32, AnimationType>());
	IntToEnumMap[ActionType::SKILL].Add(0, AnimationType::SKILL1);
	IntToEnumMap[ActionType::SKILL].Add(1, AnimationType::SKILL2);

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
	PlayerEnumToAnimTypeMap.Add(AnimationType::SHIELDMOVE, PlayerAction::CANWALK);
	PlayerEnumToAnimTypeMap.Add(AnimationType::SPRINT, PlayerAction::SPRINT);

	PlayerEnumToAnimTypeMap.Add(AnimationType::BATTLEDODGE, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::BASICDODGE, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::BACKSTEP, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::PARRING, PlayerAction::CANTACT);

	PlayerEnumToAnimTypeMap.Add(AnimationType::SHIELDSTART, PlayerAction::CANWALK);
	PlayerEnumToAnimTypeMap.Add(AnimationType::SHIELDLOOP, PlayerAction::CANWALK);
	PlayerEnumToAnimTypeMap.Add(AnimationType::SHIELDEND, PlayerAction::NONE);
	PlayerEnumToAnimTypeMap.Add(AnimationType::EOSTOEXECUTION, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::SHIELDKNOCKBACK, PlayerAction::CANTACT);


	PlayerEnumToAnimTypeMap.Add(AnimationType::SHIELDATTACK, PlayerAction::CANTACT);


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
	PlayerEnumToAnimTypeMap.Add(AnimationType::SKILL1, PlayerAction::BEFOREATTACK);
	PlayerEnumToAnimTypeMap.Add(AnimationType::SKILL2, PlayerAction::BEFOREATTACK);

	PlayerEnumToAnimTypeMap.Add(AnimationType::SAVESTART, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::SAVELOOP, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::SAVEEND, PlayerAction::CANTACT);
	PlayerEnumToAnimTypeMap.Add(AnimationType::ACTIVESAVEPOINT, PlayerAction::CANTACT);

	StatCurrentIdxMap.Add(EPlayerStatType::STRENGTH, 0);
	StatCurrentIdxMap.Add(EPlayerStatType::HP, 0);
	StatCurrentIdxMap.Add(EPlayerStatType::STAMINA, 0);

	NotifyBeginEndEventMap.Add(AnimationType::NONE, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap.Add(AnimationType::IDLE, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap.Add(AnimationType::FORWARDWALK, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap.Add(AnimationType::LEFTWALK, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap.Add(AnimationType::RIGHTWALK, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap.Add(AnimationType::BACKWALK, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap.Add(AnimationType::FORWARDLEFTWALK, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap.Add(AnimationType::FORWARDRIGHTWALK, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap.Add(AnimationType::BACKLEFTWALK, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap.Add(AnimationType::BACKRIGHTWALK, TMap<bool, TFunction<void()>>());

	NotifyBeginEndEventMap.Add(AnimationType::BATTLEDODGE, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::BATTLEDODGE].Add(true, [&]()
		{
			Imotal = true;
		});
	NotifyBeginEndEventMap[AnimationType::BATTLEDODGE].Add(false, [&]()
		{
			PlayerCurAction = PlayerAction::CANATTACK;
			Imotal = false;
			ComboAttackEnd();
		});

	NotifyBeginEndEventMap.Add(AnimationType::BASICDODGE, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::BASICDODGE].Add(true, [&]()
		{
			Imotal = true;
		});
	NotifyBeginEndEventMap[AnimationType::BASICDODGE].Add(false, [&]()
		{
			PlayerCurAction = PlayerAction::CANATTACK;
			Imotal = false;
			ComboAttackEnd();
		});

	NotifyBeginEndEventMap.Add(AnimationType::BACKSTEP, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::BACKSTEP].Add(true, [&]()
		{
			Imotal = true;

		});
	NotifyBeginEndEventMap[AnimationType::BACKSTEP].Add(false, [&]()
		{
			PlayerCurAction = PlayerAction::CANATTACK;
			Imotal = false;
			ComboAttackEnd();
		});

	NotifyBeginEndEventMap.Add(AnimationType::HEAL, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::HEAL].Add(true, [&]()
		{
		});
	NotifyBeginEndEventMap[AnimationType::HEAL].Add(false, [&]()
		{
			PlayerDataStruct.CharacterHp = FMath::Clamp(PlayerDataStruct.CharacterHp += PlayerDataStruct.PlayerHealValue, 0.0f, PlayerDataStruct.CharacterMaxHp);
			PlayerHUD->SetHP(PlayerDataStruct.CharacterHp / PlayerDataStruct.CharacterMaxHp);
			GameInstance->DebugLogWidget->T_PlayerHP->SetText(FText::AsNumber(PlayerDataStruct.CharacterHp));
		});

	NotifyBeginEndEventMap.Add(AnimationType::ATTACK1, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::ATTACK1].Add(true, [&]()
		{
			UCombatManager::GetInstance().ActivateCollider();
			ActivateSMOverlap();
			ActivateRightWeapon();
			SwordTrailComp->Activate();
		});

	NotifyBeginEndEventMap[AnimationType::ATTACK1].Add(false, [&]()
		{
			DeactivateRightWeapon();
			DeactivateSMOverlap();
			SwordTrailComp->Deactivate();
		});

	NotifyBeginEndEventMap.Add(AnimationType::POWERATTACK1, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::POWERATTACK1].Add(true, [&]()
	{
			NotifyBeginEndEventMap[AnimationType::ATTACK1][true]();
			CameraShake(PlayerCameraShake);
			VibrateGamePad(0.2f, 0.2f);
		});
	NotifyBeginEndEventMap[AnimationType::POWERATTACK1].Add(false, [&]()
		{
			DeactivateRightWeapon();
			DeactivateSMOverlap();			
			SwordTrailComp->Deactivate();
		});

	NotifyBeginEndEventMap.Add(AnimationType::ATTACK2, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::ATTACK2].Add(true, NotifyBeginEndEventMap[AnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[AnimationType::ATTACK2].Add(false, NotifyBeginEndEventMap[AnimationType::ATTACK1][false]);

	NotifyBeginEndEventMap.Add(AnimationType::ATTACK3, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::ATTACK3].Add(true, NotifyBeginEndEventMap[AnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[AnimationType::ATTACK3].Add(false, NotifyBeginEndEventMap[AnimationType::ATTACK1][false]);

	NotifyBeginEndEventMap.Add(AnimationType::ATTACK4, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::ATTACK4].Add(true, NotifyBeginEndEventMap[AnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[AnimationType::ATTACK4].Add(false, NotifyBeginEndEventMap[AnimationType::ATTACK1][false]);

	NotifyBeginEndEventMap.Add(AnimationType::RUNATTACK, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::RUNATTACK].Add(true, NotifyBeginEndEventMap[AnimationType::POWERATTACK1][true]);
	NotifyBeginEndEventMap[AnimationType::RUNATTACK].Add(false, NotifyBeginEndEventMap[AnimationType::ATTACK1][false]);

	NotifyBeginEndEventMap.Add(AnimationType::RUNPOWERATTACK, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::RUNPOWERATTACK].Add(true, NotifyBeginEndEventMap[AnimationType::POWERATTACK1][true]);
	NotifyBeginEndEventMap[AnimationType::RUNPOWERATTACK].Add(false, NotifyBeginEndEventMap[AnimationType::ATTACK1][false]);

	NotifyBeginEndEventMap.Add(AnimationType::POWERATTACK2, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::POWERATTACK2].Add(true, NotifyBeginEndEventMap[AnimationType::POWERATTACK1][true]);
	NotifyBeginEndEventMap[AnimationType::POWERATTACK2].Add(false, NotifyBeginEndEventMap[AnimationType::POWERATTACK1][false]);

	NotifyBeginEndEventMap.Add(AnimationType::POWERATTACK3, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::POWERATTACK3].Add(true, NotifyBeginEndEventMap[AnimationType::POWERATTACK1][true]);
	NotifyBeginEndEventMap[AnimationType::POWERATTACK3].Add(false, [&]()
		{
			NotifyBeginEndEventMap[AnimationType::POWERATTACK1][false]();
			PlayerCurAttackIndex = 0;
			CameraShake(PlayerCameraShake);
			VibrateGamePad(PlayerDataStruct.DamageList[AnimationType::POWERATTACK1].VibrateIntensity,
			PlayerDataStruct.DamageList[AnimationType::POWERATTACK1].VibrateDuration);
		});

	NotifyBeginEndEventMap.Add(AnimationType::DODGEATTACK, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::DODGEATTACK].Add(true, NotifyBeginEndEventMap[AnimationType::ATTACK1][true]);
	NotifyBeginEndEventMap[AnimationType::DODGEATTACK].Add(false, [&]()
		{
			NotifyBeginEndEventMap[AnimationType::ATTACK1][false]();
			PlayerCurAttackIndex = 0;
		});

	NotifyBeginEndEventMap.Add(AnimationType::PARRING, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::PARRING].Add(true, [&]()
		{
			ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		});
	NotifyBeginEndEventMap[AnimationType::PARRING].Add(false, [&]()
		{
			ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		});

	NotifyBeginEndEventMap.Add(AnimationType::HIT, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::HIT].Add(true, [&]()
		{
			PlayerCurAction = PlayerAction::AFTERATTACK;
		});
	NotifyBeginEndEventMap[AnimationType::HIT].Add(false, [&]()
		{

		});

	NotifyBeginEndEventMap.Add(AnimationType::HITFRONTLEFT, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::HITFRONTLEFT].Add(true, [&]()
		{
			PlayerCurAction = PlayerAction::AFTERATTACK;
		});
	NotifyBeginEndEventMap[AnimationType::HITFRONTLEFT].Add(false, [&]()
		{

		});

	NotifyBeginEndEventMap.Add(AnimationType::HITFRONTRIGHT, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::HITFRONTRIGHT].Add(true, [&]()
		{
			PlayerCurAction = PlayerAction::AFTERATTACK;
		});
	NotifyBeginEndEventMap[AnimationType::HITFRONTRIGHT].Add(false, [&]()
		{

		});

	NotifyBeginEndEventMap.Add(AnimationType::HITBACKLEFT, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::HITBACKLEFT].Add(true, [&]()
		{
			PlayerCurAction = PlayerAction::AFTERATTACK;
		});
	NotifyBeginEndEventMap[AnimationType::HITBACKLEFT].Add(false, [&]()
		{

		});

	NotifyBeginEndEventMap.Add(AnimationType::SHIELDKNOCKBACK, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::SHIELDKNOCKBACK].Add(true, [&]()
		{
			PlayerCurAction = PlayerAction::AFTERATTACK;
		});
	NotifyBeginEndEventMap[AnimationType::SHIELDKNOCKBACK].Add(false, [&]()
		{

		});

	NotifyBeginEndEventMap.Add(AnimationType::HITBACKRIGHT, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::HITBACKRIGHT].Add(true, [&]()
		{
			PlayerCurAction = PlayerAction::AFTERATTACK;
		});
	NotifyBeginEndEventMap[AnimationType::HITBACKRIGHT].Add(false, [&]()
		{

		});


	NotifyBeginEndEventMap.Add(AnimationType::EXECUTIONBOSS, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::EXECUTIONBOSS].Add(true, [&]()
		{
			CameraShake(PlayerCameraShake);
			VibrateGamePad(0.2f, 0.2f);
		});
	NotifyBeginEndEventMap[AnimationType::EXECUTIONBOSS].Add(false, [&]()
		{

		});

	NotifyBeginEndEventMap.Add(AnimationType::SUPERHIT, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::SUPERHIT].Add(true, [&]()
		{
			PlayerCurAction = PlayerAction::AFTERATTACK;
		});
	NotifyBeginEndEventMap[AnimationType::SUPERHIT].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(AnimationType::DOOROPEN, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::DOOROPEN].Add(false, [&]()
		{
			CameraShake(PlayerDoorCameraShake);
			VibrateGamePad(0.6, 0.2);
		});
	NotifyBeginEndEventMap[AnimationType::DOOROPEN].Add(true, [&]()
		{
			CameraShake(PlayerDoorCameraShake);
			VibrateGamePad(0.6, 0.2);
		});

	NotifyBeginEndEventMap.Add(AnimationType::SAVESTART, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::SAVESTART].Add(false, [&]()
		{
		});
	NotifyBeginEndEventMap[AnimationType::SAVESTART].Add(true, [&]()
		{
			PlayerHUD->FadeInAnimation(true);
		});

	NotifyBeginEndEventMap.Add(AnimationType::GAMESTART, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::GAMESTART].Add(false, [&]()
		{
			LocketSKMesh->SetVisibility(false);
		});
	NotifyBeginEndEventMap[AnimationType::GAMESTART].Add(true, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(AnimationType::SKILL1, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::SKILL1].Add(false, [&]()
		{
			DeactivateRightWeapon();
			SkillTrailComp->SetVisibility(false);
			SkillAuraComp->Deactivate();
			SkillAuraComp->SetVisibility(false);
		});
	NotifyBeginEndEventMap[AnimationType::SKILL1].Add(true, [&]()
		{
			UCombatManager::GetInstance().ActivateCollider();
			AObjectPool& objectpool = AObjectPool::GetInstance();
			CameraShake(PlayerCameraShake);
			VibrateGamePad(0.4f, 0.4f);

			for (int32 i = 0; i < SkillCount; i++)
			{
				FRotator CurrentRotation = GetActorRotation();
				CurrentRotation.Yaw -= SkillRotateLUT[i];
				FVector Location = GetActorLocation() + CurrentRotation.Vector() * SkillDistance;
				objectpool.SpawnObject(objectpool.ObjectArray[43].ObjClass, Location, FRotator::ZeroRotator);
			}
			SkillCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		});

	NotifyBeginEndEventMap.Add(AnimationType::SKILL2, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[AnimationType::SKILL2].Add(false, [&]()
		{
			DeactivateRightWeapon();
			SkillTrailComp->SetVisibility(false);
			SkillAuraComp->Deactivate();
			SkillAuraComp->SetVisibility(false);
		});
	NotifyBeginEndEventMap[AnimationType::SKILL2].Add(true, [&]()
		{
			UCombatManager::GetInstance().ActivateCollider();
			SkillCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

			CameraShake(PlayerCameraShake);
			VibrateGamePad(0.4f, 0.4f);


			AObjectPool& objectpool = AObjectPool::GetInstance();
			for (int32 i = 0; i < SkillCount; i++)
			{
				FRotator CurrentRotation = GetActorRotation();
				CurrentRotation.Yaw -= SkillRotateLUT[i];
				FVector Location = GetActorLocation() + CurrentRotation.Vector() * SkillDistance;
				objectpool.SpawnObject(objectpool.ObjectArray[43].ObjClass, Location, FRotator::ZeroRotator);
			}
		});

	PlayerActionTickMap.Add(PlayerAction::NONE, TMap<ActionType, TFunction<void()>>());

	PlayerActionTickMap[PlayerAction::NONE].Add(ActionType::NONE, [&]()
		{
			RecoverStamina();
		});

	PlayerActionTickMap[PlayerAction::NONE].Add(ActionType::DODGE, [&]()
		{
			
		});
	PlayerActionTickMap[PlayerAction::NONE].Add(ActionType::ATTACK, PlayerActionTickMap[PlayerAction::NONE][ActionType::DODGE]);
	PlayerActionTickMap[PlayerAction::NONE].Add(ActionType::PARRING, PlayerActionTickMap[PlayerAction::NONE][ActionType::DODGE]);

	PlayerActionTickMap[PlayerAction::NONE].Add(ActionType::MOVE, [&]()
		{
			RecoverStamina();
			SetPlayerForwardRotAndDir();
			SetPlayerRightRotAndDir();
			LockOnCameraSettingMap[IsLockOn || IsGrab]();

			if (TargetComp != nullptr)
			{
				FVector TargetDir = (TargetComp->GetComponentLocation() - GetActorLocation());
				FVector Cross = FVector::CrossProduct(GetActorRotation().Vector(), TargetDir);

				AnimInstance->HeadBoneRotateValue = FMath::Lerp(AnimInstance->HeadBoneRotateValue, Cross.Z < 0.f ? 0 : 1, fDeltaTime * 2.0f);
				AnimInstance->HeadBoneRotate.Yaw = FMath::Lerp(AnimInstance->HeadBoneRotate.Yaw, AnimInstance->HeadBoneRotateValue * 70.0f - 35.0f, fDeltaTime * 2.0f);
			}

			//LockOnCameraSettingMap[true](character);
			PlayerMovement();

		});

	PlayerActionTickMap[PlayerAction::NONE].Add(ActionType::ROTATE, [&]()
		{
			SetPlayerForwardRotAndDir();
			SetPlayerRightRotAndDir();
		});
	PlayerActionTickMap[PlayerAction::NONE].Add(ActionType::DEAD, [&](){});

	PlayerActionTickMap.Add(PlayerAction::RUN, TMap<ActionType, TFunction<void()>>());
	PlayerActionTickMap[PlayerAction::RUN].Add(ActionType::NONE, PlayerActionTickMap[PlayerAction::NONE][ActionType::NONE]);
	PlayerActionTickMap[PlayerAction::RUN].Add(ActionType::DODGE, PlayerActionTickMap[PlayerAction::NONE][ActionType::DODGE]);
	PlayerActionTickMap[PlayerAction::RUN].Add(ActionType::ATTACK, PlayerActionTickMap[PlayerAction::NONE][ActionType::ATTACK]);
	PlayerActionTickMap[PlayerAction::RUN].Add(ActionType::PARRING, PlayerActionTickMap[PlayerAction::NONE][ActionType::PARRING]);
	PlayerActionTickMap[PlayerAction::RUN].Add(ActionType::MOVE, PlayerActionTickMap[PlayerAction::NONE][ActionType::MOVE]);
	PlayerActionTickMap[PlayerAction::RUN].Add(ActionType::HEAL, PlayerActionTickMap[PlayerAction::NONE][ActionType::MOVE]);
	PlayerActionTickMap[PlayerAction::RUN].Add(ActionType::HIT, PlayerActionTickMap[PlayerAction::NONE][ActionType::ATTACK]);
	PlayerActionTickMap[PlayerAction::RUN].Add(ActionType::DEAD, [&]() {});

	PlayerActionTickMap.Add(PlayerAction::BEFOREATTACK, TMap<ActionType, TFunction<void()>>());
	PlayerActionTickMap[PlayerAction::BEFOREATTACK].Add(ActionType::NONE, PlayerActionTickMap[PlayerAction::NONE][ActionType::NONE]);
	PlayerActionTickMap[PlayerAction::BEFOREATTACK].Add(ActionType::ROTATE, PlayerActionTickMap[PlayerAction::NONE][ActionType::ROTATE]);
	PlayerActionTickMap[PlayerAction::BEFOREATTACK].Add(ActionType::ATTACK, PlayerActionTickMap[PlayerAction::NONE][ActionType::ATTACK]);
	PlayerActionTickMap[PlayerAction::BEFOREATTACK].Add(ActionType::DEAD, [&]() {});

	PlayerActionTickMap.Add(PlayerAction::AFTERATTACK, TMap<ActionType, TFunction<void()>>());
	PlayerActionTickMap[PlayerAction::AFTERATTACK].Add(ActionType::NONE, PlayerActionTickMap[PlayerAction::NONE][ActionType::NONE]);
	PlayerActionTickMap[PlayerAction::AFTERATTACK].Add(ActionType::DODGE, PlayerActionTickMap[PlayerAction::NONE][ActionType::DODGE]);
	PlayerActionTickMap[PlayerAction::AFTERATTACK].Add(ActionType::ATTACK, PlayerActionTickMap[PlayerAction::NONE][ActionType::ATTACK]);
	PlayerActionTickMap[PlayerAction::AFTERATTACK].Add(ActionType::ROTATE, [&](){});
	PlayerActionTickMap[PlayerAction::AFTERATTACK].Add(ActionType::PARRING, PlayerActionTickMap[PlayerAction::NONE][ActionType::PARRING]);
	PlayerActionTickMap[PlayerAction::AFTERATTACK].Add(ActionType::MOVE, PlayerActionTickMap[PlayerAction::NONE][ActionType::MOVE]);
	PlayerActionTickMap[PlayerAction::AFTERATTACK].Add(ActionType::HEAL, PlayerActionTickMap[PlayerAction::NONE][ActionType::MOVE]);
	PlayerActionTickMap[PlayerAction::AFTERATTACK].Add(ActionType::HIT, PlayerActionTickMap[PlayerAction::NONE][ActionType::ATTACK]);
	PlayerActionTickMap[PlayerAction::AFTERATTACK].Add(ActionType::DEAD, [&]() {});

	

	PlayerActionTickMap.Add(PlayerAction::CANTACT, TMap<ActionType, TFunction<void()>>());
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::NONE, [&]()
		{
			const FRotator Rotation = Controller->GetControlRotation();
			YawRotation = FRotator(0, Rotation.Yaw, 0);
			PlayerForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			YawRotation = FRotator(0, Rotation.Yaw, 0);
			SetPlayerForwardRotAndDir();
			SetPlayerRightRotAndDir();
		});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::DODGE, [&]()
		{
		});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::PARRING, [&]() {});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::DEAD, [&]() {});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::INTERACTION, [&]() {});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::MOVE, [&]()
		{
		});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::ROTATE, [&]()
		{
		});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::ATTACK, [&](){});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::POWERATTACK, [&](){});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::HIT, [&](){});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::HEAL, [&](){});
	PlayerActionTickMap[PlayerAction::CANTACT].Add(ActionType::DEAD, [&]() {});

	PlayerActionTickMap.Add(PlayerAction::CANATTACK, TMap<ActionType, TFunction<void()>>());
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::NONE, [&]() {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::DODGE, [&]() {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::PARRING, [&]() {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::DEAD, [&]() {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::INTERACTION, [&]() {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::MOVE, [&]() {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::ROTATE, [&]() {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::ATTACK, [&]() {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::POWERATTACK, [&]() {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::HIT, [&]() {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::HEAL, [&]() {});
	PlayerActionTickMap[PlayerAction::CANATTACK].Add(ActionType::DEAD, [&]() {});

	PlayerActionTickMap.Add(PlayerAction::CANWALK, TMap<ActionType, TFunction<void()>>());
	PlayerActionTickMap[PlayerAction::CANWALK].Add(ActionType::NONE, [&]()
		{
			LockOnCameraSettingMap[IsGrab]();

			if (!IsGrab)return;
			PlayerDataStruct.PlayerStamina = FMath::Clamp(PlayerDataStruct.PlayerStamina -= PlayerDataStruct.DeployShieldStaminaReduce * fDeltaTime, 0.0f, 100.0f);
			PlayerHUD->DecreaseStaminaGradual(this, PlayerDataStruct.PlayerStamina / PlayerDataStruct.MaxStamina);
			if (PlayerDataStruct.PlayerStamina <= 0)
			{
				ChangeMontageAnimation(AnimationType::SHIELDEND);
				IsGrab = false;
				SetSpeed(SpeedMap[IsLockOn || IsGrab][false]);
				AnimInstance->BodyBlendAlpha = 1.0f;
				ShieldOff();
				CheckInputKey();
				ShoulderView(IsShoulderView);
			}
		});
	PlayerActionTickMap[PlayerAction::CANWALK].Add(ActionType::DODGE, [&]() {});
	PlayerActionTickMap[PlayerAction::CANWALK].Add(ActionType::PARRING, [&]() {});
	PlayerActionTickMap[PlayerAction::CANWALK].Add(ActionType::DEAD, [&]() {});
	PlayerActionTickMap[PlayerAction::CANWALK].Add(ActionType::INTERACTION, [&]() {});
	PlayerActionTickMap[PlayerAction::CANWALK].Add(ActionType::MOVE, [&]()
		{			
			LockOnCameraSettingMap[IsGrab]();
			SetPlayerForwardRotAndDir();
			SetPlayerRightRotAndDir();
			PlayerMovement();

			if (!IsGrab)return;
			PlayerDataStruct.PlayerStamina = FMath::Clamp(PlayerDataStruct.PlayerStamina -= PlayerDataStruct.DeployShieldStaminaReduce * fDeltaTime, 0.0f, 100.0f);
			PlayerHUD->DecreaseStaminaGradual(this, PlayerDataStruct.PlayerStamina / PlayerDataStruct.MaxStamina);
			if (PlayerDataStruct.PlayerStamina <= 0)
			{
				ChangeMontageAnimation(AnimationType::SHIELDEND);
				IsGrab = false;
				SetSpeed(SpeedMap[IsLockOn || IsGrab][false]);
				AnimInstance->BodyBlendAlpha = 1.0f;
				ShieldOff();
				CheckInputKey();
				ShoulderView(IsShoulderView);
			}

		});

	PlayerActionTickMap[PlayerAction::CANWALK].Add(ActionType::HIT, [&](){});
	PlayerActionTickMap[PlayerAction::CANWALK].Add(ActionType::HEAL, [&]()
		{
		});
	PlayerActionTickMap[PlayerAction::CANWALK].Add(ActionType::DEAD, [&]() {});

	PlayerActionTickMap.Add(PlayerAction::SPRINT, TMap<ActionType, TFunction<void()>>());
	PlayerActionTickMap[PlayerAction::SPRINT].Add(ActionType::NONE, PlayerActionTickMap[PlayerAction::NONE][ActionType::DODGE]);
	PlayerActionTickMap[PlayerAction::SPRINT].Add(ActionType::DODGE, [&]() {});
	PlayerActionTickMap[PlayerAction::SPRINT].Add(ActionType::PARRING, [&]() {});
	PlayerActionTickMap[PlayerAction::SPRINT].Add(ActionType::DEAD, [&]() {});
	PlayerActionTickMap[PlayerAction::SPRINT].Add(ActionType::INTERACTION, [&]() {});
	PlayerActionTickMap[PlayerAction::SPRINT].Add(ActionType::MOVE, [&]()
		{
			if (TargetComp != nullptr)
			{
				FVector TargetDir = (TargetComp->GetComponentLocation() - GetActorLocation());
				FVector Cross = FVector::CrossProduct(GetActorRotation().Vector(), TargetDir);

				AnimInstance->HeadBoneRotateValue = FMath::Lerp(AnimInstance->HeadBoneRotateValue, Cross.Z < 0.f ? 0 : 1, fDeltaTime * 2.0f);
				AnimInstance->HeadBoneRotate.Yaw = FMath::Lerp(AnimInstance->HeadBoneRotate.Yaw, AnimInstance->HeadBoneRotateValue * 70 - 35.0f, fDeltaTime * 2.0f);
			}

			PlayerDataStruct.PlayerStamina = FMath::Clamp(PlayerDataStruct.PlayerStamina -= PlayerDataStruct.PlayerRunStamina * fDeltaTime, 0.0f, 100.0f);
			PlayerHUD->DecreaseStaminaGradual(this, PlayerDataStruct.PlayerStamina / PlayerDataStruct.MaxStamina);
			if (PlayerDataStruct.PlayerStamina <= 0)
			{
				ChangeMontageAnimation(MovementAnimMap[IsLockOn]());
				SetSpeed(SpeedMap[IsLockOn || IsGrab][false]);
			}

			LockOnCameraSettingMap[false]();
			SetPlayerForwardRotAndDir();
			SetPlayerRightRotAndDir();

			if(IsLockOn)
			YawRotation.Yaw += ForwardRotation[AxisY][AxisX];

			PlayerMovement();
		});

	PlayerActionTickMap[PlayerAction::SPRINT].Add(ActionType::HIT, [&]() {});
	PlayerActionTickMap[PlayerAction::SPRINT].Add(ActionType::HEAL, [&]()
		{
		});
	PlayerActionTickMap[PlayerAction::SPRINT].Add(ActionType::DEAD, [&]() {});
	MontageEndEventMap.Add(AnimationType::SHIELDKNOCKBACK, [&]()
		{
			CheckInputKey();
		});
	MontageEndEventMap.Add(AnimationType::BATTLEDODGE, [&]()
		{	
			SetSpeed(SpeedMap[IsLockOn || IsGrab][false]);
			MontageBlendInTime = 0.1f;
			CheckInputKey();
			Imotal = false;
		});
	MontageEndEventMap.Add(AnimationType::BASICDODGE, [&]()
		{
			SetSpeed(SpeedMap[IsLockOn || IsGrab][false]);
			MontageBlendInTime = 0.1f;
			CheckInputKey();
			Imotal = false;
		});

	MontageEndEventMap.Add(AnimationType::HEAL, [&]()
		{
			if (AnimInstance->BodyBlendAlpha == 1.0f)
			{
				SetSpeed(SpeedMap[IsLockOn || IsGrab][false]);
				ChangeMontageAnimation(AnimationType::ENDOFHEAL);
			}
			else
			{
				AnimInstance->BodyBlendAlpha = 1.0f;
				CheckInputKey();
			}	
		});

	MontageEndEventMap.Add(AnimationType::HIT, [&]()
		{
			ComboAttackEnd();
			CheckInputKey();
		});
	MontageEndEventMap.Add(AnimationType::HITFRONTLEFT, [&]()
		{
			ComboAttackEnd();
			CheckInputKey();
		});
	MontageEndEventMap.Add(AnimationType::HITFRONTRIGHT, [&]()
		{
			ComboAttackEnd();
			CheckInputKey();
		});
	MontageEndEventMap.Add(AnimationType::HITBACKLEFT, [&]()
		{
			ComboAttackEnd();
			CheckInputKey();
		});
	MontageEndEventMap.Add(AnimationType::HITBACKRIGHT, [&]()
		{
			ComboAttackEnd();
			CheckInputKey();
		});
	MontageEndEventMap.Add(AnimationType::SUPERHIT, [&]()
		{
			ComboAttackEnd();
			CheckInputKey();	
			Imotal = false;
		});

	MontageEndEventMap.Add(AnimationType::DEAD, [&]()
		{
			//AnimInstance->PauseAnimation(AnimInstance->GetCurrentActiveMontage());
			ChangeMontageAnimation(AnimationType::DEADLOOP);
			GetWorldTimerManager().SetTimer(DeadTimer, this, &APlayerCharacter::FadeIn, 4.0f);
			IsDead = true;
		});

	MontageEndEventMap.Add(AnimationType::PARRING, [&]()
		{
			UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
			ComboAttackEnd();
			CheckInputKey();
			Imotal = false;
		});

	MontageEndEventMap.Add(AnimationType::ATTACK1, [&]()
		{
			ComboAttackEnd();	
			CheckInputKey();
		});

	MontageEndEventMap.Add(AnimationType::EOSTOEXECUTION, [&]()
		{
			PlayExecutionAnimation();
		});

	MontageEndEventMap.Add(AnimationType::ATTACK2, MontageEndEventMap[AnimationType::ATTACK1]);
	MontageEndEventMap.Add(AnimationType::ATTACK3, MontageEndEventMap[AnimationType::ATTACK1]);
	MontageEndEventMap.Add(AnimationType::ATTACK4, MontageEndEventMap[AnimationType::ATTACK1]);
	MontageEndEventMap.Add(AnimationType::RUNATTACK, MontageEndEventMap[AnimationType::ATTACK1]);

	MontageEndEventMap.Add(AnimationType::POWERATTACK1, MontageEndEventMap[AnimationType::ATTACK1]);
	MontageEndEventMap.Add(AnimationType::POWERATTACK2, MontageEndEventMap[AnimationType::ATTACK1]);
	MontageEndEventMap.Add(AnimationType::POWERATTACK3, [&]()
		{
			MontageEndEventMap[AnimationType::ATTACK1]();
		});

	MontageEndEventMap.Add(AnimationType::DODGEATTACK, MontageEndEventMap[AnimationType::ATTACK1]);

	MontageEndEventMap.Add(AnimationType::ENDOFHEAL, [&]()
		{
			CheckInputKey();
		});

	MontageEndEventMap.Add(AnimationType::SPRINTTURN, [&]()
		{
			if (AxisX == 1 && AxisY == 1)
			{
				SetSpeed(SpeedMap[IsLockOn || IsGrab][false]);
			}

			SetActorRotation(YawRotation);
			CheckInputKey();
		});

	MontageEndEventMap.Add(AnimationType::RUNATTACK, [&]()
		{
			if (AxisX == 1 && AxisY == 1)
			{
				SetSpeed(SpeedMap[IsLockOn || IsGrab][false]);
			}

			MontageEndEventMap[AnimationType::ATTACK1]();
		});
	MontageEndEventMap.Add(AnimationType::RUNPOWERATTACK, [&]()
		{
			if (AxisX == 1 && AxisY == 1)
			{
				SetSpeed(SpeedMap[IsLockOn || IsGrab][false]);
			}
			MontageEndEventMap[AnimationType::ATTACK1]();
		});


	MontageEndEventMap.Add(AnimationType::DOOROPEN, [&]()
		{
			ComboAttackEnd();
			AxisX = 1;
			AxisY = 1;
			CheckInputKey();
			GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, 1.0f);
			WeaponMesh->SetVisibility(true);
			Imotal = false;
		});


	MontageEndEventMap.Add(AnimationType::BACKSTEP, MontageEndEventMap[AnimationType::BATTLEDODGE]);
	MontageEndEventMap.Add(AnimationType::ENDOFRUN, [&]()
		{
			ChangeActionType(ActionType::NONE);
			ChangeMontageAnimation(AnimationType::IDLE);
		});
	MontageEndEventMap.Add(AnimationType::ENDOFSPRINT, [&]()
		{
			if (AxisX == 1 && AxisY == 1)
			{
				SetSpeed(SpeedMap[IsLockOn || IsGrab][false]);
			}
			CheckInputKey();			
		});

	MontageEndEventMap.Add(AnimationType::ACTIVESAVEPOINT, [&]()
		{
			PlayerHUD->PlayInteractionAnimation(true, EInteractions::rest);
			Imotal = false;
			ChangeMontageAnimation(AnimationType::ENDOFHEAL);
			GetWorld()->GetFirstPlayerController()->EnableInput(GetWorld()->GetFirstPlayerController());
		});
	MontageEndEventMap.Add(AnimationType::SAVESTART, [&]()
		{
			GameInstance->PlayerStatUI->AddToViewport();
			PlayerHUD->PlayInteractionAnimation(true, EInteractions::close);
			ChangeMontageAnimation(AnimationType::SAVELOOP);
			RestoreStat();
			GetWorldTimerManager().SetTimer(DeadTimer, this, &APlayerCharacter::FadeOut, 2.0f);
			GameInstance->PlayerStatUI->SetRemainSoul(PlayerDataStruct.SoulCount);

			PlayerHUD->PlayExitAnimation(true);
			SpawnLocation = GetActorLocation();
			UJesusSaveGame::GetInstance().Save(this, GameInstance, SaveMapName);
		});
	MontageEndEventMap.Add(AnimationType::SAVELOOP, [&]()
		{
			ChangeMontageAnimation(AnimationType::SAVELOOP);
		});
	MontageEndEventMap.Add(AnimationType::SAVEEND, [&]()
		{
			GetWorld()->GetFirstPlayerController()->EnableInput(GetWorld()->GetFirstPlayerController());
			PlayerHUD->PlayInteractionAnimation(false);
			SpawnLocation = GetActorLocation();
			SpawnRotation = GetActorRotation();
			CheckInputKey();
			Imotal = false;
		});
	MontageEndEventMap.Add(AnimationType::EXECUTIONBOSS, [&]()
		{
			IsExecute = false;			
			GetWorld()->GetFirstPlayerController()->EnableInput(GetWorld()->GetFirstPlayerController());
			CheckInputKey();
			ExecutionCharacter = nullptr;
			ShoulderView(!IsPhaseTwo);
			Imotal = false;
			if(!IsGrab)
			TargetCameraBoomLength = IsShoulderView ? ShoulderViewCameraLength : BackViewCameraLength;
		});
	MontageEndEventMap.Add(AnimationType::GAMESTARTLOOP, [&]()
		{
			ChangeMontageAnimation(AnimationType::GAMESTARTLOOP);
		});
	MontageEndEventMap.Add(AnimationType::GAMESTART, [&]()
		{
			GameInstance->MainMenuWidget->RemoveFromParent();
			GetWorld()->GetFirstPlayerController()->EnableInput(GetWorld()->GetFirstPlayerController());
			PlayerHUD->AddToViewport();
			PlayerHUD->ChangeHealCount(CurHealCount);
			LocketSKMesh->SetVisibility(false);
			IsCollisionCamera = true;
			CheckInputKey();
		});

	MontageEndEventMap.Add(AnimationType::IDLE, [&]()
		{
			ChangeMontageAnimation(AnimationType::IDLE);
		});
	MontageEndEventMap.Add(AnimationType::FORWARDWALK, [&]()
		{
			ChangeMontageAnimation(AnimationType::FORWARDWALK);
		});
	MontageEndEventMap.Add(AnimationType::FORWARDBATTLEWALK, [&]()
		{
			ChangeMontageAnimation(AnimationType::FORWARDBATTLEWALK);
		});
	MontageEndEventMap.Add(AnimationType::LEFTWALK, [&]()
		{
			ChangeMontageAnimation(AnimationType::LEFTWALK);
		});
	MontageEndEventMap.Add(AnimationType::RIGHTWALK, [&]()
		{
			ChangeMontageAnimation(AnimationType::RIGHTWALK);
		});
	MontageEndEventMap.Add(AnimationType::BACKWALK, [&]()
		{
			ChangeMontageAnimation(AnimationType::BACKWALK);
		});
	MontageEndEventMap.Add(AnimationType::FORWARDLEFTWALK, [&]()
		{
			ChangeMontageAnimation(AnimationType::FORWARDLEFTWALK);
		});
	MontageEndEventMap.Add(AnimationType::FORWARDRIGHTWALK, [&]()
		{
			ChangeMontageAnimation(AnimationType::FORWARDRIGHTWALK);
		});
	MontageEndEventMap.Add(AnimationType::BACKLEFTWALK, [&]()
		{
			ChangeMontageAnimation(AnimationType::BACKLEFTWALK);
		});
	MontageEndEventMap.Add(AnimationType::BACKRIGHTWALK, [&]()
		{
			ChangeMontageAnimation(AnimationType::BACKRIGHTWALK);
		});
	MontageEndEventMap.Add(AnimationType::SHIELDSTART, [&]()
		{
			ChangeMontageAnimation(AnimationType::SHIELDLOOP);
		});
	MontageEndEventMap.Add(AnimationType::SHIELDLOOP, [&]()
		{
			ChangeMontageAnimation(AnimationType::SHIELDLOOP);
		});
	MontageEndEventMap.Add(AnimationType::SHIELDEND, [&]()
		{
			CheckInputKey();
		});
	MontageEndEventMap.Add(AnimationType::SHIELDATTACK, [&]()
		{
			IsCollisionCamera = true;
			ShieldOff();
			ShoulderView(IsShoulderView);
			IsGrab = false;
			CheckInputKey();
		});
	MontageEndEventMap.Add(AnimationType::SHIELDMOVE, [&]()
		{
			ChangeMontageAnimation(AnimationType::SHIELDMOVE);
		});
	MontageEndEventMap.Add(AnimationType::SKILL1, [&]()
		{
			ComboAttackEnd();
			CheckInputKey();
		});
	MontageEndEventMap.Add(AnimationType::SKILL2, [&]()
		{
			ComboAttackEnd();
			CheckInputKey();
		});

	DodgeAnimationMap.Add(false, [&]()->AnimationType
		{
			if (AxisX == 1 && AxisY == 1)
			{
				return AnimationType::BACKSTEP;
			}
			else
			{
				if (IsLockOn)
				{
					return AnimationType::BATTLEDODGE;
				}
			}
			return AnimationType::BASICDODGE;
		});
	DodgeAnimationMap.Add(true, [&]()->AnimationType
		{
			return DodgeDirection[AxisY][AxisX];
		});

	MovementAnimMap.Add(false, [&]()->AnimationType
		{
			return (AxisX == 1 && AxisY == 1) ? AnimationType::ENDOFRUN : AnimationType::FORWARDWALK;
		});
	MovementAnimMap.Add(true, [&]()->AnimationType
		{
			return MovementAnim[AxisY][AxisX];
		});


	LockOnCameraSettingMap.Add(true, [&]()
		{
			if(IsLockOn || IsGrab)
			YawRotation.Yaw = GetController()->GetControlRotation().Yaw;
			GetCharacterMovement()->bOrientRotationToMovement = false;
		});

	LockOnCameraSettingMap.Add(false, [&]()
		{
			GetCharacterMovement()->bOrientRotationToMovement = true;
		});

	InputEventMap.Add(PlayerAction::NONE, TMap<ActionType, TMap<bool, TFunction<void()>>>());
	InputEventMap[PlayerAction::NONE].Add(ActionType::DODGE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::NONE].Add(ActionType::ATTACK, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::NONE].Add(ActionType::POWERATTACK, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::NONE].Add(ActionType::PARRING, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::NONE].Add(ActionType::MOVE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::NONE].Add(ActionType::ROTATE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::NONE].Add(ActionType::HEAL, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::NONE].Add(ActionType::INTERACTION, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::NONE].Add(ActionType::SHIELD, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::NONE].Add(ActionType::SKILL, TMap<bool, TFunction<void()>>());

	InputEventMap[PlayerAction::NONE][ActionType::SKILL].Add(true, [&]()
		{
			if (IsGrab)
				Parring();
			else
				SkillAttack();
		});
	InputEventMap[PlayerAction::NONE][ActionType::SKILL].Add(false, [&]()
		{
		});

	InputEventMap[PlayerAction::NONE][ActionType::DODGE].Add(true, [&]()
		{
			Dodge();
		});
	InputEventMap[PlayerAction::NONE][ActionType::DODGE].Add(false, [&]()
		{

		});

	InputEventMap[PlayerAction::NONE][ActionType::ATTACK].Add(true, [&]()
		{
			BasicAttack();
		});
	InputEventMap[PlayerAction::NONE][ActionType::ATTACK].Add(false, [&]()
		{

		});

	InputEventMap[PlayerAction::NONE][ActionType::POWERATTACK].Add(true, [&]()
		{
			PowerAttack();
		});
	InputEventMap[PlayerAction::NONE][ActionType::POWERATTACK].Add(false, [&]()
		{

		});

	InputEventMap[PlayerAction::NONE][ActionType::PARRING].Add(true, [&]()
		{
			if (IsGrab)
				Parring();
		});
	InputEventMap[PlayerAction::NONE][ActionType::PARRING].Add(false, [&]()
		{

		});

	InputEventMap[PlayerAction::NONE][ActionType::MOVE].Add(true, [&]()
		{

			ChangeActionType(ActionType::MOVE);
			ChangeMontageAnimation(MovementAnimMap[IsLockOn || IsGrab]());
		});
	InputEventMap[PlayerAction::NONE][ActionType::MOVE].Add(false, [&]()
		{
			ChangeMontageAnimation(MovementAnimMap[IsLockOn || IsGrab]());
		});

	InputEventMap[PlayerAction::NONE][ActionType::ROTATE].Add(true, [&]()
		{

		});
	InputEventMap[PlayerAction::NONE][ActionType::ROTATE].Add(false, [&]()
		{

		});

	InputEventMap[PlayerAction::NONE][ActionType::HEAL].Add(true, [&]()
		{
			if (CurHealCount > 0)
			{
				SetSpeed(PlayerDataStruct.PlayerWalkSpeed);
				UseItem();
			}			
		});
	InputEventMap[PlayerAction::NONE][ActionType::HEAL].Add(false, [&]()
		{

		});

	InputEventMap[PlayerAction::NONE][ActionType::INTERACTION].Add(true, [&]()
		{

		});
	InputEventMap[PlayerAction::NONE][ActionType::INTERACTION].Add(false, [&]()
		{

		});

	InputEventMap[PlayerAction::NONE][ActionType::SHIELD].Add(true, [&]()
		{
			if (PlayerDataStruct.SoulCount <= 0 || !CanShieldDeploy)return;
			AxisY != 1 || AxisX != 1 ? ChangeActionType(ActionType::MOVE) : ChangeActionType(ActionType::NONE);

			IsCollisionCamera = false;
			ComboAttackEnd();
			SetSpeed(PlayerDataStruct.PlayerWalkSpeed);
			ChangeMontageAnimation(AnimationType::SHIELDSTART);
			IsGrab = true;
			ShieldOn();
			ShieldOverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			CameraBoom1->CameraLagSpeed = 30.0f;
			SetSpeed(PlayerDataStruct.PlayerWalkSpeed);
			SetCameraTarget(GrabSocketOffset, GrabCameraLength);
		});
	InputEventMap[PlayerAction::NONE][ActionType::SHIELD].Add(false, [&]()
		{
			if (!IsGrab)return;
			IsGrab = false;
			AxisY == 1 && AxisX == 1 ? ChangeMontageAnimation(AnimationType::SHIELDEND)
				: ChangeMontageAnimation(MovementAnimMap[IsLockOn || IsGrab]());			
			SetSpeed(SpeedMap[IsLockOn || IsGrab][false]);
			AnimInstance->BodyBlendAlpha = 1.0f;
			ShieldOff();
			ShoulderView(IsShoulderView);
		});

	InputEventMap.Add(PlayerAction::BEFOREATTACK, TMap<ActionType, TMap<bool, TFunction<void()>>>());
	InputEventMap[PlayerAction::BEFOREATTACK].Add(ActionType::DODGE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::BEFOREATTACK].Add(ActionType::ATTACK, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::BEFOREATTACK].Add(ActionType::POWERATTACK, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::BEFOREATTACK].Add(ActionType::PARRING, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::BEFOREATTACK].Add(ActionType::MOVE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::BEFOREATTACK].Add(ActionType::ROTATE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::BEFOREATTACK].Add(ActionType::HEAL, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::BEFOREATTACK].Add(ActionType::INTERACTION, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::BEFOREATTACK].Add(ActionType::SHIELD, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::BEFOREATTACK].Add(ActionType::SKILL, TMap<bool, TFunction<void()>>());

	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::SKILL].Add(true, [&]()
		{
		});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::SKILL].Add(false, [&]()
		{
		});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::DODGE].Add(true,   []() {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::DODGE].Add(false,  []() {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::ATTACK].Add(true,  []() {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::ATTACK].Add(false, [&]() {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::POWERATTACK].Add(true, [&]() {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::POWERATTACK].Add(false, [&]() {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::PARRING].Add(true, [&]() {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::PARRING].Add(false, [&]() {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::MOVE].Add(true, [&]() {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::MOVE].Add(false, [&]() {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::ROTATE].Add(true, [&]() {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::ROTATE].Add(false, [&]() {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::HEAL].Add(true, [&]() {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::HEAL].Add(false, [&]() {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::INTERACTION].Add(true, [&]() {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::INTERACTION].Add(false, [&]() {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::SHIELD].Add(true, [&]() {});
	InputEventMap[PlayerAction::BEFOREATTACK][ActionType::SHIELD].Add(false, [&]() {});

	InputEventMap.Add(PlayerAction::RUN, TMap<ActionType, TMap<bool, TFunction<void()>>>());
	InputEventMap[PlayerAction::RUN].Add(ActionType::DODGE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::RUN].Add(ActionType::ATTACK, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::RUN].Add(ActionType::POWERATTACK, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::RUN].Add(ActionType::PARRING, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::RUN].Add(ActionType::MOVE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::RUN].Add(ActionType::ROTATE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::RUN].Add(ActionType::HEAL, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::RUN].Add(ActionType::INTERACTION, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::RUN].Add(ActionType::SHIELD, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::RUN].Add(ActionType::SKILL, TMap<bool, TFunction<void()>>());

	InputEventMap[PlayerAction::RUN][ActionType::SKILL].Add(true, [&]()
		{
			SkillAttack();
		});
	InputEventMap[PlayerAction::RUN][ActionType::SKILL].Add(false, [&]()
		{
		});
	InputEventMap[PlayerAction::RUN][ActionType::DODGE].Add(true, [&]()
		{
			Dodge();
		});
	InputEventMap[PlayerAction::RUN][ActionType::DODGE].Add(false, [&]()
		{
		});
	InputEventMap[PlayerAction::RUN][ActionType::ATTACK].Add(true, InputEventMap[PlayerAction::NONE][ActionType::ATTACK][true]);
	InputEventMap[PlayerAction::RUN][ActionType::ATTACK].Add(false, InputEventMap[PlayerAction::NONE][ActionType::ATTACK][false]);
	InputEventMap[PlayerAction::RUN][ActionType::POWERATTACK].Add(true, InputEventMap[PlayerAction::NONE][ActionType::POWERATTACK][true]);
	InputEventMap[PlayerAction::RUN][ActionType::POWERATTACK].Add(false, InputEventMap[PlayerAction::NONE][ActionType::POWERATTACK][false]);
	InputEventMap[PlayerAction::RUN][ActionType::PARRING].Add(true, InputEventMap[PlayerAction::NONE][ActionType::PARRING][true]);
	InputEventMap[PlayerAction::RUN][ActionType::PARRING].Add(false, InputEventMap[PlayerAction::NONE][ActionType::PARRING][false]);
	InputEventMap[PlayerAction::RUN][ActionType::MOVE].Add(true, [&]()
		{

			if (!IsLockOn && AnimInstance->PlayerAnimationType == AnimationType::FORWARDWALK)
				return;
			ChangeActionType(ActionType::MOVE);
			ChangeMontageAnimation(MovementAnimMap[IsLockOn || IsGrab]());
		});
	InputEventMap[PlayerAction::RUN][ActionType::MOVE].Add(false, [&]()
		{
			AnimationType anitype = MovementAnimMap[IsLockOn || IsGrab]();
			if (anitype != AnimationType::FORWARDWALK)
			{
				ChangeMontageAnimation(anitype);
				if(anitype == AnimationType::IDLE)
					ChangeActionType(ActionType::NONE);

			}
		});
	InputEventMap[PlayerAction::RUN][ActionType::ROTATE].Add(true, [&]() {});
	InputEventMap[PlayerAction::RUN][ActionType::ROTATE].Add(false, [&]() {});
	InputEventMap[PlayerAction::RUN][ActionType::HEAL].Add(true, InputEventMap[PlayerAction::NONE][ActionType::HEAL][true]);
	InputEventMap[PlayerAction::RUN][ActionType::HEAL].Add(false, InputEventMap[PlayerAction::NONE][ActionType::HEAL][false]);
	InputEventMap[PlayerAction::RUN][ActionType::INTERACTION].Add(true, InputEventMap[PlayerAction::NONE][ActionType::INTERACTION][true]);
	InputEventMap[PlayerAction::RUN][ActionType::INTERACTION].Add(false, InputEventMap[PlayerAction::NONE][ActionType::INTERACTION][false]);
	InputEventMap[PlayerAction::RUN][ActionType::SHIELD].Add(true, [&]()
		{
			InputEventMap[PlayerAction::NONE][ActionType::SHIELD][true]();
			if(IsGrab)
			AnimInstance->BodyBlendAlpha = 0.0f;

		});
	InputEventMap[PlayerAction::RUN][ActionType::SHIELD].Add(false, [&]()
		{
			InputEventMap[PlayerAction::NONE][ActionType::SHIELD][false]();
		});

	InputEventMap.Add(PlayerAction::AFTERATTACK, TMap<ActionType, TMap<bool, TFunction<void()>>>());
	InputEventMap[PlayerAction::AFTERATTACK].Add(ActionType::DODGE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::AFTERATTACK].Add(ActionType::ATTACK, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::AFTERATTACK].Add(ActionType::POWERATTACK, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::AFTERATTACK].Add(ActionType::PARRING, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::AFTERATTACK].Add(ActionType::MOVE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::AFTERATTACK].Add(ActionType::ROTATE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::AFTERATTACK].Add(ActionType::HEAL, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::AFTERATTACK].Add(ActionType::INTERACTION, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::AFTERATTACK].Add(ActionType::SHIELD, TMap<bool, TFunction<void()>>());

	InputEventMap[PlayerAction::AFTERATTACK][ActionType::DODGE].Add(true, InputEventMap[PlayerAction::RUN][ActionType::DODGE][true]);
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::DODGE].Add(false, [&]() {});
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::ATTACK].Add(true,  InputEventMap[PlayerAction::NONE][ActionType::ATTACK][true]);
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::ATTACK].Add(false, InputEventMap[PlayerAction::NONE][ActionType::ATTACK][false]);
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::POWERATTACK].Add(true, InputEventMap[PlayerAction::NONE][ActionType::POWERATTACK][true]);
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::POWERATTACK].Add(false, InputEventMap[PlayerAction::NONE][ActionType::POWERATTACK][false]);
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::PARRING].Add(true, InputEventMap[PlayerAction::NONE][ActionType::PARRING][true]);
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::PARRING].Add(false, InputEventMap[PlayerAction::NONE][ActionType::PARRING][false]);
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::MOVE].Add(true, [&]() {});
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::MOVE].Add(false, [&]() {});
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::ROTATE].Add(true, [&]() {});
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::ROTATE].Add(false, [&]() {});
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::HEAL].Add(true, [&]() {});
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::HEAL].Add(false, [&]() {});
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::INTERACTION].Add(true, [&]() {});
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::INTERACTION].Add(false, [&]() {});
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::SHIELD].Add(true, InputEventMap[PlayerAction::NONE][ActionType::SHIELD][true]);
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::SHIELD].Add(false, InputEventMap[PlayerAction::NONE][ActionType::SHIELD][false]);
	InputEventMap[PlayerAction::AFTERATTACK].Add(ActionType::SKILL, TMap<bool, TFunction<void()>>());

	InputEventMap[PlayerAction::AFTERATTACK][ActionType::SKILL].Add(true, [&]()
		{
			SkillAttack();
		});
	InputEventMap[PlayerAction::AFTERATTACK][ActionType::SKILL].Add(false, [&]()
		{
		});

	InputEventMap.Add(PlayerAction::CANWALK, TMap<ActionType, TMap<bool, TFunction<void()>>>());
	InputEventMap[PlayerAction::CANWALK].Add(ActionType::DODGE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANWALK].Add(ActionType::ATTACK, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANWALK].Add(ActionType::POWERATTACK, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANWALK].Add(ActionType::PARRING, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANWALK].Add(ActionType::MOVE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANWALK].Add(ActionType::ROTATE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANWALK].Add(ActionType::HEAL, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANWALK].Add(ActionType::INTERACTION, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANWALK].Add(ActionType::SHIELD, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANWALK].Add(ActionType::SKILL, TMap<bool, TFunction<void()>>());

	InputEventMap[PlayerAction::CANWALK][ActionType::SKILL].Add(true, [&]()
		{
		});
	InputEventMap[PlayerAction::CANWALK][ActionType::SKILL].Add(false, [&]()
		{
		});

	InputEventMap[PlayerAction::CANWALK][ActionType::DODGE].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANWALK][ActionType::DODGE].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANWALK][ActionType::ATTACK].Add(true, [&]()
		{
			if (IsGrab)
			ShieldAttack();
		});
	InputEventMap[PlayerAction::CANWALK][ActionType::ATTACK].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANWALK][ActionType::POWERATTACK].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANWALK][ActionType::POWERATTACK].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANWALK][ActionType::PARRING].Add(true, [&]() {

		if (IsGrab)
			Parring();
		});
	InputEventMap[PlayerAction::CANWALK][ActionType::PARRING].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANWALK][ActionType::MOVE].Add(true, [&]()
		{
			ChangeActionType(ActionType::MOVE);
			SetSpeed(PlayerDataStruct.PlayerWalkSpeed);
			AnimInstance->BodyBlendAlpha = 0.0f;
		});
	InputEventMap[PlayerAction::CANWALK][ActionType::MOVE].Add(false, [&]()
		{			
			if(AxisY == 1 && AxisX == 1)
				AnimInstance->BodyBlendAlpha = 1.0f;

		});
	InputEventMap[PlayerAction::CANWALK][ActionType::ROTATE].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANWALK][ActionType::ROTATE].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANWALK][ActionType::HEAL].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANWALK][ActionType::HEAL].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANWALK][ActionType::INTERACTION].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANWALK][ActionType::INTERACTION].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANWALK][ActionType::SHIELD].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANWALK][ActionType::SHIELD].Add(false, InputEventMap[PlayerAction::NONE][ActionType::SHIELD][false]);

	InputEventMap.Add(PlayerAction::CANTACT, TMap<ActionType, TMap<bool, TFunction<void()>>>());
	InputEventMap[PlayerAction::CANTACT].Add(ActionType::DODGE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANTACT].Add(ActionType::ATTACK, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANTACT].Add(ActionType::POWERATTACK, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANTACT].Add(ActionType::PARRING, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANTACT].Add(ActionType::MOVE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANTACT].Add(ActionType::ROTATE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANTACT].Add(ActionType::HEAL, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANTACT].Add(ActionType::INTERACTION, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANTACT].Add(ActionType::SHIELD, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANTACT].Add(ActionType::SKILL, TMap<bool, TFunction<void()>>());

	InputEventMap[PlayerAction::CANTACT][ActionType::SKILL].Add(true, [&]()
		{
		});
	InputEventMap[PlayerAction::CANTACT][ActionType::SKILL].Add(false, [&]()
		{
		});
	InputEventMap[PlayerAction::CANTACT][ActionType::DODGE].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANTACT][ActionType::DODGE].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANTACT][ActionType::ATTACK].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANTACT][ActionType::ATTACK].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANTACT][ActionType::POWERATTACK].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANTACT][ActionType::POWERATTACK].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANTACT][ActionType::PARRING].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANTACT][ActionType::PARRING].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANTACT][ActionType::MOVE].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANTACT][ActionType::MOVE].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANTACT][ActionType::ROTATE].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANTACT][ActionType::ROTATE].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANTACT][ActionType::HEAL].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANTACT][ActionType::HEAL].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANTACT][ActionType::INTERACTION].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANTACT][ActionType::INTERACTION].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANTACT][ActionType::SHIELD].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANTACT][ActionType::SHIELD].Add(false, [&]()
		{

			if (!IsGrab)return;
			IsGrab = false;
			AxisY == 1 && AxisX == 1 ? ChangeMontageAnimation(AnimationType::SHIELDEND)
				: ChangeMontageAnimation(MovementAnimMap[IsLockOn || IsGrab]());
			SetSpeed(SpeedMap[IsLockOn || IsGrab][false]);
			AnimInstance->BodyBlendAlpha = 1.0f;
			ShieldOff();
			ShoulderView(IsShoulderView);
		});

	InputEventMap.Add(PlayerAction::CANATTACK, TMap<ActionType, TMap<bool, TFunction<void()>>>());
	InputEventMap[PlayerAction::CANATTACK].Add(ActionType::DODGE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANATTACK].Add(ActionType::ATTACK, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANATTACK].Add(ActionType::POWERATTACK, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANATTACK].Add(ActionType::PARRING, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANATTACK].Add(ActionType::MOVE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANATTACK].Add(ActionType::ROTATE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANATTACK].Add(ActionType::HEAL, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANATTACK].Add(ActionType::INTERACTION, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANATTACK].Add(ActionType::SHIELD, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::CANATTACK].Add(ActionType::SKILL, TMap<bool, TFunction<void()>>());

	InputEventMap[PlayerAction::CANATTACK][ActionType::SKILL].Add(true, [&]()
		{
		});
	InputEventMap[PlayerAction::CANATTACK][ActionType::SKILL].Add(false, [&]()
		{
		});
	InputEventMap[PlayerAction::CANATTACK][ActionType::DODGE].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::DODGE].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::ATTACK].Add(true, [&]()
		{
			if (UseStamina(PlayerUseStaminaMap[ActionType::ATTACK]))
			{
				PlayerCurAttackIndex++;
				ChangeActionType(ActionType::ATTACK);
				ChangeMontageAnimation(AnimationType::DODGEATTACK);
				CanNextAttack = false;
				AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[24].ObjClass, GetActorLocation(), FRotator::ZeroRotator);
			}
		});
	InputEventMap[PlayerAction::CANATTACK][ActionType::ATTACK].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::POWERATTACK].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::POWERATTACK].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::PARRING].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::PARRING].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::MOVE].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::MOVE].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::ROTATE].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::ROTATE].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::HEAL].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::HEAL].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::INTERACTION].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::INTERACTION].Add(false, [&]() {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::SHIELD].Add(true, [&]() {});
	InputEventMap[PlayerAction::CANATTACK][ActionType::SHIELD].Add(false, [&]() {});
		

	InputEventMap.Add(PlayerAction::SPRINT, TMap<ActionType, TMap<bool, TFunction<void()>>>());
	InputEventMap[PlayerAction::SPRINT].Add(ActionType::DODGE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::SPRINT].Add(ActionType::ATTACK, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::SPRINT].Add(ActionType::POWERATTACK, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::SPRINT].Add(ActionType::PARRING, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::SPRINT].Add(ActionType::MOVE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::SPRINT].Add(ActionType::ROTATE, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::SPRINT].Add(ActionType::HEAL, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::SPRINT].Add(ActionType::INTERACTION, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::SPRINT].Add(ActionType::SHIELD, TMap<bool, TFunction<void()>>());
	InputEventMap[PlayerAction::SPRINT].Add(ActionType::SKILL, TMap<bool, TFunction<void()>>());

	InputEventMap[PlayerAction::SPRINT][ActionType::SKILL].Add(true, [&]()
		{
		});
	InputEventMap[PlayerAction::SPRINT][ActionType::SKILL].Add(false, [&]()
		{
		});
	InputEventMap[PlayerAction::SPRINT][ActionType::DODGE].Add(true, [&]()
		{
			Dodge();
		});
	InputEventMap[PlayerAction::SPRINT][ActionType::DODGE].Add(false, [&]()
		{
			IsSprint = false;
			if (CurActionType == ActionType::MOVE && 
				AnimInstance->PlayerAnimationType != AnimationType::ENDOFSPRINT &&
				AnimInstance->PlayerAnimationType != AnimationType::HEAL &&
				AnimInstance->PlayerAnimationType != AnimationType::SUPERHIT)
			{
				Run();
			}
		});
	InputEventMap[PlayerAction::SPRINT][ActionType::ATTACK].Add(true, [&]()
		{
			if (UseStamina(PlayerUseStaminaMap[ActionType::POWERATTACK]))
			{				
				PlayerCurAttackIndex++;
				ChangeActionType(ActionType::ATTACK);
				ChangeMontageAnimation(AnimationType::RUNPOWERATTACK);
				CanNextAttack = false;
				AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[25].ObjClass, GetActorLocation(), FRotator::ZeroRotator);
			}
		});
	InputEventMap[PlayerAction::SPRINT][ActionType::ATTACK].Add(false, InputEventMap[PlayerAction::NONE][ActionType::ATTACK][false]);
	InputEventMap[PlayerAction::SPRINT][ActionType::POWERATTACK].Add(true, [&]()
		{
			if (UseStamina(PlayerUseStaminaMap[ActionType::POWERATTACK]))
			{
				PlayerCurAttackIndex++;
				ChangeActionType(ActionType::ATTACK);
				ChangeMontageAnimation(AnimationType::RUNPOWERATTACK);
				CanNextAttack = false;
				AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[25].ObjClass, GetActorLocation(), FRotator::ZeroRotator);
			}
		});
	InputEventMap[PlayerAction::SPRINT][ActionType::POWERATTACK].Add(false, InputEventMap[PlayerAction::NONE][ActionType::POWERATTACK][false]);
	InputEventMap[PlayerAction::SPRINT][ActionType::PARRING].Add(true, InputEventMap[PlayerAction::NONE][ActionType::PARRING][true]);
	InputEventMap[PlayerAction::SPRINT][ActionType::PARRING].Add(false, InputEventMap[PlayerAction::NONE][ActionType::PARRING][false]);
	InputEventMap[PlayerAction::SPRINT][ActionType::MOVE].Add(true, [&]()
		{
		});
	InputEventMap[PlayerAction::SPRINT][ActionType::MOVE].Add(false, [&]()
		{
			if (AxisX == 1 && AxisY == 1)
			{
				ChangeMontageAnimation(AnimationType::ENDOFSPRINT);
			}
		});
	InputEventMap[PlayerAction::SPRINT][ActionType::ROTATE].Add(true, [&]() {});
	InputEventMap[PlayerAction::SPRINT][ActionType::ROTATE].Add(false, [&]() {});
	InputEventMap[PlayerAction::SPRINT][ActionType::HEAL].Add(true, InputEventMap[PlayerAction::NONE][ActionType::HEAL][true]);
	InputEventMap[PlayerAction::SPRINT][ActionType::HEAL].Add(false,InputEventMap[PlayerAction::NONE][ActionType::HEAL][false]);
	InputEventMap[PlayerAction::SPRINT][ActionType::INTERACTION].Add(true,  InputEventMap[PlayerAction::NONE][ActionType::INTERACTION][true]);
	InputEventMap[PlayerAction::SPRINT][ActionType::INTERACTION].Add(false, InputEventMap[PlayerAction::NONE][ActionType::INTERACTION][false]);
	InputEventMap[PlayerAction::SPRINT][ActionType::SHIELD].Add(true, [&]()
		{
			if (IsSprint && AnimInstance->PlayerAnimationType == AnimationType::SPRINT)
			{
				InputEventMap[PlayerAction::SPRINT][ActionType::DODGE][false]();
			}
			InputEventMap[PlayerAction::NONE][ActionType::SHIELD][true]();

			if (IsGrab)
				AnimInstance->BodyBlendAlpha = 0.0f;
		});
	InputEventMap[PlayerAction::SPRINT][ActionType::SHIELD].Add(false, InputEventMap[PlayerAction::NONE][ActionType::SHIELD][false]);

	PlayerEventFuncMap.Add(AnimationType::SUPERHIT, TMap<bool, TFunction<void()>>());
	PlayerEventFuncMap[AnimationType::SUPERHIT].Add(true, [&]()
		{
			if (PlayerDataStruct.CharacterHp <= 0)
			{
				GetWorldTimerManager().SetTimer(DeadTimer, this, &APlayerCharacter::FadeIn, 2.0f);
				PlayerDead(true);
			}
		});
	PlayerEventFuncMap[AnimationType::SUPERHIT].Add(false, [&]()
		{
		});

	PlayerEventFuncMap.Add(AnimationType::EXECUTIONBOSS, TMap<bool, TFunction<void()>>());
	PlayerEventFuncMap[AnimationType::EXECUTIONBOSS].Add(true, [&]()
		{
			ComboAttackEnd();
			ExecutionCharacter->TakeDamage(PlayerDataStruct.BaseDamage * PlayerDataStruct.PlayerExecutionFirstDamage, CharacterDamageEvent, nullptr, this);
			VibrateGamePad(0.4f, 0.4f);
			AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[31].ObjClass, ExecutionCharacter->GetActorLocation() + FVector(0, 0, 20.0f), FRotator::ZeroRotator);
		});
	PlayerEventFuncMap[AnimationType::EXECUTIONBOSS].Add(false, [&]()
		{
			UCombatManager::GetInstance().HitMonsterInfoArray.AddUnique(ExecutionCharacter);
			CameraShake(PlayerCameraShake);
			ExecutionCharacter->TakeDamage(PlayerDataStruct.BaseDamage * PlayerDataStruct.PlayerExecutionSecondDamage, CharacterDamageEvent, nullptr, this);
			VibrateGamePad(0.4f, 0.4f);
			AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[31].ObjClass, ExecutionCharacter->GetActorLocation() + FVector(0, 0, 20.0f), FRotator::ZeroRotator);
		});

	PlayerEventFuncMap.Add(AnimationType::EOSTOEXECUTION, TMap<bool, TFunction<void()>>());
	PlayerEventFuncMap[AnimationType::EOSTOEXECUTION].Add(true, [&]()
		{
			UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
		});
	PlayerEventFuncMap[AnimationType::EOSTOEXECUTION].Add(false , [&]()
		{

		});

	PlayerEventFuncMap.Add(AnimationType::SKILL1, TMap<bool, TFunction<void()>>());
	PlayerEventFuncMap[AnimationType::SKILL1].Add(true, [&]()
		{
			SkillTrailComp->SetVisibility(true);
			SkillAuraComp->SetVisibility(true);
			SkillAuraComp->Activate();
		});
	PlayerEventFuncMap[AnimationType::SKILL1].Add(false, [&]()
		{

		});

	PlayerEventFuncMap.Add(AnimationType::SKILL2, TMap<bool, TFunction<void()>>());
	PlayerEventFuncMap[AnimationType::SKILL2].Add(true, [&]()
		{
			SkillTrailComp->SetVisibility(true);
			SkillAuraComp->SetVisibility(true);
			SkillAuraComp->Activate();

		});
	PlayerEventFuncMap[AnimationType::SKILL2].Add(false, [&]()
		{

		});
}

APlayerCharacter::~APlayerCharacter()
{
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerSKMesh = GetMesh();
	IsPhaseTwo = false;
	DebugMode = false;
	ShieldEffectComp->SetVisibility(false);
	LocketSKMesh->SetVisibility(true);
	ChangeActionType(ActionType::DEAD);

	TArray<UActorComponent*> asd = GetComponentsByTag(UActorSequenceComponent::StaticClass(), FName("BossExecution"));
	
	BossExecutionSequence = Cast<UActorSequenceComponent>(asd[0]);
	BossExecutionSequncePlayer = BossExecutionSequence->GetSequencePlayer();
	
	asd = GetComponentsByTag(UActorSequenceComponent::StaticClass(), FName("Start"));
	GameStartSequence = Cast<UActorSequenceComponent>(asd[0]);
	GameStartSequncePlayer = GameStartSequence->GetSequencePlayer();
	
	asd = GetComponentsByTag(UActorSequenceComponent::StaticClass(), FName("Parrying"));
	BossParryingSequncePlayer = Cast<UActorSequenceComponent>(asd[0])->GetSequencePlayer();

	asd = GetComponentsByTag(UBoxComponent::StaticClass(), FName("Shield"));
	ShieldOverlapComp = Cast<UBoxComponent>(asd[0]);

	if (IsValid(PlayerUIClass))
	{
		PlayerHUD = Cast<UPlayerHUD>(CreateWidget(GetWorld(), PlayerUIClass));		
	}

	if (!PlayerMaxAttackIndex.Contains(ActionType::ATTACK))
	{
		PlayerMaxAttackIndex.Add(ActionType::ATTACK, 4);
		PlayerMaxAttackIndex.Add(ActionType::POWERATTACK, 3);
		PlayerMaxAttackIndex.Add(ActionType::SKILL, 2);
	}


	if (IsValid(PlayerUIClass))
	{
		UserSettingUI = Cast<UUserSettingUI>(CreateWidget(GetWorld(), UserSettingUIClass));
		//UserSettingUI->AddToViewport();
	}

	CurHealCount = PlayerDataStruct.MaxHealCount;

	ShieldOff();
	TargetOpacity = 1.0f;
	IsCollisionCamera = false;

	SpeedMap.Add(false, TMap<bool, float>());
	SpeedMap[false].Add(false, PlayerDataStruct.CharacterOriginSpeed);
	SpeedMap[false].Add(true,  PlayerDataStruct.PlayerRunSpeed);

	SpeedMap.Add(true, TMap<bool, float>());
	SpeedMap[true].Add(false, PlayerDataStruct.PlayerLockOnMoveSpeed);
	SpeedMap[true].Add(true, PlayerDataStruct.PlayerRunSpeed);

	GameInstance->InitInstance();
	GameInstance->InitDefaultSetting();
	GameInstance->MainMenuWidget->StartButton->OnClicked.AddDynamic(this, &APlayerCharacter::NewGameButton);
	GameInstance->MainMenuWidget->ContinueButton->OnClicked.AddDynamic(this, &APlayerCharacter::PlayStartAnimation);
	GetWorld()->GetFirstPlayerController()->DisableInput(GetWorld()->GetFirstPlayerController());

	GameInstance->MainMenuWidget->AddToViewport();

	TargetSocketOffset = CameraBoom1->SocketOffset;

	Imotal = false;
	TargetCompArray.Empty();
	TargetComp = nullptr;

	RotSpeed = 5.0f;

	GetCharacterMovement()->MaxWalkSpeed = PlayerDataStruct.CharacterOriginSpeed;
	
	bUseControllerRotationYaw = false;

	GameInstance->DebugLogWidget->T_PlayerHP->SetText(FText::AsNumber(PlayerDataStruct.CharacterHp));
	AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstance->PlayerAnimationType = AnimationType::IDLE;
	ChangeMontageAnimation(AnimationType::GAMESTARTLOOP);

	LockOnCameraSettingMap[true]();

	FollowCamera = Cast<UCameraComponent>(GetComponentByClass(UCameraComponent::StaticClass()));

	ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SwordTrailComp->Deactivate();
	SkillCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DeactivateSMOverlap();
	DeactivateRightWeapon();

	AnimInstance->BodyBlendAlpha = 1.0f;

	GameStartSequncePlayer->Play();
	GameStartSequncePlayer->Pause();

	SkillAuraComp->Deactivate();
	SkillTrailComp->SetVisibility(false);

	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnWeaponOverlapBegin);
	SkillCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnWeaponOverlapBegin);
	ExecutionTrigger->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnExecutionOverlapBegin);
	ExecutionTrigger->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnExecutionOverlapEnd);
	ParryingCollision1->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnParryingOverlapBegin);
	WeaponOverlapStaticMeshCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnSMOverlapBegin);
	WeaponOverlapStaticMeshCollision->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnSMOverlapEnd);
	TargetDetectionCollison->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnEnemyDetectionBeginOverlap);
	TargetDetectionCollison->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnEnemyDetectionEndOverlap);
	ShieldAttackOverlap->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnShieldOverlapBegin);
	
	ShoulderView(IsShoulderView);

	TArray<AActor*> ActorsToFind;
	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseTriggerActor::StaticClass(), ActorsToFind);
	}
	for (AActor* TriggerActor : ActorsToFind)
	{
		ABaseTriggerActor* TriggerActorCast = Cast<ABaseTriggerActor>(TriggerActor);
		if (TriggerActorCast)
		{
			if(TriggerActorCast->Index >= 0)
			GameInstance->SavedTriggerActor.Add(TriggerActorCast->Index, TriggerActorCast);
		}
	}
	PlayerDataStruct.SoulCount = 0;

	SaveMapName = "Garden";
	PlayerOriginDataStruct = PlayerDataStruct;
	OriginLocation = GetActorLocation();
	OriginRotation = GetActorRotation();

	GetWorldTimerManager().SetTimer(DeadTimer, this, &APlayerCharacter::LoadFile, 0.2f);
	GetWorldTimerManager().SetTimer(SprintEndTimer, this, &APlayerCharacter::LoadMap, 0.5f);

	ASoundManager::GetInstance().Init();
	CanShieldDeploy = true;
	CanUseSkill = true;

	SetSoul(PlayerDataStruct.SoulCount);
}



void APlayerCharacter::PlayStartAnimation()
{
	GameStartSequncePlayer->Play();

	MontageBlendInTime = 0.0f;
	ChangeMontageAnimation(AnimationType::GAMESTART);
	AJesusPlayerController* controller = Cast<AJesusPlayerController>(GetWorld()->GetFirstPlayerController());
	controller->DisableInput(controller);
	controller->SetInputMode(FInputModeGameOnly());
	controller->bShowMouseCursor = false;
	LocketSKMesh->GetAnimInstance()->Montage_Play(MontageMap[AnimationType::NONE]);
}


void APlayerCharacter::NewGameButton()
{
	UJesusSaveGame::GetInstance().Delete();
	GetWorldTimerManager().SetTimer(DeadTimer, this, &APlayerCharacter::ResetGame, 1.5f);
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UCombatManager::GetInstance().MonsterInfoArray.Empty();
	UCombatManager::GetInstance().HitMonsterInfoArray.Empty();
	UCombatManager::GetInstance().MonsterInfoMap.Empty();
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
		const FRotator Rotation = Controller->GetControlRotation();
		YawRotation = FRotator(0, Rotation.Yaw, 0);
		PlayerForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		if (!IsLockOn && !IsGrab)
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
		const FRotator Rotation = Controller->GetControlRotation();
		YawRotation = FRotator(0, Rotation.Yaw, 0);
		PlayerRightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		if (!IsLockOn && !IsGrab)
		{
			YawRotation = FRotator(0, Rotation.Yaw + ForwardRotation[AxisY][AxisX], 0);
		}
		else
		{
			YawRotation = FRotator(0, Rotation.Yaw, 0);
		}
	}
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
			MontageEndEventMap[AnimInstance->PlayerAnimationType]();

		DeactivateRightWeapon();
		DeactivateSMOverlap();
		ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ComboAttackEnd();
		SwordTrailComp->Deactivate();
		CanNextAttack = false;
		LockOnCameraSettingMap[false]();
		UCombatManager::GetInstance().ActivateCollider();
		SetPlayerForwardRotAndDir();
		SetPlayerRightRotAndDir();

		if (IsLockOn || IsGrab)
		{
			YawRotation += FRotator(0, ForwardRotation[AxisY][AxisX], 0);
		}
		CameraBoom1->CameraLagSpeed = 10.0f;

		ChangeActionType(ActionType::DODGE);
		
			
		ChangeMontageAnimation(DodgeAnimationMap[false]());
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
	UCombatManager& combatmanager = UCombatManager::GetInstance();

	for (int32 i = 0; i < combatmanager.MonsterInfoArray.Num(); i++)
	{
		combatmanager.MonsterInfoArray[i]->SetActive(false);
	}

	for (int32 i = 0; i < combatmanager.MonsterInfoMap[SaveMapName.ToString()].Num(); i++)
	{
		if(SaveMapName != "A_KimMinYeongMap_Boss1" && !combatmanager.MonsterInfoMap[SaveMapName.ToString()][i]->IsDie)
		combatmanager.MonsterInfoMap[SaveMapName.ToString()][i]->RespawnCharacter();
	}
}

void APlayerCharacter::MoveSpawnLocation(FVector Location)
{
	UCombatManager::GetInstance().Boss2->SetActive(true);
	IsPhaseTwo = true;
	SetActorRotation(FRotator(0, 0, 0));
	YawRotation = FRotator(0, 0, 0);
	SetActorLocation(Location);
	//SpawnLocation = Location;
	GetWorld()->GetFirstPlayerController()->EnableInput(GetWorld()->GetFirstPlayerController());
	//UJesusSaveGame::GetInstance().Save(this, GameInstance);

	if (IsLockOn)
		LockOn();
}

bool APlayerCharacter::IsAlive()
{
	if(PlayerDataStruct.CharacterHp > 0)
	return true;
	return false;
}

void APlayerCharacter::LockOn()
{
	IsLockOn = !IsLockOn;

	if (IsLockOn)
	{
		if (TargetComp == nullptr)
		{
			GetCompsInScreen(TargetCompArray);
			GetFirstTarget();
			if (TargetComp == nullptr)
			{
				IsLockOn = false;
				return;
			}
		}

		Cast<ABaseCharacter>(TargetComp->GetOwner())->ActivateLockOnImage(true, TargetComp);

		if (AnimInstance->PlayerAnimationType != AnimationType::HEAL && !IsGrab)
		{
			SetSpeed(SpeedMap[IsLockOn || IsGrab][IsSprint]);
		}


		ShoulderView(!IsPhaseTwo);
		CurRotateIndex = 1;
	}
	else
	{
		ShoulderView(true);
		if(TargetComp != nullptr)
		Cast<ABaseCharacter>(TargetComp->GetOwner())->ActivateLockOnImage(false, TargetComp);
		TargetComp = nullptr;
		if (AnimInstance->PlayerAnimationType != AnimationType::HEAL && !IsGrab)
			SetSpeed(SpeedMap[IsLockOn || IsGrab][IsSprint]);
		CurRotateIndex = 0;
	}

	if (CurActionType == ActionType::MOVE && AnimInstance->PlayerAnimationType != AnimationType::ENDOFSPRINT && 
		AnimInstance->PlayerAnimationType != AnimationType::HEAL && !IsGrab && PlayerCurAction != PlayerAction::CANTACT)
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
			TargetComp = TargetCompInScreenArray[i];
		}
	}
}

void APlayerCharacter::GetCompsInScreen(TArray<UPrimitiveComponent*>Array)
{
	FVector2D ScreenLocation;
	FVector CompLocation;
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
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
	if (AnimInstance->PlayerAnimationType == AnimationType::EXECUTIONBOSS
		|| AnimInstance->PlayerAnimationType == AnimationType::EOSTOEXECUTION ||
		AnimInstance->PlayerAnimationType == AnimationType::SHIELDATTACK)return;
	ChangeTargetTime = 0.0f;
	GetCompsInScreen(TargetCompArray);
	FVector CameraLocation = CameraBoom1->GetComponentLocation();
	FVector TargetVector = FVector(0, 0, 0);
	if (TargetComp != nullptr)
		TargetVector = TargetComp->GetComponentLocation();

	FVector TargetDir = (TargetVector - CameraLocation).GetSafeNormal();
	TArray<UPrimitiveComponent*> TargetArray;

	for (int32 i = 0; i < TargetCompInScreenArray.Num(); i++)
	{
		if (!TargetCompInScreenArray[i]->GetOwner()->IsActorTickEnabled())continue;
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
		float Distance = FVector::DistSquared(CameraLocation, TargetArray[i]->GetComponentLocation());
		float Distance2 = FVector::DistSquared(CameraLocation, TargetArray[TargetIdx]->GetComponentLocation());
		float TargetDot = FVector::DotProduct(TargetDir, (TargetArray[TargetIdx]->GetComponentLocation() - CameraLocation).GetSafeNormal());
		float NextDot = FVector::DotProduct(TargetDir, (TargetArray[i]->GetComponentLocation() - CameraLocation).GetSafeNormal());

		if (NextDot > TargetDot && Distance > Distance2)
			TargetIdx = i;
	}

	Cast<ABaseCharacter>(TargetComp->GetOwner())->ActivateLockOnImage(false, TargetComp);
	TargetComp = TargetArray[TargetIdx];
	Cast<ABaseCharacter>(TargetComp->GetOwner())->ActivateLockOnImage(true, TargetComp);

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
	IsInputPad = IsPad;
}

void APlayerCharacter::Sprint()
{
	SetSpeed(SpeedMap[IsLockOn || IsGrab][true]);
	ChangeMontageAnimation(AnimationType::SPRINT);
}

void APlayerCharacter::Run()
{
	SetSpeed(SpeedMap[IsLockOn || IsGrab][false]);
	InputEventMap[PlayerAction::NONE][ActionType::MOVE][true]();
}

void APlayerCharacter::SetShieldHP(float HP, FVector Location)
{
	SetSoul(HP * PlayerDataStruct.ShieldDecreaseSoulPercent);

	float Distance = HP <= 60 ? AttackDefDistance : PowerAttackDefDistance;
	
	LaunchCharacter(-GetActorRotation().Vector() * Distance, false, false);

	ChangeMontageAnimation(AnimationType::SHIELDKNOCKBACK);

	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[38].ObjClass, ShieldMeshComp->GetComponentLocation(), FRotator(0, 0, 0));
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[40].ObjClass, ShieldMeshComp->GetComponentLocation(), FRotator(0, 0, 0));

	if (PlayerDataStruct.SoulCount <= 0)
	{
		PlayerDataStruct.SoulCount = 0;
		PlayerHUD->ClearShield();
		IsGrab = false;
		VibrateGamePad(0.4f, 0.4f);
		CameraShake(PlayerCameraShake);
		ShieldOff();
		ShoulderView(IsShoulderView);
		AnimInstance->BodyBlendAlpha = 1.0f;
		ChangeActionType(ActionType::HIT);
		ChangeMontageAnimation(AnimationType::HITBACKRIGHT);
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[39].ObjClass, ShieldMeshComp->GetComponentLocation(), GetActorRotation() + FRotator(0, 90, 0));
	}
}

void APlayerCharacter::RecoverStamina()
{
	PlayerDataStruct.PlayerStamina = FMath::Clamp(PlayerDataStruct.PlayerStamina += fDeltaTime * PlayerDataStruct.StaminaRecovery, 0.0f, 100.0f);
	PlayerHUD->SetStamina(PlayerDataStruct.PlayerStamina / PlayerDataStruct.MaxStamina);
	GameInstance->DebugLogWidget->T_PlayerStamina->SetText(FText::AsNumber(PlayerDataStruct.PlayerStamina));
}

void APlayerCharacter::RecoverShield()
{
	CanShieldDeploy = true;
}

void APlayerCharacter::RecoverSkill()
{
	CanUseSkill = true;
}

bool APlayerCharacter::UseStamina(float value)
{
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
	if (IsGrab)
	{
		ChangeMontageAnimation(AnimationType::SHIELDLOOP);
		return;
	}
	else
	{

		IsGrab = false;
		AxisY == 1 && AxisX == 1 ? ChangeMontageAnimation(AnimationType::SHIELDEND)
			: ChangeMontageAnimation(MovementAnimMap[IsLockOn || IsGrab]());
		SetSpeed(SpeedMap[IsLockOn || IsGrab][false]);
		AnimInstance->BodyBlendAlpha = 1.0f;
		ShieldOff();
		ShoulderView(IsShoulderView);
	}
	if (AxisX != 1  || AxisY != 1)
	{
		if(AxisY == 2 && !IsGrab)
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
		SetSpeed(SpeedMap[IsLockOn || IsGrab][false]);
		ChangeActionType(ActionType::NONE);
		ChangeMontageAnimation(AnimationType::IDLE);
	}	
}

bool APlayerCharacter::CanActivate(int32 SoulCount)
{
	if (SoulCount <= PlayerDataStruct.SoulCount)
	{
		SetSoul(-SoulCount);
		return true;
	}
	return false;
}

void APlayerCharacter::SetSpeed(float speed)
{
	if (speed == PlayerDataStruct.PlayerWalkSpeed && !IsGrab)
	{
		RotSpeed = 1.0f;
	}
	else
	{
		RotSpeed = 5.0f;
	}
	if (!IsGrab)
	{
		CameraBoom1->CameraLagSpeed = 6.0f;
	}
	if(IsSprint)
		CameraBoom1->CameraLagSpeed = 3.0f;

	GetCharacterMovement()->MaxWalkSpeed = speed;
}

void APlayerCharacter::ShieldOff()
{
	IsCollisionCamera = true;
	ShieldEffectComp->SetVisibility(false);
	ShieldMeshComp->SetVisibility(false);
	ShieldAttackOverlap->SetRelativeLocation(FVector(10000, 10000, 10000));
	ShieldOverlapComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void APlayerCharacter::ShieldOn()
{
	IsCollisionCamera = false;
	ShieldEffectComp->SetVisibility(true);
	ShieldMeshComp->SetVisibility(true);
	ShieldOverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
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
		AObjectPool& objectpool = AObjectPool::GetInstance();
		if (PlayerAttackType == ActionType::ATTACK)
			objectpool.SpawnObject(objectpool.ObjectArray[24].ObjClass, GetActorLocation(), FRotator::ZeroRotator);
		else
			objectpool.SpawnObject(objectpool.ObjectArray[25].ObjClass, GetActorLocation(), FRotator::ZeroRotator);

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
			NotifyBeginEndEventMap[AnimInstance->PlayerAnimationType][value]();
		}
	}
}

void APlayerCharacter::CheckMontageEndNotify()
{
	if (MontageEndEventMap.Contains(AnimInstance->PlayerAnimationType))
	{
		MontageEndEventMap[AnimInstance->PlayerAnimationType]();
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

	FRotator rotation = FRotationMatrix::MakeFromX(TargetComp->GetComponentLocation() - HeadBoneLocation->GetComponentLocation()).Rotator();



	if (AnimInstance->PlayerAnimationType != AnimationType::BATTLEDODGE
		&& AnimInstance->PlayerAnimationType != AnimationType::SPRINT 
		&& AnimInstance->PlayerAnimationType != AnimationType::DEADLOOP
		&& AnimInstance->PlayerAnimationType != AnimationType::DEADLOOP2)
		YawRotation.Yaw = GetController()->GetControlRotation().Yaw;
}

void APlayerCharacter::ResetGame()
{
	PlayStartAnimation();
	SaveMapName = "Garden";
	PlayerDataStruct = PlayerOriginDataStruct;
	SpawnLocation = OriginLocation;
	SetActorLocation(OriginLocation);
	SetActorRotation(OriginRotation);

	for (int i = 0; i < GameInstance->SavedTriggerActor.Num(); i++)
	{
		GameInstance->SavedTriggerActor[i]->IsActive = false;
		GameInstance->SavedTriggerActor[i]->Init();
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	fDeltaTime = DeltaTime;
	ChangeTargetTime += fDeltaTime;

	AnimInstance->HeadBoneRotate.Yaw = FMath::Lerp(AnimInstance->HeadBoneRotate.Yaw, 0.0f, fDeltaTime * 2.0f);
	PlayerActionTickMap[PlayerCurAction][CurActionType]();

	LookTarget();

	RotatePlayer();

	SetActorRotation(FRotator(0, GetActorRotation().Yaw, 0));

	FollowCamera->SetWorldRotation(FRotator(FollowCamera->GetComponentRotation().Pitch, FollowCamera->GetComponentRotation().Yaw, 0));
	CameraBoom1->TargetArmLength = FMath::Lerp(CameraBoom1->TargetArmLength, TargetCameraBoomLength, DeltaTime * 2.0f);
	CameraBoom1->SocketOffset = FMath::Lerp(CameraBoom1->SocketOffset, TargetSocketOffset, DeltaTime * 2.0f);

	CameraDistanceToPlayer = FVector::Distance(FollowCamera->GetComponentLocation(), GetActorLocation());
	CameraDistanceToPlayer = FMath::Clamp(CameraDistanceToPlayer, 40, 300);
	PlayerSKMesh->SetScalarParameterValueOnMaterials("Dither", GetPercent(CameraDistanceToPlayer, 40, 100));
	WeaponMesh->SetScalarParameterValueOnMaterials("Dither", GetPercent(CameraDistanceToPlayer, 40, 100));
}

float APlayerCharacter::GetPercent(float value, float min, float max)
{
	return IsCollisionCamera ? ((value - min) / (max - min)) : 1.0f;
}


void APlayerCharacter::RespawnCharacter()
{
	Super::RespawnCharacter();
	GetWorldTimerManager().SetTimer(SprintEndTimer, this, &APlayerCharacter::FadeOut, 4.0f);

	ASoundManager::GetInstance().StartBGMSound(IsPhaseTwo);

	GetWorld()->GetFirstPlayerController()->EnableInput(GetWorld()->GetFirstPlayerController());

	AnimInstance->BodyBlendAlpha = 1.0f;

	Imotal = false;
	TargetCompArray.Empty();

	CameraBoom1->SetWorldRotation(FRotator::ZeroRotator);
	YawRotation = SpawnRotation;
	ChangeActionType(ActionType::NONE);
	ChangeMontageAnimation(AnimationType::IDLE);

	AxisX = 1;
	AxisY = 1;

	RestoreStat();

	GetWorldTimerManager().SetTimer(SprintEndTimer, this, &APlayerCharacter::LoadMap, 0.5f);

	SetSpeed(SpeedMap[false][false]);
}

void APlayerCharacter::EventNotify(bool value)
{
	if (PlayerEventFuncMap.Contains(AnimInstance->PlayerAnimationType))
	{
		PlayerEventFuncMap[AnimInstance->PlayerAnimationType][value]();
	}
}

void APlayerCharacter::PlayExecutionAnimation()
{	
	IsExecute = true;
	//ExecuteDirection = ExecutionGetActorLocation() - GetActorLocation();
	//ExecuteDirection.Normalize();
	//ExecuteLocation = GetActorLocation() - ExecuteDirection * 50.0f;
	//SetActorLocation(ExecuteLocation);

	if (!IsLockOn)
	{
		LockOn();
	}

	if (TargetComp != nullptr)
	{
		if (TargetComp->GetOwner() != ExecutionCharacter)
		{
			Cast<ABaseCharacter>(TargetComp->GetOwner())->ActivateLockOnImage(false, TargetComp);
			TargetComp = ExecutionCharacter->LockOnComp;
			Cast<ABaseCharacter>(TargetComp->GetOwner())->ActivateLockOnImage(true, TargetComp);
		}
	}
	ShoulderView(true);
	ExecutionCharacter->PlayExecutionAnimation();
	DeactivateRightWeapon();
	DeactivateSMOverlap();
	SwordTrailComp->Deactivate();
	ComboAttackEnd();
	ChangeMontageAnimation(AnimationType::EXECUTIONBOSS);
	BossExecutionSequncePlayer->Play();
	AxisX = 1;
	AxisY = 1;

	Imotal = true;
}

void APlayerCharacter::Parring()
{
	if (UseStamina(PlayerUseStaminaMap[ActionType::PARRING]))
	{
		AObjectPool& objectpool = AObjectPool::GetInstance();
		objectpool.SpawnObject(objectpool.ObjectArray[24].ObjClass, GetActorLocation(), FRotator::ZeroRotator);
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
	TargetCompArray.Remove(OtherComp);
}

void APlayerCharacter::PlayerMovement()
{
	DiagonalSpeed = PlayerDataStruct.PlayerRunSpeed * sqrt(0.5f) + PlayerDataStruct.PlayerRunSpeed * sqrt(0.5f);
		
	AddMovementInput(PlayerForwardDirection, ForwardMovementValue * DiagonalSpeed * fDeltaTime);
	AddMovementInput(PlayerRightDirection, RightMovementValue * DiagonalSpeed * fDeltaTime);
}

void APlayerCharacter::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	

	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);

	if (character != nullptr)
	{
		AObjectPool& objectpool = AObjectPool::GetInstance();
		UCombatManager::GetInstance().HitMonsterInfoArray.AddUnique(character);

		HitStop();
		CameraShake(PlayerCameraShake);
		objectpool.SpawnObject(objectpool.ObjectArray[17].ObjClass, GetActorLocation(), FRotator::ZeroRotator);

		if (HitEffectRotatorList.Contains(AnimInstance->PlayerAnimationType))
		{
			objectpool.SpawnObject(objectpool.ObjectArray[0].ObjClass, OverlappedComponent->GetComponentLocation(), YawRotation - HitEffectRotatorList[AnimInstance->PlayerAnimationType]);
			objectpool.SpawnObject(objectpool.ObjectArray[1].ObjClass, OverlappedComponent->GetComponentLocation(), YawRotation - HitEffectRotatorList[AnimInstance->PlayerAnimationType]);
			objectpool.SpawnObject(objectpool.ObjectArray[3].ObjClass, OverlappedComponent->GetComponentLocation(), YawRotation - HitEffectRotatorList[AnimInstance->PlayerAnimationType]);
			objectpool.SpawnObject(objectpool.ObjectArray[3].ObjClass, OverlappedComponent->GetComponentLocation(), YawRotation - HitEffectRotatorList[AnimInstance->PlayerAnimationType]);
			objectpool.SpawnObject(objectpool.ObjectArray[1].ObjClass, OverlappedComponent->GetComponentLocation(), YawRotation - HitEffectRotatorList[AnimInstance->PlayerAnimationType]);
			objectpool.SpawnObject(objectpool.ObjectArray[42].ObjClass, OverlappedComponent->GetComponentLocation(), FRotator::ZeroRotator);

			objectpool.SpawnObject(objectpool.ObjectArray[5].ObjClass, OverlappedComponent->GetComponentLocation(), YawRotation - HitEffectRotatorList[AnimInstance->PlayerAnimationType]);
			objectpool.SpawnObject(objectpool.ObjectArray[31].ObjClass, OtherActor->GetActorLocation() + FVector(0, 0, 20.0f), FRotator::ZeroRotator);
		}

		if (PlayerDataStruct.DamageList.Contains(AnimInstance->PlayerAnimationType))
		{
			VibrateGamePad(PlayerDataStruct.DamageList[AnimInstance->PlayerAnimationType].VibrateIntensity, PlayerDataStruct.DamageList[AnimInstance->PlayerAnimationType].VibrateDuration);
			OtherActor->TakeDamage(PlayerDataStruct.BaseDamage * PlayerDataStruct.DamageList[AnimInstance->PlayerAnimationType].Damage, CharacterDamageEvent, nullptr, this);
		}
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
	ABaseCharacter * character = Cast<ABaseCharacter>(OtherActor);
	if (!character->CanExecution)return;
	if (!IsExecute && ExecutionCharacter == nullptr)
	{
		ExecutionCharacter = character;
	}
}

void APlayerCharacter::OnExecutionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsExecute)
	{
		ExecutionCharacter = nullptr;
	}
}

void APlayerCharacter::OnParryingOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::SetGlobalTimeDilation(this, .5f);
	ExecutionCharacter = Cast<ABaseCharacter>(OtherActor);
	CameraShake(PlayerCameraShake);
	Imotal = true;
	BossParryingSequncePlayer->Play();
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[15].ObjClass, OtherComp->GetComponentLocation(), FRotator(90, 180, 0));
	ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayerCharacter::OnShieldOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsGrab)return;
	if (IsExecute) return;
	ExecutionCharacter = Cast<ABaseCharacter>(OtherActor);

	if (ExecutionCharacter == nullptr)return;

	PlayerHUD->ClearShield();
	//UGameplayStatics::SetGlobalTimeDilation(this, .25f);
	VibrateGamePad(0.4f, 0.4f);
	CameraShake(PlayerCameraShake);
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[38].ObjClass, ShieldMeshComp->GetComponentLocation(), FRotator(0, 0, 0));
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[39].ObjClass, ShieldMeshComp->GetComponentLocation(), GetActorRotation() + FRotator(0, 90, 0));
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[40].ObjClass, ShieldMeshComp->GetComponentLocation(), FRotator(0, 0, 0));
	CanShieldDeploy = false;

	PlayerHUD->SetSkill(PlayerDataStruct.ShieldCoolDown);
	GetWorldTimerManager().SetTimer(ShieldCoolDownTimer, this, &APlayerCharacter::RecoverShield, PlayerDataStruct.ShieldCoolDown);

	//ShieldCoolDown

	IsCollisionCamera = true;
	Imotal = true;
	ChangeActionType(ActionType::DEAD);	
	ShieldOff();

	if (TargetComp != nullptr)
	{
		if (TargetComp->GetOwner() != ExecutionCharacter)
		{
			Cast<ABaseCharacter>(TargetComp->GetOwner())->ActivateLockOnImage(false, TargetComp);
			TargetComp = ExecutionCharacter->LockOnComp;
			Cast<ABaseCharacter>(TargetComp->GetOwner())->ActivateLockOnImage(true, TargetComp);
		}
	}

	ExecutionCharacter->Stun();
	ChangeMontageAnimation(AnimationType::ENDOFHEAL);
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
	SkillCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkillAuraComp->Deactivate();
	SkillAuraComp->SetVisibility(false);
	SkillTrailComp->SetVisibility(false);
}

void APlayerCharacter::DeactivateSMOverlap()
{
	Super::DeactivateSMOverlap();
}

void APlayerCharacter::Attack()
{
	if (ExecutionCharacter && ExecutionCharacter->CanExecution)
	{
		if (ExecutionCharacter)
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
	if (!IsGrab)
	{
		Attack();
	}
	else
	{
		if (CanActivate(PlayerDataStruct.ShieldBashSoulCost))
		{
			ShieldAttack();
		}
	}
}

void APlayerCharacter::PowerAttack()
{
	if (ExecutionCharacter && ExecutionCharacter->CanExecution)
	{
		if (ExecutionCharacter)
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

void APlayerCharacter::SkillAttack()
{
	if (CanNextAttack)
	{
		if (UseStamina(PlayerUseStaminaMap[ActionType::SKILL]) && CanUseSkill)
		{
			CanActivate(PlayerDataStruct.SkillSoulCost) ? PlayerAttackType = ActionType::SKILL : PlayerAttackType = ActionType::ATTACK;
			if (PlayerAttackType == ActionType::SKILL)
			{
				GetWorldTimerManager().SetTimer(SkillCoolDownTimer, this, &APlayerCharacter::RecoverSkill, PlayerDataStruct.SkillCoolDown);
			}
			Imotal = false;
			ComboAttackStart();
			CanNextAttack = false;

		}
	}
}

void APlayerCharacter::FadeIn()
{
	PlayerHUD->FadeInAnimation(true);
	GetWorldTimerManager().SetTimer(SprintStartTimer, this, &APlayerCharacter::RespawnCharacter, 2.0f);
}

void APlayerCharacter::ComboAttackEnd()
{	
	PlayerCurAttackIndex = 0;
	CanNextAttack = true;
}

void APlayerCharacter::ComboAttackStart()
{	
	if (IsGrab)
		return;

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
		&& AnimInstance->PlayerAnimationType != AnimationType::HEAL
		&& PlayerCurAction != PlayerAction::CANTACT
		&& AnimInstance->PlayerAnimationType != AnimationType::ENDOFRUN
		&& AnimInstance->PlayerAnimationType != AnimationType::ENDOFSPRINT
		&& AnimInstance->PlayerAnimationType != AnimationType::SUPERHIT)
	{
		Sprint();
	}
}

void APlayerCharacter::PlayerShieldDashMovement()
{
	FRotator ControllerRotation = Controller->GetControlRotation();
	FRotator Rotation = FRotator(0, ControllerRotation.Yaw, 0);
	FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	LaunchCharacter(Direction * PlayerDataStruct.ShieldDashMoveDistance, false, false);
}

void APlayerCharacter::FadeOut()
{
	PlayerHUD->FadeInAnimation(false);
	GetWorld()->GetFirstPlayerController()->EnableInput(GetWorld()->GetFirstPlayerController());
}

void APlayerCharacter::SetCameraTarget(FVector Offset, float Length)
{
	TargetSocketOffset = Offset;
	TargetCameraBoomLength = Length;
}

void APlayerCharacter::ShieldAttack()
{
	if (CanActivate(PlayerDataStruct.ShieldBashSoulCost))
	{
		if (UseStamina(PlayerUseStaminaMap[ActionType::SHIELD]))
		{
			IsGrab = false;
			AObjectPool& objectpool = AObjectPool::GetInstance();
			objectpool.SpawnObject(objectpool.ObjectArray[24].ObjClass, GetActorLocation(), FRotator::ZeroRotator);
			PlayerShieldDashMovement();
			ChangeActionType(ActionType::MOVE);
			ChangeMontageAnimation(AnimationType::SHIELDATTACK);
			ShoulderView(IsShoulderView);
			CameraShake(PlayerCameraShake);
			AnimInstance->BodyBlendAlpha = 1.0f;
			ShieldOverlapComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			ShieldAttackOverlap->SetRelativeLocation(FVector(0, 62.86, 107.34));
		}
	}
}

void APlayerCharacter::SetSoul(int32 value)
{
	PlayerDataStruct.SoulCount += value;

	value < 0 ? PlayerHUD->DecreaseSoulGradual(this, PlayerDataStruct.SoulCount / PlayerDataStruct.MaxSoulCount) : 
		PlayerHUD->SetSoul(PlayerDataStruct.SoulCount / PlayerDataStruct.MaxSoulCount);

	if (PlayerDataStruct.SoulCount <= 0)PlayerDataStruct.SoulCount = 0;
	else if(PlayerDataStruct.SoulCount >= PlayerDataStruct.MaxSoulCount)PlayerDataStruct.SoulCount = 100;
}

void APlayerCharacter::LoadFile()
{
	UJesusSaveGame::GetInstance().Load(this, GameInstance);

	ASoundManager::GetInstance().StartBGMSound(IsPhaseTwo);
	if(IsPhaseTwo)
		UCombatManager::GetInstance().Boss2->SetActive(true);

	SetSoul(PlayerDataStruct.SoulCount);
}

void APlayerCharacter::LoadMap()
{

	FLatentActionInfo LatentInfo;
	UGameplayStatics::LoadStreamLevel(this, SaveMapName, true, true, LatentInfo);

	ALevelLightingManager* LightManager = Cast<ALevelLightingManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelLightingManager::StaticClass()));


	LightManager->ChangeTargetLightSetting(SaveMapName.ToString());

	if (SaveMapName == "2-2Map")
	{
		SaveMapName = "A_KimMinYeongMap_Boss1";
		GetWorldTimerManager().SetTimer(SprintEndTimer, this, &APlayerCharacter::LoadMap, 1.0f);
		UCombatManager::GetInstance().MonsterInfoMap[SaveMapName.ToString()][0]->RespawnCharacter();
	}
	GetWorldTimerManager().SetTimer(DeadTimer, this, &APlayerCharacter::LoadingMonster, 2.0f);
}
void APlayerCharacter::PlayerDead(bool IsFly)
{
	if (IsLockOn)
	{
		LockOn();
	}
	if (TargetComp)
	{
		Cast<ABaseCharacter>(TargetComp->GetOwner())->ActivateLockOnImage(false, TargetComp);
	}

	if (PlayerHUD->IsRender())
	{
		//UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
		PlayerHUD->PlayAnimations(EGuides::dodge, false);
	}

	GetWorld()->GetFirstPlayerController()->DisableInput(GetWorld()->GetFirstPlayerController());
	Imotal = true;
	PlayerHUD->PlayDied(true);
	ChangeActionType(ActionType::DEAD);
	MontageBlendInTime = 0.0f;
	IsFly ? ChangeMontageAnimation(AnimationType::DEADLOOP2) : ChangeMontageAnimation(AnimationType::DEAD);
	IsPhaseTwo = false;
}

void APlayerCharacter::LoadingMonster()
{
	UCombatManager& combatmanager = UCombatManager::GetInstance();

	for (int32 i = 0; i < combatmanager.MonsterInfoArray.Num(); i++)
	{
		combatmanager.MonsterInfoArray[i]->SetActive(false);
	}

	for (int32 i = 0; i < combatmanager.MonsterInfoMap[SaveMapName.ToString()].Num(); i++)
	{
		combatmanager.MonsterInfoMap[SaveMapName.ToString()][i]->RespawnCharacter();
	}
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


		AnimInstance->BodyBlendAlpha = 1.0f;
		ShieldOff();
		ShoulderView(IsShoulderView);
		IsGrab = false;

		return DamageAmount;
	}

	if (DamageAmount > 10)
	{

		AnimInstance->BodyBlendAlpha = 1.0f;
		ShieldOff();
		ShoulderView(IsShoulderView);
		IsGrab = false;

		UCombatManager::GetInstance().ActivateCollider();

		AnimInstance->StopMontage(MontageMap[AnimInstance->PlayerAnimationType]);
		if (MontageEndEventMap.Contains(AnimInstance->PlayerAnimationType))
			MontageEndEventMap[AnimInstance->PlayerAnimationType]();

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
		GetWorld()->GetFirstPlayerController()->DisableInput(GetWorld()->GetFirstPlayerController());
		ASoundManager::GetInstance().PlaySoundWithCymbalSound(3);
		PlayerDead(false);
	}
	return DamageAmount;
}
