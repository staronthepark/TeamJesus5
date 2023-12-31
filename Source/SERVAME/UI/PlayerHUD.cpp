#include "PlayerHUD.h"
#include "Components/CanvasPanelSlot.h"
#include "SkillUI.h"
#include "PlayerSoulStatUI.h"
#include "SubtitleUI.h"
#include <SERVAME/Player/JesusPlayerController.h>
// Fill out your copyright notice in the Description page of Project Settings.



void UPlayerHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	GuideAnimationFunction.Add(EGuides::keyguide, [&](bool value)
		{
			PlayKeyGuideAnimation(value);
		});
	GuideAnimationFunction.Add(EGuides::dodge, [&](bool value)
		{
			PlayGuidesAnimation(EGuides::dodge, value);
		});
	GuideAnimationFunction.Add(EGuides::camera, [&](bool value)
		{
			PlayGuidesAnimation(EGuides::camera, value);
		});
	GuideAnimationFunction.Add(EGuides::grogy, [&](bool value)
		{
			PlayGuidesAnimation(EGuides::grogy, value);
		});	
	GuideAnimationFunction.Add(EGuides::parrying, [&](bool value)
		{
			PlayGuidesAnimation(EGuides::parrying, value);
		});	
	GuideAnimationFunction.Add(EGuides::savepoint, [&](bool value)
		{
			PlayGuidesAnimation(EGuides::savepoint, value);
		});
	GuideAnimationFunction.Add(EGuides::Haros, [&](bool value)
		{
			currentLanguage = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->language;
			PlayMapUIAnimation(EGuides::Haros);
		});
	GuideAnimationFunction.Add(EGuides::LostWay, [&](bool value)
		{
			currentLanguage = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->language;
			PlayMapUIAnimation(EGuides::LostWay);
		});
	GuideAnimationFunction.Add(EGuides::HouseOfAbandoned, [&](bool value)
		{
			currentLanguage = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->language;
			PlayMapUIAnimation(EGuides::HouseOfAbandoned);
		});
	GuideAnimationFunction.Add(EGuides::MainHall, [&](bool value)
		{
			currentLanguage = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->language;
			PlayMapUIAnimation(EGuides::MainHall);
		});
	GuideAnimationFunction.Add(EGuides::Repent, [&](bool value)
		{
			currentLanguage = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->language;
			PlayMapUIAnimation(EGuides::Repent);
		});
	//GuideAnimationFunction.Add(EGuides::shield, [&](bool value)
	//	{
	//		PlayGuidesAnimation(EGuides::shield, value);
	//	});
	GuideAnimationFunction.Add(EGuides::shieldattack, [&](bool value)
		{
			PlayGuidesAnimation(EGuides::shieldattack, value);
		});
	GuideAnimationFunction.Add(EGuides::soul, [&](bool value)
		{
			PlayGuidesAnimation(EGuides::soul, value);
		});

	GuideAnimationFunction.Add(EGuides::LockOn, [&](bool value)
		{
			PlayTutorialAnimation(EGuides::LockOn, isGamePad);
		});
	GuideAnimationFunction.Add(EGuides::Parring, [&](bool value)
		{
			PlayTutorialAnimation(EGuides::Parring, isGamePad);
		});
	GuideAnimationFunction.Add(EGuides::Heal, [&](bool value)
		{
			PlayTutorialAnimation(EGuides::Heal, isGamePad);
		});
	GuideAnimationFunction.Add(EGuides::ShieldBash, [&](bool value)
		{
			PlayTutorialAnimation(EGuides::ShieldBash, isGamePad);
		});
	GuideAnimationFunction.Add(EGuides::Shield, [&](bool value)
		{
			PlayTutorialAnimation(EGuides::Shield, isGamePad);
		});
	GuideAnimationFunction.Add(EGuides::StrongAttack, [&](bool value)
		{
			PlayTutorialAnimation(EGuides::StrongAttack, isGamePad);
		});
	GuideAnimationFunction.Add(EGuides::Skill, [&](bool value)
		{
			PlayTutorialAnimation(EGuides::Skill, isGamePad);
		});

	// Subtitle
	GuideAnimationFunction.Add(EGuides::ST_Garden, [&](bool value)
		{
			PlaySubtitleAnimation(EGuides::ST_Garden);
		});
	GuideAnimationFunction.Add(EGuides::ST_MainHall, [&](bool value)
		{
			PlaySubtitleAnimation(EGuides::ST_MainHall);
		});
	GuideAnimationFunction.Add(EGuides::ST_BossHall, [&](bool value)
		{
			PlaySubtitleAnimation(EGuides::ST_BossHall);
		});
	GuideAnimationFunction.Add(EGuides::ST_Tomb, [&](bool value)
		{
			PlaySubtitleAnimation(EGuides::ST_Tomb);
		});
	GuideAnimationFunction.Add(EGuides::ST_PrayRoom, [&](bool value)
		{
			PlaySubtitleAnimation(EGuides::ST_PrayRoom);
		});
	GuideAnimationFunction.Add(EGuides::ST_Jamsic, [&](bool value)
		{
			PlaySubtitleAnimation(EGuides::ST_Jamsic);
		});
	GuideAnimationFunction.Add(EGuides::ST_Elgi, [&](bool value)
		{
			PlaySubtitleAnimation(EGuides::ST_Elgi);
		});
	GuideAnimationFunction.Add(EGuides::ST_Nun1, [&](bool value)
		{
			PlaySubtitleAnimation(EGuides::ST_Nun1);
		});
	GuideAnimationFunction.Add(EGuides::ST_Nun3, [&](bool value)
		{
			PlaySubtitleAnimation(EGuides::ST_Nun3);
		});
	GuideAnimationFunction.Add(EGuides::ST_Phase1_1, [&](bool value)
		{
			PlaySubtitleAnimation(EGuides::ST_Phase1_1);
		});
	GuideAnimationFunction.Add(EGuides::ST_Phase1_3, [&](bool value)
		{
			PlaySubtitleAnimation(EGuides::ST_Phase1_3);
		});
	GuideAnimationFunction.Add(EGuides::ST_Phase1_4, [&](bool value)
		{
			PlaySubtitleAnimation(EGuides::ST_Phase1_4);
		});
	GuideAnimationFunction.Add(EGuides::ST_Phase2_1, [&](bool value)
		{
			PlaySubtitleAnimation(EGuides::ST_Phase2_1);
		});
	GuideAnimationFunction.Add(EGuides::ST_Phase2_2, [&](bool value)
		{
			PlaySubtitleAnimation(EGuides::ST_Phase2_2);
		});

	GuideAnimationFunction.Add(EGuides::SoulGuide, [&](bool value)
		{
			WBP_PlayerGuideUI->GuideImage->SetBrushFromTexture(WBP_PlayerGuideUI->GuideTextures.Find(EGuides::SoulGuide)->KorTextures[0], true);
			PlayAnimation(PlayGuideAnimation);
		});
	isGamePad = Keyboard;
}

