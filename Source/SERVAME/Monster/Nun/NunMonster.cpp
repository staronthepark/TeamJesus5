// Fill out your copyright notice in the Description page of Project Settings.


#include "NunMonster.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Math/RandomStream.h"
#include "..\..\Manager\CombatManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NunAttackTriggerComp.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"

ANunMonster::ANunMonster()
{
	AttackTrigger = CreateDefaultSubobject<UNunAttackTriggerComp>(TEXT("AttackTriggerCollision"));
	AttackTrigger->SetupAttachment(GetMesh());

	AnimTypeToStateType.Add(MonsterAnimationType::HEAL1, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::HEAL2, MonsterStateType::BEFOREATTACK);
	AnimTypeToStateType.Add(MonsterAnimationType::SPAWNKNIGHT, MonsterStateType::BEFOREATTACK);

	MonsterMoveMap.Add(1, [&]()
		{
		});

	TargetDetectEventMap.Add(MonsterAttackType::MELEE, [&]()
		{
			ChangeActionType(MonsterActionType::NONE);
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});
	
	NotifyBeginEndEventMap.Add(MonsterAnimationType::HEAL1, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::HEAL1].Add(true, [&]()
		{
			SingleHeal();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::HEAL1].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::HEAL2, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::HEAL2].Add(true, [&]()
		{
			MultiHeal();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::HEAL2].Add(false, [&]()
		{
		});

	NotifyBeginEndEventMap.Add(MonsterAnimationType::SPAWNKNIGHT, TMap<bool, TFunction<void()>>());
	NotifyBeginEndEventMap[MonsterAnimationType::SPAWNKNIGHT].Add(true, [&]()
		{
			SpawnKnight();
		});
	NotifyBeginEndEventMap[MonsterAnimationType::SPAWNKNIGHT].Add(false, [&]()
		{
		});

	MontageEndEventMap.Add(MonsterAnimationType::ATTACK1, [&]()
		{
			ChangeActionType(MonsterActionType::NONE);
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});

	MontageEndEventMap.Add(MonsterAnimationType::HEAL1, [&]()
		{
			ChangeActionType(MonsterActionType::NONE);
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});

	MontageEndEventMap.Add(MonsterAnimationType::HEAL2, [&]()
		{
			ChangeActionType(MonsterActionType::NONE);
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});

	MontageEndEventMap.Add(MonsterAnimationType::SPAWNKNIGHT, [&]()
		{
			ChangeActionType(MonsterActionType::NONE);
			ChangeMontageAnimation(MonsterAnimationType::IDLE);
		});

	SetActionByRandomMap.Add(MonsterAnimationType::ATTACK1, [&](float percent)
		{
			if (percent >= 0.5)
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::ATTACK1);
			}
			else if (percent < 0.5f)
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::ATTACK1);
			}
		});

	SetActionByRandomMap.Add(MonsterAnimationType::HEAL1, [&](float percent)
		{
			if (percent >= 0.5)
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::HEAL1);
			}
			else if (percent < 0.5f)
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::HEAL2);
			}
		});

	SetActionByRandomMap.Add(MonsterAnimationType::HEAL2, [&](float percent)
		{
			if (percent >= 0.5)
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::HEAL1);
			}
			else if (percent < 0.5f)
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::HEAL2);
			}
		});

	SetActionByRandomMap.Add(MonsterAnimationType::SPAWNKNIGHT, [&](float percent)
		{
			ChangeActionType(MonsterActionType::ATTACK);
			ChangeMontageAnimation(MonsterAnimationType::SPAWNKNIGHT);
		});
}

