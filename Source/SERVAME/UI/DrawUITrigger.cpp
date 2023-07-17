#include "DrawUITrigger.h"
#include "Kismet/GameplayStatics.h"

void ADrawUITrigger::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

}

void ADrawUITrigger::BeginTriggerEvent()
{
	Super::BeginTriggerEvent();
}

void ADrawUITrigger::DrawTimeEnd()
{
}

void ADrawUITrigger::PauseGame()
{
	if (TriggerOnlyOnce)
		TriggerComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADrawUITrigger::EndTriggerEvent()
{
	Super::EndTriggerEvent();
	if (DrawImageType == EGuides::keyguide)
		PlayerCharacter->PlayerHUD->PlayAnimations(DrawImageType, false);
}

void ADrawUITrigger::EnableEvent()
{
	Super::EnableEvent();

	PlayerCharacter->PlayerHUD->PlayAnimations(DrawImageType, true);

	if(DrawImageType != EGuides::keyguide)
	GetWorldTimerManager().SetTimer(UITimerHandle, this, &ADrawUITrigger::PauseGame, 0.1f);

}