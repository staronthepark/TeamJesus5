#include "BaseTriggerActor.h"

ABaseTriggerActor::ABaseTriggerActor()
{
	PrimaryActorTick.bCanEverTick = true;
	IsActive = false;
}

void ABaseTriggerActor::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerComp = Cast<UBoxTriggerComp>(GetComponentByClass(UBoxTriggerComp::StaticClass()));
}

void ABaseTriggerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseTriggerActor::BeginTriggerEvent()
{
	IsOverlap = true;
	if (AutoActive)
		EnableEvent();
}

void ABaseTriggerActor::EndTriggerEvent()
{
	IsOverlap = false;
}

void ABaseTriggerActor::EnableEvent()
{
	IsActive = true;
}