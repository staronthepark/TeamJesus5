// Fill out your copyright notice in the Description page of Project Settings.


#include "BossUI.h"
#include <Kismet/GameplayStatics.h>
#include <SERVAME/Manager/JesusGameInstance.h>


void UBossUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	EndDelegate.BindDynamic(this, &UBossUI::OnAnimationEnd);

	BindToAnimationFinished(FadeInAnimation, EndDelegate);
	BindToAnimationFinished(GameClearAnimation, GameClearDelegate);
}

void UBossUI::NativeConstruct()
{
	Super::NativeConstruct();
	CurrentBoss = EBossSettings::phase1;
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

void UBossUI::PlayBossHPOpenAnimation(bool IsOpen, EBossSettings Boss)
{
	CurrentBoss = Boss;
	ChangeLanguage();
	PlayAnimation(BossHPOpenAnimation, IsOpen ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
}

void UBossUI::PlayBossDiedAnimtion()
{
	PlayFadeInAnimation();
}

void UBossUI::PlayFadeInAnimation()
{
	PlayAnimation(FadeInAnimation);
}

void UBossUI::PlayFadeOutAnimation()
{
	PlayAnimation(FadeOutAnimation);
}

void UBossUI::PlayGameClearAnimation()
{
	PlayAnimation(GameClearAnimation);
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
	UE_LOG(LogTemp, Warning, TEXT("BossDied, Delegate Proceed after FadeOut Ends"));
	//EndingCreditImage->SetVisibility(ESlateVisibility::Visible);
	//MediaPlayer->OpenSource(MediaSource);
	//MediaPlayer->Play();

	//FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	//FTimerHandle OpenLevelTimer;
	//TimerManager.SetTimer(OpenLevelTimer, [&]() {
	//UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/00_Maps/01_Art/00_Modeller/KimMinYeongMap_Boss2")));
		//}, 20.0f, false);
}

void UBossUI::ChangeLanguage()
{
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance->language == Language::ENG)
	{
		BossNameText->SetBrushFromTexture(BossNameTextures.Find(CurrentBoss)->EngTexture, true);
	}
	else if (GameInstance -> language == Language::KOR)
	{
		BossNameText->SetBrushFromTexture(BossNameTextures.Find(CurrentBoss)->KorTexture, true);
	}
}



