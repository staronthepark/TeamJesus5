#include "LevelStreamerActor.h"
#include "Kismet/GameplayStatics.h"
#include "../SERVAME/Player/PlayerCharacter.h"
#include "./Manager/CombatManager.h"

ALevelStreamerActor::ALevelStreamerActor()
{
	PrimaryActorTick.bCanEverTick = true;
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	RootComponent = OverlapVolume;
}

void ALevelStreamerActor::BeginPlay()
{
	Super::BeginPlay();

	OverlapVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelStreamerActor::OverlapBegins);
	OverlapVolume->OnComponentEndOverlap.AddDynamic(this, &ALevelStreamerActor::OverlapEnds);
	OverlapVolume->OnComponentEndOverlap.AddDynamic(this, &ALevelStreamerActor::OverlapEndsLoad);

	
}

void ALevelStreamerActor::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (LoadType == ELoadType::ONLYUNLOAD || IsLoadWhenOverlapEnds)return;
	if (LevelToLoad != "")
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);		
		APlayerCharacter* character = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		character->CurrentMapName = LevelToLoad;

		character->LoadMapArray.AddUnique(LevelToLoad);
		character->LoadMonster(LevelToLoad.ToString());
	}
}

void ALevelStreamerActor::OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (LoadType == ELoadType::ONLYLOAD  || IsLoadWhenOverlapEnds)return;
	if (LevelToLoad != "")
	{
		APlayerCharacter* character = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		if (character->LoadMapArray.Contains(LevelToLoad))
		{
			character->LoadMapArray.Remove(LevelToLoad);
			FLatentActionInfo LatentInfo;
			UGameplayStatics::UnloadStreamLevel(this, LevelToLoad, LatentInfo, false);
		}
	}
}

void ALevelStreamerActor::OverlapEndsLoad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsLoadWhenOverlapEnds)return;
	if (LevelToLoad != "")
	{

	}
}