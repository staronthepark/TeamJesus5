// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_LockOn.h"
#include "..\Boss_1\BossAIController.h"
#include "..\Boss_1\JesusBoss.h"
#include "..\Player\PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_LockOn::UBTDecorator_LockOn()
{
	NodeName = TEXT("LockOn");
}

bool UBTDecorator_LockOn::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	auto Boss = Cast<AJesusBoss>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("SelfActor"))));

	if(Boss->IsLockOn)
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsLockOn")), true);
	else
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsLockOn")), false);


	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName(TEXT("IsLockOn"))))
	{
		FRotator ToTarget = UKismetMathLibrary::FindLookAtRotation(Boss->GetActorLocation(), Target->GetActorLocation());
		//FRotator LookAtRotation = FRotator(0.f, ToTarget.Yaw, 0.f);
		FRotator LookAtRotation = FMath::RInterpTo(Boss->GetActorRotation(), ToTarget, GetWorld()->DeltaTimeSeconds, 2.f);
		Boss->SetActorRotation(LookAtRotation);

		return true;
	}

	return true;
}