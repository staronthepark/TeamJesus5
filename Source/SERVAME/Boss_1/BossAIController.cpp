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

	AJesusBoss* BossPawn = Cast<AJesusBoss>(GetPawn());
	if (BossPawn)
	{
		Boss = BossPawn;
	}

	GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsDead")), false);
	GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsPlayerAlive")), true);
	GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("IsStep")), false);
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

	if (GetBlackboardComponent()->GetValueAsBool("IsPlayerAlive"))
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
				}
				ASoundManager::GetInstance().PlaySoundWithCymbalSound(1);
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
				BossUI->RemoveFromViewport();
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
}

void ABossAIController::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

