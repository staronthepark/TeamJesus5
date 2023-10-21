// Fill out your copyright notice in the Description page of Project Settings.


#include "JamsigAniminstance.h"

UJamsigAniminstance::UJamsigAniminstance()
{

}

void UJamsigAniminstance::AnimNotify_KnockBackStart() { KnockBackStart.Broadcast(); }

void UJamsigAniminstance::AnimNotify_KnockBackEnd() { KnockBackEnd.Broadcast(); }
