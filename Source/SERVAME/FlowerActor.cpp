#include "FlowerActor.h"

AFlowerActor::AFlowerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerComp = CreateDefaultSubobject<UBoxComponent>("Trigger");
	ChangeMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	ChangeMeshComp->SetupAttachment(TriggerComp);
	OriginMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh2");
	OriginMeshComp->SetupAttachment(TriggerComp);
}

void AFlowerActor::BeginPlay()
{
	Super::BeginPlay();

	ChangeMeshComp->SetVisibility(false);

	TriggerComp->OnComponentBeginOverlap.AddDynamic(this, &AFlowerActor::OverlapBegin);
}

void AFlowerActor::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TriggerComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ChangeMeshComp->SetVisibility(true);
	OriginMeshComp->SetVisibility(false);
}