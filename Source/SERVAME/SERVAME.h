// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(BLOG_LOG, Log, All);

//1. 호출된 함수의 이름과 라인 매크로를 String화 시킴
#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

//2. 1번 CallInfo와 함께 아무 내용이 없는 로그를 찍는 매크로
#define LOG_S(Verbosity) UE_LOG(BLOG_LOG, Verbosity, TEXT("%s"), *LOG_CALLINFO)

#define LOG_F(Verbosity, Format) UE_LOG(BLOG_LOG, Verbosity, TEXT("%s %f"), *LOG_CALLINFO, Format)

//3. CallInfo와 함께 텍스트로 로그를 찍는 매크로
#define LOG(Verbosity, Format, ...) UE_LOG(BLOG_LOG, Verbosity, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

//4. LOG에 Verbosity를 인자에서 빼고 매크로화 시킨것. 
#define LOG_WARNING(Format, ...) UE_LOG(BLOG_LOG, Warning, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define LOG_ERROR(Format, ...) UE_LOG(BLOG_LOG, Error, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

//5. assert()와 거의 비슷한 용도로 사용가능한 매크로
#define CHECK(Expr, ...) {if(!(Expr)) {LOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}

//6. 게임 플레이 스크린에다가 LOG를 직접 찍는 매크로
#define	LOG_SCREEN(Format, ...) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(Format, ##__VA_ARGS__))