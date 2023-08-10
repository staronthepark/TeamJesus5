#include "MoveToLocationComp.h"
#include "Kismet/GameplayStatics.h"

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
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}


// Called every frame
void UMoveToLocationComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector OwnerLocation = Owner->GetActorLocation();
	Owner->SetActorLocation(FMath::Lerp(OwnerLocation, Player->GetActorLocation(), DeltaTime * MoveSpeed));
	if (FVector::Distance(OwnerLocation, Player->GetActorLocation()) < 10.0f)
	{
		SetComponentTickEnabled(false);
	}
}


void UMoveToLocationComp::SetTargetLocation(FVector Location)
{
	TargetLocation = Location;
}