// Fill out your copyright notice in the Description page of Project Settings.

#include "./Player/JesusPlayerController.h"
#include "PlaySequenceTriggerActor.h"
#include "./Manager/SoundManager.h"

APlaySequenceTriggerActor::APlaySequenceTriggerActor()
{

}

void APlaySequenceTriggerActor::BeginPlay()
{
	Super::BeginPlay();

	Boss = Cast<AJesusBoss>(UGameplayStatics::GetActorOfClass(GetWorld(), AJesusBoss::StaticClass()));
	
	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	SequncePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Sequence
		, PlaybackSettings, SequenceActor);

	SequncePlayer->OnFinished.AddDynamic(this, &APlaySequenceTriggerActor::EndSequence);
	Character = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	TArray<AActor*>asdarray;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACineCameraActor::StaticClass(), FName("OpenDoor"), asdarray);
	if (asdarray.Num() > 0)
		CineCameraActor = Cast<ACineCameraActor>(asdarray[0]);
}

void APlaySequenceTriggerActor::BeginTriggerEvent()
{
	Super::BeginTriggerEvent();
}

void APlaySequenceTriggerActor::EndTriggerEvent()
{
	Super::EndTriggerEvent();
}

void APlaySequenceTriggerActor::EnableEvent()
{
	Super::EnableEvent();
	if (IsActive)return;
	//Boss->BossAnimInstance->StopAllMontages(0.f);

	SequncePlayer->Play();
	
	Character->AxisX = 1;
	Character->AxisY = 1;
	Character->PlayerHUD->SetVisibility(ESlateVisibility::Collapsed);


	Character->AnimInstance->StopAllMontages(0.0f);
	Character->ChangeActionType(ActionType::NONE);
	Character->ChangePlayerAction(PlayerAction::CANTACT);

	//Character->SetActorHiddenInGame(true);

	AJesusPlayerController* controller = Cast<AJesusPlayerController>(GetWorld()->GetFirstPlayerController());

	controller->CurrentSequncePlayer = SequncePlayer;
	controller->DisableInput(GetWorld()->GetFirstPlayerController());

	ASoundManager::GetInstance().PauseBGM();

	if (TriggerOnlyOnce)
		TriggerComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlaySequenceTriggerActor::EndSequence()
{
	//Boss->BossAnimInstance->IsSequenceEnd = true;
	//Boss->BossAnimInstance->ResumeMontage(Boss->GetCurrentMontage());
	GetWorld()->GetFirstPlayerController()->EnableInput(GetWorld()->GetFirstPlayerController());
	GetWorld()->GetFirstPlayerController()->SetViewTarget(Character);
	Character->ChangePlayerAction(PlayerAction::NONE);
	Character->ChangeMontageAnimation(AnimationType::IDLE);
	Character->SetActorHiddenInGame(false);
	Character->PlayerHUD->SetVisibility(ESlateVisibility::HitTestInvisible);
	Character->WeaponMesh->SetVisibility(true);
}
