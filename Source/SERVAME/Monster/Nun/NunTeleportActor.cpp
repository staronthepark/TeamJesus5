// Fill out your copyright notice in the Description page of Project Settings.

#include "NunMonster.h"
#include "NunTeleportActor.h"

// Sets default values
ANunTeleportActor::ANunTeleportActor()
{
	LocationActor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportLocation"));
	LocationActor->SetupAttachment(RootComponent);

	SphereComponent=CreateDefaultSubobject<USphereComponent>(TEXT("CheckCollsion"));
	SphereComponent->SetupAttachment(LocationActor);
}

// Called when the game starts or when spawned
void ANunTeleportActor::BeginPlay()
{
	Super::BeginPlay();

	Nun = Cast<ANunMonster>(UGameplayStatics::GetActorOfClass(GetWorld(), ANunMonster::StaticClass()));
	if (Nun != nullptr)
		Nun->TeleportArr.Push(this);

	LocationActor->SetVisibility(false);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ANunTeleportActor::CheckBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ANunTeleportActor::CheckEndOverlap);
}

// Called every frame
void ANunTeleportActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANunTeleportActor::CheckBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//LOG(Warning, TEXT("IsOverlaped = true"));
	IsOverlaped = true;
}

void ANunTeleportActor::CheckEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//LOG(Warning, TEXT("IsOverlaped = false"));
	IsOverlaped = false;
}

