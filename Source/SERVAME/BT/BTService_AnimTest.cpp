// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_AnimTest.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_AnimTest::UBTService_AnimTest()
{
	NodeName = TEXT("AnimTest");

	TestAnimMap.Add(BossAnimationType::GROGGYIDLE, TMap<bool, TFunction<void()>>());
	TestAnimMap[BossAnimationType::GROGGYIDLE].Add(true, [=]()
		{
			TestMontage = Boss->BossMontageMap[BossAnimationType::GROGGYIDLE];
			Boss->ChangeMontageAnimation(BossAnimationType::GROGGYIDLE);
			IsPlayed = true;
		});
	TestAnimMap[BossAnimationType::GROGGYIDLE].Add(false, [=]()
		{
			IsPlayed = false;
			return;
		});
}

void UBTService_AnimTest::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (Boss == nullptr)
		Boss = Cast<AJesusBoss>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("SelfActor"))));

	if (!IsPlayed && TestAnimMap.Contains(TestAnimType))
	{
		TestAnimMap[TestAnimType][IsPlay]();
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsAnimTest", IsPlay);
	}
}
