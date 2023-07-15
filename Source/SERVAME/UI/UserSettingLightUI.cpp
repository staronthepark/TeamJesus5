// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSettingLightUI.h"
#include "..\Manager\JesusGameInstance.h"

void UUserSettingLightUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	WBP_Setting_Slider->Slider_1->OnValueChanged.AddDynamic(this, &UUserSettingLightUI::ChangeSliderValue);
}

void UUserSettingLightUI::NativeConstruct()
{
	Super::NativeConstruct();
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	float value = 1 - GameInstance->GetPostProcessBrightness();
	WBP_Setting_Slider->SetValue(value);
	RightImage->SetRenderOpacity(value);
}

void UUserSettingLightUI::ChangeSliderValue(float value)
{
	RightImage->SetRenderOpacity(value);
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->SetPostProcessBrightness(1 - value);
}
