#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "FlowerActor.generated.h"

UCLASS()
class SERVAME_API AFlowerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFlowerActor();

protected:
	virtual void BeginPlay() override;

public:	

	UPROPERTY(Editanywhere)
		UBoxComponent* TriggerComp;

	UPROPERTY(Editanywhere)
		USceneComponent* ParentComp;

	UPROPERTY(Editanywhere)
		UNiagaraComponent* NiagaraComp1;

	UPROPERTY(Editanywhere)
		UStaticMeshComponent* ChangeMeshComp;

	UPROPERTY(Editanywhere)
		UStaticMeshComponent* OriginMeshComp;


	UFUNCTION()
		void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
