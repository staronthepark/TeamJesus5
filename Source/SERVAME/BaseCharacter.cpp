// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	IsPlayer = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	CanHit = true;

	SpawnLocation = GetActorLocation();
	SpawnRotation = GetActorRotation();

	GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));


	TArray<UActorComponent*>ActorCompArray;
	ActorCompArray = GetComponentsByTag(UBoxComponent::StaticClass(), FName("Dust"));


	for (int32 i = ActorCompArray.Num() - 1; i >= 0; i--)
	{
		DustColliderArray.Add(Cast<UBoxComponent>(ActorCompArray[i]));
	}

	for (int32 i = 0; i < DustColliderArray.Num(); i++)
	{
		DustColliderArray[i]->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnDustCollisionBeginOverlap);
	}

	ActorCompArray = GetComponentsByTag(UCapsuleComponent::StaticClass(), FName("Hit"));
	HitCollision = Cast<UCapsuleComponent>(ActorCompArray[0]);
}

void ABaseCharacter::ActivateHitCollision()
{
	CanHit = true;
}

void ABaseCharacter::DeactivateHitCollision()
{
	CanHit = false;
}

void ABaseCharacter::RespawnCharacter()
{
	SetActorLocation(SpawnLocation);
	SetActorRotation(SpawnRotation);	
	SetActive(true);
}

void ABaseCharacter::CameraShake(TSubclassOf< UCameraShakeBase> Shake)
{
	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(Shake, 1.0f);
}

void ABaseCharacter::VibrateGamePad(float Intensity, float time)
{
	GetWorld()->GetFirstPlayerController()->PlayDynamicForceFeedback(Intensity, time, true, true, true, true, EDynamicForceFeedbackAction::Start);
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!CanHit)
		return false;

	int value = FMath::RandRange(16, 17);
	AObjectPool& objectpool = AObjectPool::GetInstance();
	objectpool.SpawnObject(objectpool.ObjectArray[value].ObjClass, GetActorLocation(), FRotator::ZeroRotator);
	objectpool.SpawnObject(objectpool.ObjectArray[18].ObjClass, GetActorLocation(), FRotator::ZeroRotator);
	objectpool.SpawnObject(objectpool.ObjectArray[19].ObjClass, GetActorLocation(), FRotator::ZeroRotator);
	HitStop();
	return DamageAmount;
}

void ABaseCharacter::OnDustCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	//objectpool.SpawnObject(objectpool.ObjectArray[2].ObjClass, OverlappedComponent->GetComponentTransform().GetLocation() - FVector(0, 0, 5.0f), FRotator::ZeroRotator);
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABaseCharacter::SetActive(bool active)
{
	SetActorHiddenInGame(!active);
	SetActorEnableCollision(active);
	SetActorTickEnabled(active);
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	fDeltaTime = DeltaTime;	
}

void ABaseCharacter::HitStopTimer()
{
	GetWorldTimerManager().SetTimer(TimerHandler, this, &ABaseCharacter::ResumeMontage, 0.2f);
}

void ABaseCharacter::SwordVFXSpawn()
{
 	AObjectPool& objectpool = AObjectPool::GetInstance();
	objectpool.SpawnObject(objectpool.ObjectArray[12].ObjClass, WeaponOverlapStaticMeshCollision->GetComponentLocation(), FRotator(90, 0, 0));
	GetWorldTimerManager().SetTimer(SMOverlapTimerHandler, this, &ABaseCharacter::SwordVFXSpawn, 0.02f);
}

void ABaseCharacter::SwordVFXSpawnByLocation(FVector Location)
{
	AObjectPool& objectpool = AObjectPool::GetInstance();
	objectpool.SpawnObject(objectpool.ObjectArray[12].ObjClass, Location, FRotator(90, 0, 0));

	GetWorldTimerManager().SetTimer(SMOverlapTimerHandler, FTimerDelegate::CreateLambda([=]()
		{
			SwordVFXSpawnByLocation(Location);
		}), 0.02f, false);
}
