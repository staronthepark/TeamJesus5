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

	MonsterMoveMap.Add(1, [&]()
		{
		});

	//TODO : SetActionByRandomMap 재정의 해서 각종 패턴 넣어주기.
	
}

void ANunMonster::BeginPlay()
{
	Super::BeginPlay();

	SetActive(true);

	DeactivateHpBar();

	NunAnimInstance = Cast<UNumAnimInstance>(GetMesh()->GetAnimInstance());
	WeaponOverlapStaticMeshCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ParryingCollision1->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

	if (NunAnimInstance == nullptr)
	{
		NunAnimInstance = Cast<UNumAnimInstance>(GetMesh()->GetAnimInstance());
	}
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
	AttackAnimationType = MonsterAnimationType::NONE;
}

void ANunMonster::StartAttackTrigger(MonsterAnimationType AttackAnimType)
{
	TracePlayer = false;
	if (StateType == MonsterStateType::CANTACT)
		return;
	AttackAnimationType = AttackAnimType;
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
	TracePlayer = true;
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
	
	if (Imotal)
	{
		return 0;
	}

	//MonsterHpWidget->Hp->SetVisibility(ESlateVisibility::Visible);
	//MonsterHpWidget->HpBG->SetVisibility(ESlateVisibility::Visible);
	GetWorldTimerManager().SetTimer(HpTimer, this, &AEnemyMonster::DeactivateHpBar, 3.0f);

	DeactivateHitCollision();

	MonsterHPWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	MonsterDataStruct.CharacterHp -= DamageAmount;

	float CurrentPercent = MonsterDataStruct.CharacterHp / MonsterDataStruct.CharacterMaxHp;
	MonsterHPWidget->DecreaseHPGradual(this, CurrentPercent);

	Die(DamageAmount);

	if ((MonsterDataStruct.CharacterMaxHp * TeleportVal) <= MonsterDataStruct.CharacterMaxHp-MonsterDataStruct.CharacterHp)
		TelePort();

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
