#include "BaseDoor.h"
#include "GameFramework/Character.h"

ABaseDoor::ABaseDoor()
{
	PrimaryActorTick.bCanEverTick = true;

}
void ABaseDoor::BeginPlay()
{
	Super::BeginPlay();

	UActorComponent* ActorComp;
	ActorComp = GetComponentByClass(UBoxComponent::StaticClass());
	
	BoxComp = Cast<UBoxComponent>(ActorComp);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ABaseDoor::OnCollisionBeginOverlap);

//	AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
}

void ABaseDoor::Tick(float DeltaTime)
{
	if (IsCollision && !IsOpen)
	{
		AnimInstance->PlayMontage(montage);
		IsOpen = true;
	}
}

void ABaseDoor::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* character = Cast<APlayerCharacter>(OtherActor);

	if (character)
	{
		IsCollision = true;
	}
}