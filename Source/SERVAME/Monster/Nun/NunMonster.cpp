// Fill out your copyright notice in the Description page of Project Settings.


#include "NunMonster.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Math/RandomStream.h"
#include "..\..\Manager\CombatManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NunAttackTriggerComp.h"

ANunMonster::ANunMonster()
{
	AttackTrigger = CreateDefaultSubobject<UNunAttackTriggerComp>(TEXT("AttackTriggerCollision"));
	AttackTrigger->SetupAttachment(GetMesh());

}

void ANunMonster::BeginPlay()
{

}

void ANunMonster::Tick(float DeltaTime)
{

}

void ANunMonster::OnNunTargetDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ANunMonster::OnNunTargetDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ANunMonster::StartAttackTrigger(MonsterAnimationType AttackAnimType)
{
}

void ANunMonster::EndAttackTrigger(MonsterAnimationType AttackAnimType)
{
}

float ANunMonster::Die(float Dm)
{
	return 0.0f;
}

void ANunMonster::Stun()
{
}

void ANunMonster::MonsterHitStop()
{
}

void ANunMonster::ChangeMontageAnimation(MonsterAnimationType type)
{
}

float ANunMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

void ANunMonster::CheckMontageEndNotify()
{
}

void ANunMonster::PlayExecutionAnimation()
{
}

void ANunMonster::IsNotifyActive(bool value)
{
}

void ANunMonster::RespawnCharacter()
{
}

void ANunMonster::ResumeMontage()
{
}
