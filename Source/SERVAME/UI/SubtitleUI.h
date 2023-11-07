// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SubtitleUI.generated.h"

/**
 * 
 */
enum class EGuides : uint8;

USTRUCT(BlueprintType)
struct SERVAME_API FSubtitlesTimeTextures
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	UTexture2D* Texture;
	UPROPERTY(EditAnywhere)
	float Endtime;
};

UCLASS()
class SERVAME_API USubtitleUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<EGuides, FSubtitlesTimeTextures> Subtitles;

	UPROPERTY(meta = (BindWiget))
	class UImage* SubtitleImage;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* StartSubtitleAnimation;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* EndSubtitleAnimation;
	
	FTimerHandle DelayTimerHandle;

public:
	void PlaySubtitle(EGuides value);
};
