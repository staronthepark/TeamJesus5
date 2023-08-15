#include "MoveToLocationComp.h"

UMoveToLocationComp::UMoveToLocationComp()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UMoveToLocationComp::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	SetComponentTickEnabled(false);
}


// Called every frame
void UMoveToLocationComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector OwnerLocation = Owner->GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();


	FVector TargetDirection = TargetLocation + FVector(0, 0, 100) - OwnerLocation;
	FRotator Difference = FRotationMatrix::MakeFromX(TargetDirection).Rotator();

	Owner->SetActorLocation(FMath::Lerp(OwnerLocation, TargetLocation, DeltaTime * MoveSpeed));
	Owner->SetActorRotation(Difference);


	if (FVector::Distance(OwnerLocation, TargetLocation) < 10.0f)
	{
		SetComponentTickEnabled(false);
	}
}

void UMoveToLocationComp::SetTargetLocation(AActor* actor)
{
	TargetActor = actor;
}