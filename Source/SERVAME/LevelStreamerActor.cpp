#include "LevelStreamerActor.h"
#include "Kismet/GameplayStatics.h"

ALevelStreamerActor::ALevelStreamerActor()
{
	PrimaryActorTick.bCanEverTick = true;
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	RootComponent = OverlapVolume;

	for (int32 i = 0; i < MonsterList.Num(); i++)
	{
		MonsterList[i]->SetActive(false);
	}
}

void ALevelStreamerActor::BeginPlay()
{
	Super::BeginPlay();

	OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &ALevelStreamerActor::OverlapBegins);
	OverlapVolume->OnComponentEndOverlap.AddUniqueDynamic(this, &ALevelStreamerActor::OverlapEnds);
}

void ALevelStreamerActor::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (LoadType == ELoadType::ONLYUNLOAD)return;
	if (LevelToLoad != "")
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);

		for (int32 i = 0; i < MonsterList.Num(); i++)
		{
			if(MonsterList[i]->IsAlive())
			MonsterList[i]->SetActive(true);
		}
	}
}

void ALevelStreamerActor::OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (LoadType == ELoadType::ONLYLOAD)return;
	if (LevelToLoad != "")
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::UnloadStreamLevel(this, LevelToLoad, LatentInfo, false);

		for (int32 i = 0; i < MonsterList.Num(); i++)
		{
			MonsterList[i]->SetActive(false);
		}
	}
}