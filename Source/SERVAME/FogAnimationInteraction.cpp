// Fill out your copyright notice in the Description page of Project Settings.


#include "FogAnimationInteraction.h"

AFogAnimationInteraction::AFogAnimationInteraction()
{
	BoxComp2 = CreateDefaultSubobject<UBoxComponent>("Boxcomp2");
}


void AFogAnimationInteraction::BeginPlay()
{
	Super::BeginPlay();


	TArray<UActorComponent*> array = GetComponentsByClass(UStaticMeshComponent::StaticClass());

	array = GetComponentsByTag(UBoxComponent::StaticClass(), "FogCollision");

	BoxCompArray.Add(Cast<UBoxComponent>(array[0]));
	BoxCompArray.Add(Cast<UBoxComponent>(array[1]));

	BoxCompArray[0]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCompArray[1]->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BoxComp2->OnComponentBeginOverlap.AddDynamic(this, &AFogAnimationInteraction::OnBeginOverlap);
}

void AFogAnimationInteraction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFogAnimationInteraction::BeginTriggerEvent()
{
	Super::BeginTriggerEvent();	
}

void AFogAnimationInteraction::EndTriggerEvent()
{
	Super::EndTriggerEvent();
}

void AFogAnimationInteraction::EnableEvent()
{
	Super::EnableEvent();
	Character->SetActorLocation(FVector(-4507.492188, -31.434, 68.2));
	Character->YawRotation = FRotator::ZeroRotator;
	Character->AxisX = 1;
	Character->AxisY = 1;
	Character->ChangeActionType(ActionType::INTERACTION);
	Character->ChangeMontageAnimation(AnimationType::ENTERTHEFOG);
	BoxCompArray[0]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCompArray[1]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFogAnimationInteraction::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	BoxCompArray[0]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCompArray[1]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}