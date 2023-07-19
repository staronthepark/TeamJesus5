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
	UE_LOG(LogTemp, Warning, TEXT("#@!#!@#!@#"));
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

	if (TriggerOnlyOnce)
		TriggerComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADrawUITrigger::EnableEvent()
{
	Super::EnableEvent();

	UE_LOG(LogTemp, Warning, TEXT("#@!#!@#!@#"));
	PlayerCharacter->PlayerHUD->PlayAnimations(DrawImageType, true);

	if(DrawImageType != EGuides::keyguide && DrawImageType != EGuides::map)
	GetWorldTimerManager().SetTimer(UITimerHandle, this, &ADrawUITrigger::PauseGame, 0.1f);

}