void ANunMonster::BeginPlay()
{
	Super::BeginPlay();

	NunAnimInstance = Cast<UNumAnimInstance>(GetMesh()->GetAnimInstance());
	WeaponOverlapStaticMeshCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponCollision->SetupAttachment(GetMesh(), FName("Nun_Weapon_Bone"));

	PlayerCharacter = nullptr;
	
	TargetDetectionCollison->OnComponentBeginOverlap.AddDynamic(this, &ANunMonster::OnNunTargetDetectionBeginOverlap);
	TargetDetectionCollison->OnComponentEndOverlap.AddDynamic(this, &ANunMonster::OnNunTargetDetectionEndOverlap);

	if (NunAnimInstance != nullptr)
	{
		
	}

	MonsterMoveEventIndex = 1;
}

void ANunMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("%d"), ActionType);
}

void ANunMonster::OnNunTargetDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ActivateHpBar();

	if (ActionType == MonsterActionType::DEAD)
		return;
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	}
}

void ANunMonster::OnNunTargetDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//AttackAnimationType = MonsterAnimationType::NONE;
}

void ANunMonster::StartAttackTrigger(MonsterAnimationType AttackAnimType)
{
	if (NunAnimInstance == nullptr)
	{
		NunAnimInstance = Cast<UNumAnimInstance>(GetMesh()->GetAnimInstance());
	}

	TracePlayer = false;
	if (StateType == MonsterStateType::CANTACT)
		return;
	AttackAnimationType = AttackAnimType;

	//TODO : 거리별 패턴을 위한 Map 생성해주기
	if (CurrentDistance >= 1000.f)
		AttackAnimationType = MonsterAnimationType::HEAL1;
	else if(CurrentDistance >= 800.f)
		AttackAnimationType = MonsterAnimationType::HEAL2;

	if (ActionType != MonsterActionType::ATTACK)
	{
		MonsterController->StopMovement();
		NunAnimInstance->StopMontage(MontageMap[AnimationType]);

		float RandomValue = FMath::RandRange(0, 100) * 0.01f;
		if (SetActionByRandomMap.Contains(AttackAnimType))
		{
			MonsterMoveEventIndex = 1;
			SetActionByRandomMap[AttackAnimType](RandomValue);
		}
	}
}

void ANunMonster::EndAttackTrigger(MonsterAnimationType AttackAnimType)
{
	if (AnimationType == MonsterAnimationType::DEAD || AnimationType == MonsterAnimationType::DEADLOOP)
		return;

	//TracePlayer = true;

	AttackAnimationType = AttackAnimType;
}

float ANunMonster::Die(float Dm)
{
	if (MonsterDataStruct.CharacterHp <= 0)
	{
		Imotal = true;
		//UGameplayStatics::SetGlobalTimeDilation(this, 0.1f);
		//ChangeMontageAnimation(MonsterAnimationType::DEAD);
		//AnimInstance->StopMontage(MontageMap[AnimationType]);
		ChangeActionType(MonsterActionType::DEAD);
		StateType = MonsterStateType::CANTACT;
		//PlayerCharacter->PlayerHUD->PlayAnimations(EGuides::grogy, true);
		return Dm;
	}

	return Dm;
}

void ANunMonster::SpawnKnight()
{

}

void ANunMonster::MultiHeal()
{
	//스피어 콜라이더 만들어서 처리
}

void ANunMonster::SingleHeal()
{
	TArray<float> KnightHpArr;
	int Min;
	int index = 0;

	for (int i = 0; i < KnightArr.Num(); i++)
		KnightHpArr.Push(KnightArr[i]->MonsterDataStruct.CharacterHp);
	
	Min = KnightHpArr[0];

	for (int i = 0; i < KnightHpArr.Num(); i++)
	{
		if (Min > KnightHpArr[i])
		{
			Min = KnightHpArr[i];
			index = i;
		}
	}

	auto TargetKnight = KnightArr[index];

	TargetKnight->MonsterDataStruct.CharacterHp += HealVal;

	if (TargetKnight->MonsterDataStruct.CharacterHp > TargetKnight->MonsterDataStruct.CharacterMaxHp)
		TargetKnight->MonsterDataStruct.CharacterHp = TargetKnight->MonsterDataStruct.CharacterMaxHp;

	float CurrentPercent = TargetKnight->MonsterDataStruct.CharacterHp / TargetKnight->MonsterDataStruct.CharacterMaxHp;
	TargetKnight->MonsterHPWidget->SetHP(CurrentPercent);
}

