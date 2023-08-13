
#include "KinghtMonster.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Math/RandomStream.h"
#include "KnightAttackTriggerComp.h"
#include "..\..\Manager\CombatManager.h"
#include "KnightAttackTriggerComp.h"
#include "GameFramework/CharacterMovementComponent.h"

AKinghtMonster::AKinghtMonster()
{
	AttackTrigger = CreateDefaultSubobject<UKnightAttackTriggerComp>(TEXT("AttackTriggerCollision"));
	AttackTrigger->SetupAttachment(GetMesh());

	DashAttackTrigger = CreateDefaultSubobject<UKnightAttackTriggerComp>(TEXT("DashAttackTriggerCollision"));
	DashAttackTrigger->SetupAttachment(GetMesh());

	MonsterMoveMap.Add(3, [&]()
		{
			if (CircleWalkEnd == false)
				MonsterController->MoveWhenArrived(CirclePoints[CircleIndexCount]);
		});

	SetActionByRandomMap.Add(MonsterAnimationType::DASHATTACK1, [&](float percent)
		{
			if (percent <= 0.45f)
			{
				ChangeActionType(MonsterActionType::ATTACK);
				ChangeMontageAnimation(MonsterAnimationType::DASHATTACK1);
			}
			else
			{
				DrawCircle(PlayerCharacter->GetActorLocation());
				CircleWalkEnd = false;
				MonsterMoveEventIndex = 3;
				DashAttackTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			
				GetWorldTimerManager().SetTimer(CircleWalkTimerHandle, FTimerDelegate::CreateLambda([=]()
					{					
						if (MonsterDataStruct.CharacterHp > 0 && MonsterController->FindPlayer)
						{
							CircleWalkEnd = true;
							MonsterMoveEventIndex = 1;
							ChangeActionType(MonsterActionType::MOVE);
							ChangeMontageAnimation(MonsterAnimationType::FORWARDMOVE);

							DashAttackTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
						}
						//GetWorldTimerManager().ClearTimer(CircleWalkTimerHandle);
					}), CircleWalkMinTime, false);

				ChangeActionType(MonsterActionType::MOVE);
				ChangeMontageAnimation(MonsterAnimationType::LEFTMOVE);
			}
		});
}

void AKinghtMonster::BeginPlay()
{
	Super::BeginPlay(); 

	const FTransform socket = GetMesh()->GetSocketTransform("Bip001-Spine2", ERelativeTransformSpace::RTS_World);
	auto Armor = GetWorld()->SpawnActor(ArmorClass, &socket);

	if (Armor != nullptr)
	{
		Armor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "Bip001-Spine2");
		KnightArmor = Cast<AKnightArmor>(Armor);
	}

	SetActive(true);

	PlayerCharacter = nullptr;

	KnightAnimInstance = Cast<UKnightAnimInstance>(GetMesh()->GetAnimInstance());

	TargetDetectionCollison->OnComponentBeginOverlap.AddDynamic(this, &AKinghtMonster::OnKnightTargetDetectionBeginOverlap);
	TargetDetectionCollison->OnComponentEndOverlap.AddDynamic(this, &AKinghtMonster::OnKnightTargetDetectionEndOverlap);

	if (KnightAnimInstance != nullptr)
	{
		KnightAnimInstance->InterpStart.AddUObject(this, &AKinghtMonster::InterpStart);
		KnightAnimInstance->InterpEnd.AddUObject(this, &AKinghtMonster::InterpEnd);
		KnightAnimInstance->KnockBackStart.AddUObject(this, &AKinghtMonster::KnockBackStart);
		KnightAnimInstance->KnockBackEnd.AddUObject(this, &AKinghtMonster::KnockBackEmd);
	}
}

void AKinghtMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("%d"), ActionType);

	if (IsInterpStart)
		InterpMove();
}

void AKinghtMonster::RespawnCharacter()
{
	Super::RespawnCharacter();

	WeaponOpacity = 0.171653f;
	MeshOpacity = 0.171653f;

	ActivateHitCollision();
	MonsterDataStruct.CharacterHp = MonsterDataStruct.CharacterMaxHp;
	MonsterHPWidget->SetHP(1.0f);
}

