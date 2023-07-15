#pragma once

#include "CoreMinimal.h"
#include "BaseTriggerActor.h"
#include ".\Player\PlayerCharacter.h"
#include "InteractionActor.generated.h"

UCLASS()
class SERVAME_API AInteractionActor : public ABaseTriggerActor
{
	GENERATED_BODY()
	
public:
	AInteractionActor();


	UPROPERTY()

	APlayerCharacter* Character;

private:
	TMap<bool, TMap<bool, TFunction<void(AInteractionActor* actor)>>> TickFuncMap;

public:	

	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);
	virtual void BeginTriggerEvent();
	virtual void EndTriggerEvent();
	virtual void EnableEvent();
	void Init();

};