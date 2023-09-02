// Fill out your copyright notice in the Description page of Project Settings.


#include "NunTeleportActor.h"

// Sets default values
ANunTeleportActor::ANunTeleportActor()
{
	LocationActor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportLocation"));
	LocationActor->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ANunTeleportActor::BeginPlay()
{
	Super::BeginPlay();

	Nun = Cast<ANunMonster>(UGameplayStatics::GetActorOfClass(GetWorld(), ANunMonster::StaticClass()));
	if (Nun != nullptr)
		Nun->TeleportArr.Push(this);

	LocationActor->SetVisibility(false);
}

// Called every frame
void ANunTeleportActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

