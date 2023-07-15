#include "PlayerHUD.h"
#include "Components/CanvasPanelSlot.h"
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
	GuideAnimationFunction.Add(EGuides::map, [&](bool value)
		{
			PlayMapUIAnimation("asd");
		});

	isGamePad = Keyboard;


}

void UPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();
	if(this->IsInViewport())
		PlayAnimation(HUDOpenAnimation);
}

void UPlayerHUD::SetHP(float value)
{
	WBP_PlayerHPUI->SetHP(value);
}

void UPlayerHUD::DecreaseHPGradual(AActor* Actor, float value)
{
	WBP_PlayerHPUI->DecreaseHPGradual(Actor, value);
}

void UPlayerHUD::SetStamina(float value)
{
	WBP_PlayerHPUI->SetStamina(value);
}

void UPlayerHUD::DecreaseStaminaGradual(AActor* Actor, float value)
{
	WBP_PlayerHPUI->DecreaseStaminaGradual(Actor, value);
}

void UPlayerHUD::SetSkill(float value)
{
	/*WBP_PlayerHPUI->SetSkill(value);*/
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


void UPlayerHUD::PlayGuidesAnimation(EGuides type, bool IsOpen)
{
	IsOpen ? WBP_PlayerGuideUI->StartGuide(type) : WBP_PlayerGuideUI->Disable();
}

void UPlayerHUD::PlayKeyGuideAnimation(bool IsOpen)
{
	IsOpen ? PlayAnimation(GuidOpenAnimation) : PlayAnimation(GuidCloseAnimation);
}


void UPlayerHUD::PlayInteractionAnimation(bool IsOpen, EInteractions interactions)
{
	Interaction_Image->SetBrushFromTexture(InteractionTextures.Find(interactions)->Textures[isGamePad]); 
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

void UPlayerHUD::PlayMapUIAnimation(FString MapName)
{
	if (WBP_MapInfo)
		WBP_MapInfo->PlayMapInfoAnimation(MapName);
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