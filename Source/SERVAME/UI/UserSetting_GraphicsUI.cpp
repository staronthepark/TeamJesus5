// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSetting_GraphicsUI.h"
#include <SERVAME/Manager/JesusGameInstance.h>

void UUserSetting_GraphicsUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WBP_Volumetric_Button->LeftButton->OnClicked.AddDynamic(this, &UUserSetting_GraphicsUI::SetVolumetic);
	WBP_Volumetric_Button->RightButton->OnClicked.AddDynamic(this, &UUserSetting_GraphicsUI::SetVolumetic);

	WBP_Shadow_Button->LeftButton->OnClicked.AddDynamic(this, &UUserSetting_GraphicsUI::SetShadow);
	WBP_Shadow_Button->RightButton->OnClicked.AddDynamic(this, &UUserSetting_GraphicsUI::SetShadow);

	WBP_GI_Button->LeftButton->OnClicked.AddDynamic(this, &UUserSetting_GraphicsUI::SetGI);
	WBP_GI_Button->RightButton->OnClicked.AddDynamic(this, &UUserSetting_GraphicsUI::SetGI);

	WBP_Effect_Button->LeftButton->OnClicked.AddDynamic(this, &UUserSetting_GraphicsUI::SetEffect);
	WBP_Effect_Button->RightButton->OnClicked.AddDynamic(this, &UUserSetting_GraphicsUI::SetEffect);

	WBP_Reflect_Button->LeftButton->OnClicked.AddDynamic(this, &UUserSetting_GraphicsUI::SetReflect);
	WBP_Reflect_Button->RightButton->OnClicked.AddDynamic(this, &UUserSetting_GraphicsUI::SetReflect);

	Setting = GEngine->GetGameUserSettings();

	WBP_Volumetric_Button->SetValue(Setting->GetPostProcessingQuality() - 1);
	WBP_Shadow_Button->SetValue(Setting->GetShadingQuality() - 1);
	WBP_GI_Button->SetValue(Setting->GetGlobalIlluminationQuality() - 1);
	WBP_Effect_Button->SetValue(Setting->GetVisualEffectQuality() - 2);
	WBP_Reflect_Button->SetValue(Setting->GetReflectionQuality() - 1);

	ButtonArray.Add(WBP_Volumetric_Button);
	ButtonArray.Add(WBP_Shadow_Button);
	ButtonArray.Add(WBP_GI_Button);
	ButtonArray.Add(WBP_Effect_Button);
	ButtonArray.Add(WBP_Reflect_Button);

}

void UUserSetting_GraphicsUI::NativeConstruct()
{
	Super::NativeConstruct();
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	ChangeLanguage(GameInstance->language);
	index = 0;
	LeftRightIndex = 0;
}

void UUserSetting_GraphicsUI::SetVolumetic()
{
	Setting->SetPostProcessingQuality(WBP_Volumetric_Button->GetValue() + 1);
	Setting->ApplySettings(true);
}

void UUserSetting_GraphicsUI::SetShadow()
{
	Setting->SetShadowQuality(WBP_Shadow_Button->GetValue() + 1);
	Setting->ApplySettings(true);
}

void UUserSetting_GraphicsUI::SetGI()
{
	Setting->SetGlobalIlluminationQuality(WBP_GI_Button->GetValue() + 1);
	Setting->ApplySettings(true);
}

void UUserSetting_GraphicsUI::SetEffect()
{
	Setting->SetVisualEffectQuality(WBP_Effect_Button->GetValue() + 2);
	Setting->ApplySettings(true);
}

void UUserSetting_GraphicsUI::SetReflect()
{
	Setting->SetReflectionQuality(WBP_Reflect_Button->GetValue() + 1);
	Setting->ApplySettings(true);
}



void UUserSetting_GraphicsUI::ChangeLanguage(Language& language)
{
	if (language == Language::ENG)
	{
		//TitleGraphicImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::title)->EngTexture, true);
		VolumetricImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::volumetric)->EngTexture, true);
		ShadowImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::shadow)->EngTexture, true);
		GIImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::Gi)->EngTexture, true);
		EffectImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::effect)->EngTexture, true);
		ReflectImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::reflect)->EngTexture, true);
	}
	else if (language == Language::KOR)
	{
		//TitleGraphicImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::title)->KorTexture, true);
		VolumetricImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::volumetric)->KorTexture, true);
		ShadowImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::shadow)->KorTexture, true);
		GIImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::Gi)->KorTexture, true);
		EffectImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::effect)->KorTexture, true);
		ReflectImage->SetBrushFromTexture(ImageTextures.Find(EGraphicsSettings::reflect)->KorTexture, true);
	}

	WBP_Volumetric_Button->ChangeLanguage(language);
	WBP_Shadow_Button->ChangeLanguage(language);
	WBP_GI_Button->ChangeLanguage(language);
	WBP_Effect_Button->ChangeLanguage(language);
	WBP_Reflect_Button->ChangeLanguage(language);
}

FReply UUserSetting_GraphicsUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = FReply::Unhandled();
	if (InKeyEvent.GetKey() == EKeys::Down)
	{
		ButtonArray[index]->Buttons[LeftRightIndex]->OnUnhovered.Broadcast();
		index = FMath::Clamp(index + 1, 0, ButtonArray.Num() - 1);
		ButtonArray[index]->Buttons[LeftRightIndex]->OnHovered.Broadcast();
		Reply = FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Up)
	{
		ButtonArray[index]->Buttons[LeftRightIndex]->OnUnhovered.Broadcast();
		index = FMath::Clamp(index - 1, 0, ButtonArray.Num() - 1);
		ButtonArray[index]->Buttons[LeftRightIndex]->OnHovered.Broadcast();
		Reply = FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Right)
	{
		ButtonArray[index]->Buttons[LeftRightIndex]->OnUnhovered.Broadcast();
		LeftRightIndex = FMath::Clamp(LeftRightIndex + 1, 0, ButtonArray[index]->Buttons.Num() - 1);
		ButtonArray[index]->Buttons[LeftRightIndex]->OnHovered.Broadcast();
		Reply = FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Left)
	{
		ButtonArray[index]->Buttons[LeftRightIndex]->OnUnhovered.Broadcast();
		LeftRightIndex = FMath::Clamp(LeftRightIndex - 1, 0, ButtonArray[index]->Buttons.Num() - 1);
		ButtonArray[index]->Buttons[LeftRightIndex]->OnHovered.Broadcast();
		Reply = FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Enter)
	{
		ButtonArray[index]->Buttons[LeftRightIndex]->OnClicked.Broadcast();
		Reply = FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Escape || InKeyEvent.GetKey() == EKeys::Platform_Delete)
	{
		GetParent()->SetKeyboardFocus();
		Reply = FReply::Handled();
	}

	UE_LOG(LogTemp, Warning, TEXT("Index = %d\nLeftRightIndex = %d"), index, LeftRightIndex);
	return Reply;
}
