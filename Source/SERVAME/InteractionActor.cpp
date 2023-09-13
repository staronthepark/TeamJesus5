#include "InteractionActor.h"
#include "Kismet/GameplayStatics.h"
#include "./UI/PlayerHUD.h"

AInteractionActor::AInteractionActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInteractionActor::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	SetActorTickEnabled(false);
	TickFuncMap.Add(true, TMap<bool, TFunction<void(AInteractionActor * actor)>>());
	TickFuncMap[true].Add(true, [](AInteractionActor* actor)
		{
			actor->EnableEvent();
			actor->Character->IsInteraction = false;
		});
	TickFuncMap[true].Add(false, [](AInteractionActor* actor) {});
	TickFuncMap.Add(false, TMap<bool, TFunction<void(AInteractionActor * actor)>>());
	TickFuncMap[false].Add(true, [](AInteractionActor* actor) {});
	TickFuncMap[false].Add(false, [](AInteractionActor* actor) {});
	Init();
}

void AInteractionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickFuncMap[IsOverlap][Character->IsInteraction](this);
}

void AInteractionActor::BeginTriggerEvent()
{
	Super::BeginTriggerEvent();
	//Character->PlayerHUD->PlayInteractionAnimation(true);
	SetActorTickEnabled(true);
}

void AInteractionActor::EndTriggerEvent()
{
	Super::EndTriggerEvent();
	//Character->PlayerHUD->PlayInteractionAnimation(false);
}

void AInteractionActor::EnableEvent()
{
	Super::EnableEvent();
	//Character->PlayerHUD->PlayInteractionAnimation(false);
}

void AInteractionActor::Init()
{
}