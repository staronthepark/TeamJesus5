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


	FVector TargetDirection = TargetLocation - OwnerLocation;
	FRotator Difference = TargetDirection.Rotation();


	Owner->SetActorRotation(FMath::Lerp(Owner->GetActorRotation(), Difference, DeltaTime * 6.0f));
	Owner->AddActorWorldOffset(Owner->GetActorRotation().Vector() * DeltaTime * 600.0f);
}

void UMoveToLocationComp::SetTargetLocation(AActor* actor)
{
	TargetActor = actor;
}