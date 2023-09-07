#include "FlowerActor.h"

AFlowerActor::AFlowerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	ParentComp = CreateDefaultSubobject<USceneComponent>("SceneComp");
	TriggerComp = CreateDefaultSubobject<UBoxComponent>("Trigger");
	ChangeMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	OriginMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh2");
	NiagaraComp1 = CreateDefaultSubobject<UNiagaraComponent>("Niagara Comp");

	TriggerComp->SetupAttachment(ParentComp);
	NiagaraComp1->SetupAttachment(ParentComp);
	ChangeMeshComp->SetupAttachment(ParentComp);
	OriginMeshComp->SetupAttachment(ParentComp);
}

void AFlowerActor::BeginPlay()
{
	Super::BeginPlay();

	NiagaraComp1->SetActive(false);
	ChangeMeshComp->SetVisibility(false);

	TriggerComp->OnComponentBeginOverlap.AddDynamic(this, &AFlowerActor::OverlapBegin);
}

void AFlowerActor::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TriggerComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	NiagaraComp1->SetActive(true);
	ChangeMeshComp->SetVisibility(true);
	OriginMeshComp->SetVisibility(false);
}