void UPlayerHUD::ClearShield()
{
	//WBP_Player_ShieldsUI->Clear();
}


void UPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();
	if(this->IsInViewport())
		PlayAnimation(HUDOpenAnimation);
	currentLanguage = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->language;
}

void UPlayerHUD::SetHP(float value)
{
	WBP_PlayerHPUI->SetHP(value);
}

void UPlayerHUD::DecreaseHPGradual(AActor* Actor, float value)
{
	WBP_PlayerHPUI->DecreaseHPGradual(Actor, value);
}

void UPlayerHUD::IncreaseHpSize(float value)
{
	WBP_PlayerHPUI->IncreaseHPSize(value);
}

void UPlayerHUD::SetStamina(float value)
{
	WBP_PlayerHPUI->SetStamina(value);
}

void UPlayerHUD::DecreaseStaminaGradual(AActor* Actor, float value)
{
	WBP_PlayerHPUI->DecreaseStaminaGradual(Actor, value);
}

void UPlayerHUD::IncreaseStaminaSize(float value)
{
	WBP_PlayerHPUI->IncreaseStaminaSize(value);
}

void UPlayerHUD::SetSoul(float value)
{
	WBP_PlayerHPUI->SetSoul(value);
}

void UPlayerHUD::DecreaseSoulGradual(AActor* Actor, float value)
{
	WBP_PlayerHPUI->DecreaseSoulGradual(Actor, value);
}

void UPlayerHUD::IncreaseSoulSize(float value)
{
	WBP_PlayerHPUI->IncreaseSoulSize(value);
}

void UPlayerHUD::SetSkill(float value)
{
	SkillUI->SetSkill(value);
}

void UPlayerHUD::SetGamepad(bool isPad)
{
	if (isPad) 
	{
		WBP_PlayerGuideUI->SetGamePad(Gamepad);
		isGamePad = Gamepad;
	}
	else
	{
		WBP_PlayerGuideUI->SetGamePad(Keyboard);
		isGamePad = Keyboard;
	}
	//if (isPad) {
	//	PlayerControlGuid->SetBrushFromTexture(GuidTextures[Gamepad]);
	//	Interaction_Image->SetBrushFromTexture(InteractionTextures[Gamepad]);
	//	Exit_Image->SetBrushFromTexture(ExitTextures[Gamepad]);
	//	WBP_PlayerGuideUI->SetGamePad(true);
	//}
	//else {
	//	PlayerControlGuid->SetBrushFromTexture(GuidTextures[Keyboard]);
	//	Interaction_Image->SetBrushFromTexture(InteractionTextures[Keyboard]);
	//	Exit_Image->SetBrushFromTexture(ExitTextures[Keyboard]);
	//	WBP_PlayerGuideUI->SetGamePad(false);
	//}
}

void UPlayerHUD::PlayAnimations(EGuides type, bool IsOpen)
{
	GuideAnimationFunction[type](IsOpen);
}

//void UPlayerHUD::SetShield(int value)
//{
//	WBP_Player_ShieldsUI->SetShield(value);
//}


void UPlayerHUD::OpenStat()
{
	WBP_PlayerStat_UI->Open();
}

void UPlayerHUD::InitStat(int str, int stamina, int hp, int shield)
{
	WBP_PlayerStat_UI->InitStat(str, stamina, hp, shield);
}

