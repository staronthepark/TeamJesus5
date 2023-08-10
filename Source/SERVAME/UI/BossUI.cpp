// Fill out your copyright notice in the Description page of Project Settings.


#include "BossUI.h"
#include <Kismet/GameplayStatics.h>


void UBossUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	EndDelegate.BindDynamic(this, &UBossUI::OnAnimationEnd);

	BindToAnimationFinished(BossDiedAnimation, EndDelegate);
}

void UBossUI::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(BossHPOpenAnimation);
}

void UBossUI::SetHP(float value)
{
	value = FMath::Clamp(value, 0.0f, 1.0f);

	Boss_HP->SetPercent(value);
	Boss_White_P->SetValue(value);
	Boss_HP_Y->SetPercent(value);
}

void UBossUI::DecreaseHPGradual(AActor* Actor, float value)
{
	value = FMath::Clamp(value, 0.0f, 1.0f);

	Boss_HP->SetPercent(value);
	Boss_White_P->SetValue(value);
	ProgressDecrease(Actor, Boss_HP_Y, value, 1, HpDelayTimerHandle, HpProgressTimerHandle);

}

void UBossUI::PlayBossHPOpenAnimation(bool IsOpen)
{
	PlayAnimation(BossHPOpenAnimation, IsOpen ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
}

void UBossUI::PlayBossDiedAnimtion()
{
	PlayAnimation(BossDiedAnimation);
}

void UBossUI::SetDamageText(float value)
{
	CumulateDamage += value;
	TakeDamageText->SetText(FText::AsNumber(CumulateDamage));

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(DamageTimerHandle))
		TimerManager.ClearTimer(DamageTimerHandle);

	TimerManager.SetTimer(DamageTimerHandle, [&]() {
		TakeDamageText->SetText(FText::FromString(""));
		CumulateDamage = 0;
		}, 1.0f, false);
}

void UBossUI::OnAnimationEnd()
{
	//EndingCreditImage->SetVisibility(ESlateVisibility::Visible);
	//MediaPlayer->OpenSource(MediaSource);
	//MediaPlayer->Play();

	//FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	//FTimerHandle OpenLevelTimer;
	//TimerManager.SetTimer(OpenLevelTimer, [&]() {
	UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/00_Maps/01_Art/00_Modeller/InGameMap/Boss2PhaseMap")));
		//}, 20.0f, false);
}

