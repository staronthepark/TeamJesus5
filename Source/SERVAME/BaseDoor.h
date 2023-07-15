// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include ".\Player\PlayerCharacter.h"
#include "DoorAnimInstance.h"
#include "BaseDoor.generated.h"

UCLASS()
class SERVAME_API ABaseDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseDoor();

	UBoxComponent* BoxComp;
	UDoorAnimInstance* AnimInstance;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* montage;

	bool IsCollision;
	bool IsOpen;




	UFUNCTION()
		void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime);
};
