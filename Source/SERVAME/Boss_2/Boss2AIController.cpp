// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss2AIController.h"
#include "..\Player\\PlayerCharacter.h"
#include "JesusBoss2.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}

void ABoss2AIController::BeginPlay()
{
	Super::BeginPlay();

	AJesusBoss2* Boss2Pawn = Cast<AJesusBoss2>(GetPawn());
	if (Boss2Pawn)
		Boss2 = Boss2Pawn;

}

void ABoss2AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Boss2->GetCharacterMovement()->bUseControllerDesiredRotation = true;

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
			return;
		}
		AIPerceptionComponent->GetCurrentlyPerceivedActors(UAISenseConfig_Sight::StaticClass(), DetectedActorArr);
	}
	else
	{
		DetectedActorArr.Empty();
		GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsDetected")), false);
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
}