void UPlayerHUD::SetRemainSoul(int value)
{
	WBP_PlayerStat_UI->SetRemainSoul(value);
}

void UPlayerHUD::PlayGuidesAnimation(EGuides type, bool IsOpen)
{
	currentLanguage = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->language;
	IsOpen ? WBP_PlayerGuideUI->StartGuide(type, currentLanguage) : WBP_PlayerGuideUI->Disable();
}

void UPlayerHUD::PlayKeyGuideAnimation(bool IsOpen)
{
	currentLanguage = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->language;

	if (isGamePad == Gamepad)
	{
		if (currentLanguage == Language::ENG)
			PlayerControlGuid->SetBrushFromTexture(ControlGuideTextures.Find(Gamepad)->EngTextures[0]);
		else if (currentLanguage == Language::KOR)
			PlayerControlGuid->SetBrushFromTexture(ControlGuideTextures.Find(Gamepad)->KorTextures[0]);
	}
	else 
	{
		if (currentLanguage == Language::ENG)
			PlayerControlGuid->SetBrushFromTexture(ControlGuideTextures.Find(Keyboard)->EngTextures[0]);
		else if (currentLanguage == Language::KOR)
			PlayerControlGuid->SetBrushFromTexture(ControlGuideTextures.Find(Keyboard)->KorTextures[0]);
	}

	IsOpen ? PlayAnimation(GuidOpenAnimation) : PlayAnimation(GuidCloseAnimation);
}


void UPlayerHUD::PlayInteractionAnimation(bool IsOpen, EInteractions interactions)
{
	currentLanguage = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->language;
	if(currentLanguage == Language::ENG)
		Interaction_Image->SetBrushFromTexture(InteractionTextures.Find(interactions)->EngTextures[isGamePad]); 
	else if(currentLanguage == Language::KOR)
		Interaction_Image->SetBrushFromTexture(InteractionTextures.Find(interactions)->KorTextures[isGamePad]); 

	if (IsOpen && Interaction_Image->GetRenderOpacity() < 0.1f)
	{
		UE_LOG(LogTemp, Warning, TEXT("OPEN"));
		PlayAnimation(InteractionOpacityAnimation);
	}
	else if (IsOpen)
	{
		return;
	}
	else if (!IsOpen && Interaction_Image->GetRenderOpacity() < 0.1f)
	{
		UE_LOG(LogTemp, Warning, TEXT("CLOSE Pressed But Opacity less"));
		return;
	}
	else
	{

		UE_LOG(LogTemp, Warning, TEXT("CLOSE"));
		PlayAnimation(InteractionOpacityOutAnimation);
	}
	//IsOpen ? PlayAnimation(InteractionOpacityAnimation) : PlayAnimation(InteractionOpacityOutAnimation);
}

void UPlayerHUD::PlayInteractionAnimation(bool IsOpen)
{
	if (IsOpen && Interaction_Image->GetRenderOpacity() < 0.1f)
	{
		UE_LOG(LogTemp, Warning, TEXT("OPEN"));
		PlayAnimation(InteractionOpacityAnimation);
	}
	else if (IsOpen)
	{
		return;
	}
	else if (!IsOpen && Interaction_Image->GetRenderOpacity() < 0.1f)
	{
		UE_LOG(LogTemp, Warning, TEXT("CLOSE Pressed But Opacity less"));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CLOSE"));
		PlayAnimation(InteractionOpacityOutAnimation);
	}
}

void UPlayerHUD::PlayExitAnimation(bool IsOpen)
{
	//IsOpen ? PlayAnimation(ExitOpenAnimation) : PlayAnimation(ExitCloseAnimation);
}

void UPlayerHUD::PlayMapUIAnimation(EGuides MapName)
{
	if (WBP_MapInfo)
		WBP_MapInfo->PlayMapInfoAnimation(MapName);
}

void UPlayerHUD::PlayTutorialAnimation(EGuides type, int isPad)
{
	WBP_Tutorial->ChangeTexture(type, isPad);
	PlayAnimation(TutorialAnimation);
}

void UPlayerHUD::PlaySubtitleAnimation(EGuides type)
{
	WBP_Subtitle->PlaySubtitle(type);
}

void UPlayerHUD::FadeInAnimation(bool isFadeIn)
{
	if (WBP_FadeInOutUI)
		WBP_FadeInOutUI->PlayFadeInOutAnimation(isFadeIn);
}

bool UPlayerHUD::IsRender()
{
	return WBP_PlayerGuideUI->IsRender();
}

void UPlayerHUD::PlayDied(bool IsOpen)
{
	if (WBP_DiedUI)
		WBP_DiedUI->PlayDiedAnimation(IsOpen);
}

void UPlayerHUD::ChangeSkill(const bool isPressedRight)
{
	//WBP_SkillIconUI->ChangeSkill(isPressedRight);
}

void UPlayerHUD::ChangeHealCount(int value)
{
	WBP_SkillIconUI->ChangeHealCount(value);
}

void UPlayerHUD::Reset()
{
	SkillUI->ResetTimer();
}
