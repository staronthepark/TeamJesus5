// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StaticMeshCollision.h"
#include "StaticMeshCollisionWall.generated.h"

/**
 * 
 */
UCLASS()
class SERVAME_API AStaticMeshCollisionWall : public AStaticMeshCollision
{
	GENERATED_BODY()

		AStaticMeshCollisionWall() {};


protected:
	virtual void BeginPlay() override;

	virtual void OverlapBegin(UPrimitiveComponent* OtherComp) override;
	virtual void OverlapEnd(UPrimitiveComponent* OtherComp) override {};
};
