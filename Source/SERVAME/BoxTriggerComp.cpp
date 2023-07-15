#include "BoxTriggerComp.h"
#include "BaseTriggerActor.h"
#include ".\Player\PlayerCharacter.h"

UBoxTriggerComp::UBoxTriggerComp()
{

}

void UBoxTriggerComp::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UBoxTriggerComp::BeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UBoxTriggerComp::EndOverlap);

	Owner = Cast<ABaseTriggerActor>(GetOwner());
}

void UBoxTriggerComp::BeginEvent()
{
	Owner->BeginTriggerEvent();

}

void UBoxTriggerComp::EndEvent()
{
	Owner->EndTriggerEvent();

}

void UBoxTriggerComp::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* character = Cast<APlayerCharacter>(OtherActor);
	if(character != nullptr)
	BeginEvent();
}

void UBoxTriggerComp::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* character = Cast<APlayerCharacter>(OtherActor);
	if (character != nullptr)
	EndEvent();
}