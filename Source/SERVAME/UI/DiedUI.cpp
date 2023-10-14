// Fill out your copyright notice in the Description page of Project Settings.


#include "DiedUI.h"



void UDiedUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//DiedInAnimationEvent.BindDynamic(this, &UDiedUI::DiedFadeOutAnimation);
	//BindToAnimationFinished(DiedInAnimation, DiedInAnimationEvent);
}

//void UDiedUI::DiedFadeOutAnimation()
//{
//	PlayAnimation(DiedOutAnimation);
//}


void UDiedUI::PlayDiedAnimation(bool IsOpen)
{
	//IsOpen ? PlayAnimation(DiedInAnimation) : PlayAnimation(DiedOutAnimation);
	PlayAnimation(DiedInAnimation);
}