void ANunMonster::Stun()
{
	NunAnimInstance->StopMontage(MontageMap[AnimationType]);
	MonsterController->StopMovement();
	DeactivateSMOverlap();
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();
	ChangeMontageAnimation(MonsterAnimationType::DEAD);
}

void ANunMonster::MonsterHitStop()
{
	if (MontageMap.Contains(AnimationType))
		NunAnimInstance->PauseAnimation(MontageMap[AnimationType]);
}

void ANunMonster::ChangeMontageAnimation(MonsterAnimationType type)
{
	if (NunAnimInstance == nullptr)
	{
		NunAnimInstance = Cast<UNumAnimInstance>(GetMesh()->GetAnimInstance());
	}
	else
	{
		NunAnimInstance->StopMontage(MontageMap[AnimationType]);
		AnimationType = type;
		StateType = AnimTypeToStateType[type];
		NunAnimInstance->PlayMontage(MontageMap[type]);
	}
}

float ANunMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	ChangeActionType(MonsterActionType::NONE);

	if (Imotal)
	{
		return 0;
	}

	//MonsterHpWidget->Hp->SetVisibility(ESlateVisibility::Visible);
	//MonsterHpWidget->HpBG->SetVisibility(ESlateVisibility::Visible);
	//GetWorldTimerManager().SetTimer(HpTimer, this, &AEnemyMonster::DeactivateHpBar, 3.0f);

	DeactivateHitCollision();

	MonsterHPWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	MonsterDataStruct.CharacterHp -= DamageAmount;

	float CurrentPercent = MonsterDataStruct.CharacterHp / MonsterDataStruct.CharacterMaxHp;
	MonsterHPWidget->DecreaseHPGradual(this, CurrentPercent);

	Die(DamageAmount);

	if ((MonsterDataStruct.CharacterMaxHp * TeleportVal) <= MonsterDataStruct.CharacterMaxHp - MonsterDataStruct.CharacterHp)
	{
		TelePort();
		SpawnKnight();
	}

	return DamageAmount;
}

void ANunMonster::TelePort()
{
	//TODO : 텔레포트 좌표로 쓸 액터 블루프린트로 만들고 태그 지정해준 다음
	//태그로 액터 찾아서 배열 자동 할당 해주기.
	//기사도 똑같이 태그로 찾아서 할당 해주기.
	srand(time(NULL));

	auto Num = rand() % TeleportArr.Num();

	SetActorLocation(TeleportArr[Num]->GetActorLocation());

	ChangeActionType(MonsterActionType::NONE);
	ChangeMontageAnimation(MonsterAnimationType::IDLE);
}

void ANunMonster::CheckMontageEndNotify()
{
	if (MontageEndEventMap.Contains(AnimationType))
	{
		MontageEndEventMap[AnimationType]();
	}
}

void ANunMonster::PlayExecutionAnimation()
{
	IsStun = false;
	CanExecution = false;
	ChangeMontageAnimation(MonsterAnimationType::EXECUTION);
}

void ANunMonster::IsNotifyActive(bool value)
{
	if (NotifyBeginEndEventMap.Contains(AnimationType))
	{
		NotifyBeginEndEventMap[AnimationType][value]();
	}
}

void ANunMonster::RespawnCharacter()
{
	Super::RespawnCharacter();

	WeaponOpacity = 0.171653f;
	MeshOpacity = 0.171653f;

	ActivateHitCollision();
	MonsterDataStruct.CharacterHp = MonsterDataStruct.CharacterMaxHp;
	MonsterHPWidget->SetHP(1.0f);
}

void ANunMonster::ResumeMontage()
{
	if (MontageMap.Contains(AnimationType))
		NunAnimInstance->ResumeMontage(MontageMap[AnimationType]);
}