void AKinghtMonster::ResumeMontage()
{
	if (MontageMap.Contains(AnimationType))
		KnightAnimInstance->ResumeMontage(MontageMap[AnimationType]);
}

void AKinghtMonster::InterpStart() { IsInterpStart = true; }

void AKinghtMonster::InterpEnd() { IsInterpStart = false; }

void AKinghtMonster::KnockBackStart()
{
	if (!KnightArmor->IsBroke)
		return;

	GetWorld()->GetTimerManager().SetTimer(KnockBackTimerHandle, FTimerDelegate::CreateLambda([=]()
		{
			IsKnockBack = false;
			GetWorld()->GetTimerManager().ClearTimer(KnockBackTimerHandle);
		}), KnockBackTime, false);

	GetWorld()->GetTimerManager().SetTimer(KnockBackDelayTimerHandle, FTimerDelegate::CreateLambda([=]()
		{
			ActivateAttackTrigger();
			MontageEndEventMap[MonsterAnimationType::HIT]();
			GetWorld()->GetTimerManager().ClearTimer(KnockBackDelayTimerHandle);
		}), KnockBackDelayTime, false);

	StopAnimMontage(MontageMap[AnimationType]); 
	DeactivateAttackTrigger();
	IsKnockBack = true; 
}

void AKinghtMonster::KnockBackEmd() 
{ 
	IsKnockBack = false; 
}

void AKinghtMonster::Stun()
{
	KnightAnimInstance->StopMontage(MontageMap[AnimationType]);
	MonsterController->StopMovement();
	DeactivateSMOverlap();
	ParryingCollision1->Deactivate();
	DeactivateRightWeapon();
	ChangeMontageAnimation(MonsterAnimationType::DEAD);
}

void AKinghtMonster::ChangeMontageAnimation(MonsterAnimationType type)
{
	if (KnightAnimInstance == nullptr)
	{
		KnightAnimInstance = Cast<UKnightAnimInstance>(GetMesh()->GetAnimInstance());
	}
	else
	{
		KnightAnimInstance->StopMontage(MontageMap[AnimationType]);
		AnimationType = type;
		StateType = AnimTypeToStateType[type];
		KnightAnimInstance->PlayMontage(MontageMap[type]);
	}
}

void AKinghtMonster::InterpMove()
{
	auto NewLoc = FMath::Lerp(GetActorLocation(), PlayerCharacter->GetActorLocation(), 0.1f);
	SetActorLocation(NewLoc);
}

void AKinghtMonster::MonsterHitStop()
{
	if (MontageMap.Contains(AnimationType))
		KnightAnimInstance->PauseAnimation(MontageMap[AnimationType]);
}
void AKinghtMonster::ActivateAttackTrigger()
{
	AttackTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DashAttackTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AKinghtMonster::DeactivateAttackTrigger()
{
	AttackTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DashAttackTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKinghtMonster::OnKnightTargetDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO : 
	//소환되자 마자 플레이어 추격 하도록 변경?
	ActivateHpBar();

	if (ActionType == MonsterActionType::DEAD)
		return;
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	}
}

void AKinghtMonster::OnKnightTargetDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AttackAnimationType = MonsterAnimationType::NONE;
}

void AKinghtMonster::StartAttackTrigger(MonsterAnimationType AttackAnimType)
{
	TracePlayer = false;
	if (StateType == MonsterStateType::CANTACT || GetMesh()->GetCollisionProfileName() == "Ragdoll" || IsKnockBack == true)
		return;
	AttackAnimationType = AttackAnimType;
	if (ActionType != MonsterActionType::ATTACK)
	{
		MonsterController->StopMovement();
		KnightAnimInstance->StopMontage(MontageMap[AnimationType]);

		float RandomValue = FMath::RandRange(0, 100) * 0.01f;
		if (SetActionByRandomMap.Contains(AttackAnimType))
		{
			MonsterMoveEventIndex = 1;
			SetActionByRandomMap[AttackAnimType](RandomValue);
		}
	}
}

void AKinghtMonster::EndAttackTrigger(MonsterAnimationType AttackAnimType)
{
	if (AnimationType == MonsterAnimationType::DEAD || AnimationType == MonsterAnimationType::DEADLOOP)
		return;
	TracePlayer = true;
}

