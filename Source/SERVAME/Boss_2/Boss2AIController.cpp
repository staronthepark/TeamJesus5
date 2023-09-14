// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss2AIController.h"
#include "..\Player\PlayerCharacter.h"
#include "JesusBoss2.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


ABoss2AIController::ABoss2AIController(const FObjectInitializer& ObjectInitializer)
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	Sight->SightRadius = 1500.f;
	Sight->LoseSightRadius = Sight->SightRadius + 500.f;
	Sight->PeripheralVisionAngleDegrees = 360.f;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComponent->ConfigureSense(*Sight);
	AIPerceptionComponent->SetDominantSense(Sight->GetSenseImplementation());

	static ConstructorHelpers::FClassFinder<UBossUI> BossUIAsset(TEXT("/Game/02_Resource/04_UI/01_WBP/02_BossUI/WBP_BossUI"));

	if (BossUIAsset.Succeeded())
		BossUIClass = BossUIAsset.Class;
}

void ABoss2AIController::BeginPlay()
{
	Super::BeginPlay();

	AJesusBoss2* Boss2Pawn = Cast<AJesusBoss2>(GetPawn());
	if (Boss2Pawn)
		Boss2 = Boss2Pawn;

	// UI
	if (IsValid(BossUIClass))
	{
		BossUI = Cast<UBossUI>(CreateWidget(GetWorld(), BossUIClass));
	}

	BossUI->SetHP(1);
}

void ABoss2AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Boss2->GetCharacterMovement()->bUseControllerDesiredRotation = true;

	if (!Boss2->IsStartBoneRot && IsPerception)
	{
		Boss2->Boss2AnimInstance->CurrentBoneType = Boss2BoneRotateType::TURNHEAD;
	}

	if (Boss2->IsLockOn)
	{
		GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsLockOn")), true);
		Boss2->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
	else
	{
		GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsLockOn")), false);
		Boss2->GetCharacterMovement()->bUseControllerDesiredRotation = false;
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
					Boss2->PlayerCharacter->UserSettingUI->WBP_UserSetting_GameUI->WBP_Language_Button->LeftButton->OnClicked.AddDynamic(this, &ABoss2AIController::ChangeLanguage);
					Boss2->PlayerCharacter->UserSettingUI->WBP_UserSetting_GameUI->WBP_Language_Button->RightButton->OnClicked.AddDynamic(this, &ABoss2AIController::ChangeLanguage);
					BossUI->PlayBossHPOpenAnimation(true, EBossSettings::phase2);
				}
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

	//if (DetectedActorArr.Num() >= 1 && !TestBool)
	//{
	//	Boss2->ChangeMontageAnimation(Boss2AnimationType::CROSSFALL);
	//	TestBool = true;
	//}
}

void ABoss2AIController::OnPossess(APawn* InPawn)
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
	
	if (Boss2 != nullptr)
	{
		Boss2->CanMove = true;
		Boss2->IsLockOn = true;
		Boss2->Boss2AnimInstance->IsStart = true;
	}

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABoss2AIController::OnPerception);
}

void ABoss2AIController::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void ABoss2AIController::OnPerception(AActor* Actor, FAIStimulus Stimulus)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Actor);

	if (Player == nullptr)
		return;

	UE_LOG(LogTemp, Log, TEXT("OnPerception"));
	//Boss->SetAnimState(Stimulus.WasSuccessfullySensed());
	SetFocus(Stimulus.WasSuccessfullySensed() ? Player : nullptr);
	IsPerception = true;
}

void ABoss2AIController::MoveWhenArrived(FVector Location)
{
	IsArrived = false;

	auto type = MoveToLocation(Location);

	if (type == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		IsArrived = true;
		Boss2->CircleIndexCount++;
	}
}

void ABoss2AIController::ChangeLanguage()
{
	BossUI->ChangeLanguage();
}


