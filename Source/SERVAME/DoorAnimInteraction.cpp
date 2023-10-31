// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorAnimInteraction.h"


void ADoorAnimInteraction::Init()
{
	AnimInstance->DoorAnimationType = DoorAnimType;
	CloseDoorComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}

void ADoorAnimInteraction::BeginPlay()
{
	Super::BeginPlay();

	TArray<UActorComponent*> ActorCompArray;

	ActorCompArray = GetComponentsByClass(UParticleSystemComponent::StaticClass());
	
	for (int i = 0; i < ActorCompArray.Num(); i++)
	{
		ParticleArray.Add(Cast<UParticleSystemComponent>(ActorCompArray[i]));
		ParticleArray[i]->SetActive(false, false);
	}
	AnimInstance = Cast<UDoorAnimInstance>(MeshComp->GetAnimInstance());

	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	if (BossRoomDoorOpenSequence != nullptr)
	{
		BossRoomDoorOpenSequncePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), BossRoomDoorOpenSequence
			, PlaybackSettings, LevelSequenceActor);

		BossRoomDoorOpenSequncePlayer->OnFinished.AddDynamic(this, &ADoorAnimInteraction::EndSequence);
	}

	CineCameraActor = Cast<ACineCameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACineCameraActor::StaticClass()));


	CloseDoorComp->OnComponentBeginOverlap.AddDynamic(this, &ADoorAnimInteraction::OnCloseDoorOverlapBegin);
	OpenDoorComp->OnComponentEndOverlap.AddDynamic(this, &ADoorAnimInteraction::OnOpenDoorOverlapEnd);
	Init();

	BoxComp = Cast<UBoxTriggerComp>(GetComponentByClass(UBoxTriggerComp::StaticClass()));

	if (DisableTriggerWhenStart)
		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}

void ADoorAnimInteraction::BeginTriggerEvent()
{
	Super::BeginTriggerEvent();
	Character->PlayerHUD->PlayInteractionAnimation(true, EInteractions::open);
}

void ADoorAnimInteraction::EndTriggerEvent()
{
	Super::EndTriggerEvent();
	Character->PlayerHUD->PlayInteractionAnimation(false);
}


void ADoorAnimInteraction::OnCloseDoorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character->PlayerHUD->PlayInteractionAnimation(false);
	AnimInstance->DoorAnimationType = DoorAnimationType::CLOSE;
	CloseDoorComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OpenDoorComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ADoorAnimInteraction::EndSequence()
{
	GetWorld()->GetFirstPlayerController()->EnableInput(GetWorld()->GetFirstPlayerController());
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(Character, 100.0f);
}

void ADoorAnimInteraction::OnOpenDoorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AnimInstance->DoorAnimationType = DoorAnimationType::CLOSE;
	CloseDoorComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}


void ADoorAnimInteraction::EnableEvent()
{
	Super::EnableEvent();
	SetActorTickEnabled(false);
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[26].ObjClass, GetActorLocation(), GetActorForwardVector().Rotation() + FRotator(0, 0, -20));
	AnimInstance->DoorAnimationType = DoorAnimationType::OPEN;

	if (PlayerPlayDoorOpenAnim)
	{
		Character->SetActorLocation(TriggerComp->GetComponentLocation());
		Character->SetActorRotation(GetActorRotation() - FRotator(0, -90, 0));
		Character->YawRotation = GetActorRotation() - FRotator(0, -90, 0);
		Character->CameraShake(Character->PlayerDoorCameraShake);
		Character->WeaponMesh->SetVisibility(false);
		Character->ChangeActionType(ActionType::INTERACTION);
		Character->Imotal = true;
		Character->AxisX = 1;
		Character->AxisY = 1;
		Character->ChangeMontageAnimation(AnimationType::DOOROPEN);
		Character->ChangeActionType(ActionType::NONE);
		Character->ChangePlayerAction(PlayerAction::CANATTACK);
	}
	TriggerComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CloseDoorComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	if (BossRoomDoorOpenSequence != nullptr)
	{
		BossRoomDoorOpenSequncePlayer->Play();
		GetWorld()->GetFirstPlayerController()->DisableInput(GetWorld()->GetFirstPlayerController());
		GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(CineCameraActor, 6.0f);
		Character->WeaponMesh->SetVisibility(true);
	}

	IsActive = true;
	
	for (int i = 0; i < ParticleArray.Num(); i++)
	{
		ParticleArray[i]->SetActive(true, false);
	}
}