void AKinghtMonster::Rotate()
{
	if (AnimationType == MonsterAnimationType::DEAD || AnimationType == MonsterAnimationType::DEADLOOP
		|| AnimationType == MonsterAnimationType::EXECUTION)
		return;
	SetActorRotation(FMath::Lerp(GetActorRotation(), YawRotation, MonsterDataStruct.RotateSpeed * fDeltaTime));
}

void AKinghtMonster::DrawCircle(FVector Center)
{
	CirclePoints.SetNum(NumSegments);

	const float AngleBetweenSegments = 2 * PI / NumSegments;

	// 원을 구성하는 점들 계산
	int ClosestPointIndex = 0;
	float MinDistance = FLT_MAX;

	for (int i = 1; i < NumSegments; ++i)
	{
		float Angle = i * AngleBetweenSegments;
		float X = Center.X + Radius * FMath::Cos(Angle);
		float Y = Center.Y + Radius * FMath::Sin(Angle);
		FVector Point(X, Y, Center.Z);
		CirclePoints[i] = Point;

		float Distance = FVector::Dist(Point, GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosestPointIndex = i;
		}
	}

	// ClosestPointIndex부터 시작하여 원의 점들 재배열
	TArray<FVector> TempArray;
	TempArray.Append(CirclePoints.GetData() + ClosestPointIndex, CirclePoints.Num() - ClosestPointIndex);
	TempArray.Append(CirclePoints.GetData() + 1, ClosestPointIndex);
	CirclePoints = TempArray;

	if (DrawDebugCircle)
	{
		// 점들을 이어서 원 그리기
		for (int32 i = 0; i < NumSegments; ++i)
		{
			FVector Start = CirclePoints[i];
			FVector End = CirclePoints[(i + 1) % NumSegments];

			if (i == 0)
				DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 5.0f);
			else
				DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 5.0f);
		}
	}
}

float AKinghtMonster::Die(float Dm)
{
	if (MonsterDataStruct.CharacterHp <= 0)
	{
		Imotal = true;

		KnightAnimInstance->StopMontage(MontageMap[AnimationType]);
		ChangeActionType(MonsterActionType::DEAD);
		StateType = MonsterStateType::CANTACT;

		MonsterController->StopMovement();
		DeactivateSMOverlap();
		ParryingCollision1->Deactivate();
		DeactivateRightWeapon();
		//UGameplayStatics::SetGlobalTimeDilation(this, 0.1f);
		ChangeMontageAnimation(MonsterAnimationType::DEAD);

		return Dm;
	}

	return Dm;
}

float AKinghtMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (KnightArmor->IsBroke)
		Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	IsInterpStart = false;
	DeactivateHitCollision();

	if (AnimationType == MonsterAnimationType::EXECUTION)
		return 0.f;

	if (!KnightArmor->IsBroke)
	{
		KnightArmor->ArmorDataStruct.ArmorHp -= DamageAmount;
		AObjectPool& objectpool = AObjectPool::GetInstance();
		objectpool.SpawnObject(objectpool.ObjectArray[38].ObjClass, GetActorLocation(), FRotator::ZeroRotator);
		objectpool.SpawnObject(objectpool.ObjectArray[39].ObjClass, GetActorLocation(), FRotator::ZeroRotator);
	}
	else if (DamageAmount >= 30 && MonsterDataStruct.CharacterHp > 0)
	{
		MonsterController->StopMovement();
		KnightAnimInstance->StopMontage(MontageMap[AnimationType]);
		if (MontageEndEventMap.Contains(AnimationType))
			MontageEndEventMap[AnimationType]();
		
		ChangeActionType(MonsterActionType::HIT);
		ChangeMontageAnimation(MonsterAnimationType::HIT);
	}

	

	return DamageAmount;
}

void AKinghtMonster::IsNotifyActive(bool value)
{
	if (NotifyBeginEndEventMap.Contains(AnimationType))
	{
		NotifyBeginEndEventMap[AnimationType][value]();
	}
}

void AKinghtMonster::CheckMontageEndNotify()
{
	if (MontageEndEventMap.Contains(AnimationType))
	{
		MontageEndEventMap[AnimationType]();
	}
}

void AKinghtMonster::PlayExecutionAnimation()
{
	IsStun = false;
	CanExecution = false;
	ChangeMontageAnimation(MonsterAnimationType::EXECUTION);
}
