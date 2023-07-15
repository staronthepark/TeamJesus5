// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "UObject/UObjectGlobals.h"
#include "..\Manager\JesusGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UMainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	StartIntroAnimationDelegate.BindDynamic(this, &UMainWidget::StartIntroAnimation);
	StopIntroAnimationDelegate.BindDynamic(this, &UMainWidget::StoppedIntroAnimation);

	//BindToAnimationStarted(IntroAnimation, StartIntroAnimationDelegate);
	//BindToAnimationFinished(IntroAnimation, StopIntroAnimationDelegate);

	LevelToLoad = "/Game/00_Maps/01_Art/00_Modeller/KimMinYeongMap_Actor";
	LevelLoaded = false;

	StartIntroAnimation();
	//PlayAnimation(IntroAnimation);
}

void UMainWidget::StartIntroAnimation()
{
	LoadPackageAsync(LevelToLoad,
		FLoadPackageAsyncDelegate::CreateLambda([=](const FName PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result) {
			if (Result == EAsyncLoadingResult::Succeeded) {
				UE_LOG(LogTemp, Warning, TEXT("Loaded Complete"));
				LevelLoaded = true;
			}
			}),
			0,
		PKG_ContainsMap);



}

void UMainWidget::StoppedIntroAnimation()
{
	UJesusGameInstance* GameInstance = Cast<UJesusGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//if (LevelLoaded)
	UGameplayStatics::OpenLevel(this, FName(LevelToLoad));
}


