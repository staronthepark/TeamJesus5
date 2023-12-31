// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIController.h"
#include "..\Player\PlayerCharacter.h"
#include "JesusBoss.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "..\Manager\SoundManager.h"
#include "GameFramework/CharacterMovementComponent.h"

void ABossAIController::BeginPlay()
{
	Super::BeginPlay();

	//AJesusBoss* BossPawn = Cast<AJesusBoss>(GetPawn());
	//if (BossPawn)
	//{
	//	Boss = BossPawn;
	//}

	//auto BlackboardComponent = GetBlackboardComponent();
	//if (BlackboardComponent != nullptr)
	//{
	//	BlackboardComponent->SetValueAsBool(FName(TEXT("IsDead")), false);
	//	BlackboardComponent->SetValueAsBool(FName(TEXT("IsPlayerAlive")), true);
	//	BlackboardComponent->SetValueAsEnum(FName(TEXT("IsStep")), false);
	//}

	//if (Boss != nullptr)
	//	Boss->IsLockOn = true;

	//// UI
	//if (IsValid(BossUIClass))
	//{
	//	BossUI = Cast<UBossUI>(CreateWidget(GetWorld(), BossUIClass));

	//	//if (IsValid(BossUI))
	//	//{
	//	//	BossUI->AddToViewport();
	//	//}
	//}
	////GameInstance->PlayerHUDWidget->PB_BossHP->SetPercent((float)BossDataStruct.CharacterHp / (float)BossDataStruct.CharacterMaxHp);
	//BossUI->SetHP(1);
}

void ABossAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Boss->IsLockOn)
	{
		GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsLockOn")), true);
		Boss->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
	else
	{
		GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsLockOn")), false);
		Boss->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	}

	if (GetBlackboardComponent()->GetValueAsBool("IsPlayerAlive") && !Boss->IsDead)
	{
		if (DetectedActorArr.Num() >= 1)
		{
			GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsDetected")), true);

			if (!IsUIActivate.Exchange(true))
			{
				UE_LOG(LogTemp, Warning, TEXT("Visible"));
				if (IsValid(BossUI))
				{
					BossUI->AddToViewport();
					Boss->PlayerCharacter->UserSettingUI->WBP_UserSetting_GameUI->WBP_Language_Button->LeftButton->OnClicked.AddDynamic(this, &ABossAIController::ChangeLanguage);
					Boss->PlayerCharacter->UserSettingUI->WBP_UserSetting_GameUI->WBP_Language_Button->RightButton->OnClicked.AddDynamic(this, &ABossAIController::ChangeLanguage);
					BossUI->PlayBossHPOpenAnimation(true, EBossSettings::phase1);
				}
				ASoundManager::GetInstance().PlaySoundWithCymbalSound(BGMType::TITLEVERSE1, true);
			}
			
			return;
		}
		AIPerceptionComponent->GetCurrentlyPerceivedActors(UAISenseConfig_Sight::StaticClass(), DetectedActorArr);
	}
	else
	{
		DetectedActorArr.Empty();
		GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsDetected")), false);

		if (IsUIActivate.Exchange(false))
		{
			UE_LOG(LogTemp, Warning, TEXT("Hidden"));
			if (IsValid(BossUI))
			{
				BossUI->RemoveFromParent();
			}
		}
	}
}

ABossAIController::ABossAIController(const FObjectInitializer& ObjectInitializer)
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	Sight->SightRadius = 4000.f;
	Sight->LoseSightRadius = Sight->SightRadius + 4000.f;
	Sight->PeripheralVisionAngleDegrees = 360.f;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComponent->ConfigureSense(*Sight);
	AIPerceptionComponent->SetDominantSense(Sight->GetSenseImplementation());

	static ConstructorHelpers::FClassFinder<UBossUI> BossUIAsset(TEXT("/Game/02_Resource/04_UI/01_WBP/02_BossUI/WBP_BossUI"));

	if (BossUIAsset.Succeeded())
		BossUIClass = BossUIAsset.Class;
}

void ABossAIController::OnPerception(AActor* Actor, FAIStimulus Stimulus)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Actor);

	if (Player == nullptr)
		return;

	UE_LOG(LogTemp, Log, TEXT("OnPerception"));
	//Boss->SetAnimState(Stimulus.WasSuccessfullySensed());
	SetFocus(Stimulus.WasSuccessfullySensed() ? Player : nullptr);
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UBlackboardComponent* BlackBoardComp = Blackboard.Get();

	if (UseBlackboard(BlackboardData, BlackBoardComp))
	{
		UE_LOG(LogTemp, Log, TEXT("UseBlackboard"));
	
		if (RunBehaviorTree(BehaviorTree))
		{
			UE_LOG(LogTemp, Log, TEXT("RunBehaviorTree"));
		}
	}
	
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABossAIController::OnPerception);


	AJesusBoss* BossPawn = Cast<AJesusBoss>(GetPawn());
	if (BossPawn)
	{
		Boss = BossPawn;
	}

	auto BlackboardComponent = GetBlackboardComponent();
	if (BlackboardComponent != nullptr)
	{
		BlackboardComponent->SetValueAsBool(FName(TEXT("IsDead")), false);
		BlackboardComponent->SetValueAsBool(FName(TEXT("IsPlayerAlive")), true);
		BlackboardComponent->SetValueAsEnum(FName(TEXT("IsStep")), false);
	}

	if (Boss != nullptr)
		Boss->IsLockOn = true;

	// UI
	if (IsValid(BossUIClass))
	{
		BossUI = Cast<UBossUI>(CreateWidget(GetWorld(), BossUIClass));

		//if (IsValid(BossUI))
		//{
		//	BossUI->AddToViewport();
		//}
	}
	//GameInstance->PlayerHUDWidget->PB_BossHP->SetPercent((float)BossDataStruct.CharacterHp / (float)BossDataStruct.CharacterMaxHp);

	BossUI->SetHP(1);

	Boss->DeactivateSMOverlap();
	Boss->WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Boss->DarkExplosionCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Boss->DamageSphereTriggerComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABossAIController::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void ABossAIController::ChangeLanguage()
{
	BossUI->ChangeLanguage();
}
