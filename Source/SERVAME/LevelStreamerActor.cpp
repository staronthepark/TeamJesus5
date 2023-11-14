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
		UCombatManager& CombatManager = UCombatManager::GetInstance();
		APlayerCharacter* character = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		character->CurrentMapName = LevelToLoad;

		for (int32 i = 0; i < character->MonsterInfoMap[LevelToLoad.ToString()].Num(); i++)
		{
			if (!character->MonsterInfoMap[LevelToLoad.ToString()][i]->IsDie)
			{
				character->MonsterInfoMap[LevelToLoad.ToString()][i]->RespawnCharacter();
			}
		}
	}
}

void ALevelStreamerActor::OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (LoadType == ELoadType::ONLYLOAD  || IsLoadWhenOverlapEnds)return;
	if (LevelToLoad != "")
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::UnloadStreamLevel(this, LevelToLoad, LatentInfo, false);
		UCombatManager& CombatManager = UCombatManager::GetInstance();
	}
}

void ALevelStreamerActor::OverlapEndsLoad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsLoadWhenOverlapEnds)return;
	if (LevelToLoad != "")
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);
		UCombatManager& CombatManager = UCombatManager::GetInstance();
		APlayerCharacter* character = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		character->CurrentMapName = LevelToLoad;


		for (int32 i = 0; i < character->MonsterInfoMap[LevelToLoad.ToString()].Num(); i++)
		{
			if (!character->MonsterInfoMap[LevelToLoad.ToString()][i]->IsDie)
			{
				character->MonsterInfoMap[LevelToLoad.ToString()][i]->RespawnCharacter();
			}
		}
	}
}