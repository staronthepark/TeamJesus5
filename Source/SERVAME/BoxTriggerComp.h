#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "BoxTriggerComp.generated.h"

class ABaseTriggerActor;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SERVAME_API UBoxTriggerComp : public UBoxComponent
{
	GENERATED_BODY()

public:
	UBoxTriggerComp();

	ABaseTriggerActor* Owner;
public:
	virtual void BeginPlay();

	virtual void BeginEvent();
	virtual void EndEvent();
	
	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};