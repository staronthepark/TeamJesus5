 // Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CircleWalk.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CircleWalk::UBTTask_CircleWalk()
{
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_CircleWalk::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Boss = Cast<AJesusBoss>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("SelfActor"))));
    Player = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
  
    //std::srand(time(0));
    //int RandNum = std::rand();
    //RandomBool = static_cast<bool>(RandNum % 2);

    CircleRange = OriginCircleRange;

    Boss->IsLockOn = true;

    count = 0;

    return EBTNodeResult::InProgress;
}

void UBTTask_CircleWalk::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    if (Player == nullptr)
        Player = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
    else
        DrawCircleAroundLocation(OwnerComp, Player->GetActorLocation(), CircleRange);
        
    if (Boss->CurrentWalkCount >= Boss->RandomWalk)
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsCircleWalk")), false);
        Boss->CurrentWalkCount = 0;
        Boss->RandomWalk = 0;
    }

    if (CircleRange > 600.f)
        CircleRange -= 200.f;
    else
    {
        CircleRange = OriginCircleRange;
        Boss->BossSuperAction = SUPER_ATTACK;
        OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("BossBaseAction")), Boss->BossSuperAction);
    }
    FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
}

void UBTTask_CircleWalk::DrawCircleAroundLocation(UBehaviorTreeComponent& OwnerComp, const FVector& Center, float Radius)
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

        float Distance = FVector::Dist(Point, Boss->GetActorLocation());
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

    // 점들을 이어서 원 그리기
    for (int32 i = 0; i < NumSegments; ++i)
    {
        FVector Start = CirclePoints[i];
        FVector End = CirclePoints[(i + 1) % NumSegments];

        // DrawDebugLine 함수를 사용하여 빨간색으로 원 그리기
        //if (i == 0)
        //    DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 5.0f);
        //else
        //    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 5.0f);
    }

    Boss->ChangeMontageAnimation(BossAnimationType::RUN_L);

    if (count == NumSegments)
        count = 0;

    OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("CirclePos")), CirclePoints[++count]);

    //if (RandomBool)
    //{
    //    Boss->ChangeAnimType(BossAnimationType::RUN_L);

    //    if (count == NumSegments)
    //        count = 0;

    //    OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("CirclePos")), CirclePoints[count++]);
    //}
    //else
    //{
    //    Boss->ChangeAnimType(BossAnimationType::RUN_R);

    //    if (reverseCount == 0)
    //        reverseCount = NumSegments - 1;
    //
    //    OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("CirclePos")), CirclePoints[reverseCount--]);
    //}

    Boss->ChangeSuperAction = false;
    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
