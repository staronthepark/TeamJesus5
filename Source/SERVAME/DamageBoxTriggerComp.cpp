#include "DamageBoxTriggerComp.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include ".\Manager\CombatManager.h"
#include ".\ObjectPool\ObjectPool.h"

UDamageBoxTriggerComp::UDamageBoxTriggerComp()
{

}

void UDamageBoxTriggerComp::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UDamageBoxTriggerComp::BeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UDamageBoxTriggerComp::EndOverlap);

	GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	Character = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	IsActiveHit = false;

	Count = 0;
}

void UDamageBoxTriggerComp::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlapActorArray.Add(Cast<ABaseCharacter>(OtherActor));

	if (!IsActiveHit)
	{
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &UDamageBoxTriggerComp::ActiveHit, 0.1f);
		IsActiveHit = true;
	}	
}

void UDamageBoxTriggerComp::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlapActorArray.Remove(Cast<ABaseCharacter>(OtherActor));
}

void UDamageBoxTriggerComp::ActiveHit()
{
	Count++;
	for (int i = 0; i < OverlapActorArray.Num(); i++)
	{
		UCombatManager::GetInstance().HitMonsterInfoArray.AddUnique(OverlapActorArray[i]);
		OverlapActorArray[i]->ActivateHitCollision();
//		OverlapActorArray[i]->TakeDamage(Damage, DamageEvent, nullptr, GetOwner());

		

		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[0].ObjClass, OverlapActorArray[i]->GetActorLocation(), FRotator(90, 180, 180));
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[1].ObjClass, OverlapActorArray[i]->GetActorLocation(), FRotator(90, 180, 180));
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[3].ObjClass, OverlapActorArray[i]->GetActorLocation(), FRotator(90, 180, 180));
		AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[1].ObjClass, OverlapActorArray[i]->GetActorLocation(), FRotator(90, 180, 180));
		
			AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[5].ObjClass, OverlapActorArray[i]->GetActorLocation(), FRotator(90, 180, 180));
	}
	if (Count >= MaxCount)
	{
		Count = 0;
		IsActiveHit = false;

		return;
	}
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UDamageBoxTriggerComp::ActiveHit, DamageTime);
}