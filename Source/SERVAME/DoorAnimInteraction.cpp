// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorAnimInteraction.h"


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
	BossRoomDoorOpenSequncePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), BossRoomDoorOpenSequence
		, PlaybackSettings, LevelSequenceActor);

	CineCameraActor = Cast<ACineCameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACineCameraActor::StaticClass()));

	BossRoomDoorOpenSequncePlayer->OnFinished.AddDynamic(this, &ADoorAnimInteraction::EndSequence);
	CloseDoorComp->OnComponentBeginOverlap.AddDynamic(this, &ADoorAnimInteraction::OnCloseDoorOverlapBegin);
	OpenDoorComp->OnComponentEndOverlap.AddDynamic(this, &ADoorAnimInteraction::OnOpenDoorOverlapEnd);
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
}

void ADoorAnimInteraction::EndSequence()
{
	GetWorld()->GetFirstPlayerController()->EnableInput(GetWorld()->GetFirstPlayerController());
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(Character, 100.0f);
}

void ADoorAnimInteraction::OnOpenDoorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AnimInstance->DoorAnimationType = DoorAnimationType::KEEPOPEN;
	CloseDoorComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}


void ADoorAnimInteraction::EnableEvent()
{
	Super::EnableEvent();
	SetActorTickEnabled(false);
	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[26].ObjClass, GetActorLocation(), GetActorForwardVector().Rotation() + FRotator(0, 0, -20));
	AnimInstance->DoorAnimationType = DoorAnimationType::OPEN;
	//Character->SetActorLocation(TriggerComp->GetComponentLocation());
	Character->SetActorLocation(FVector(0, 0, 0));
	Character->YawRotation = FRotator::ZeroRotator;
	Character->YawRotation = FRotator::ZeroRotator;
	Character->CameraShake(Character->PlayerDoorCameraShake);
	Character->WeaponMesh->SetVisibility(false);
	Character->ChangeActionType(ActionType::INTERACTION);
	Character->Imotal = true;
	Character->AxisX = 1;
	Character->AxisY = 1;
	GetWorld()->GetFirstPlayerController()->DisableInput(GetWorld()->GetFirstPlayerController());
	Character->ChangeMontageAnimation(AnimationType::DOOROPEN);
	TriggerComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BossRoomDoorOpenSequncePlayer->Play();
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(CineCameraActor, 6.0f);

	
	for (int i = 0; i < ParticleArray.Num(); i++)
	{
		ParticleArray[i]->SetActive(true, false);
	}
}