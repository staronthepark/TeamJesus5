// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightController.h"

AKnightController::AKnightController()
{

}

void AKnightController::BeginPlay()
{
	Super::BeginPlay();
}

void AKnightController::Movement(FVector Location)
{
	MoveToLocation(Location);
}