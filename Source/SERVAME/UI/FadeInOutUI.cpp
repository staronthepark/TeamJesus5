// Fill out your copyright notice in the Description page of Project Settings.


#include "FadeInOutUI.h"

void UFadeInOutUI::PlayFadeInOutAnimation(bool isFadeIn)
{
	isFadeIn ? PlayAnimation(FadeInAnimation) : PlayAnimation(FadeOutAnimation);
}
