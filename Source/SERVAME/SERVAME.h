// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(BLOG_LOG, Log, All);

//1. ȣ��� �Լ��� �̸��� ���� ��ũ�θ� Stringȭ ��Ŵ
#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

//2. 1�� CallInfo�� �Բ� �ƹ� ������ ���� �α׸� ��� ��ũ��
#define LOG_S(Verbosity) UE_LOG(BLOG_LOG, Verbosity, TEXT("%s"), *LOG_CALLINFO)

#define LOG_F(Verbosity, Format) UE_LOG(BLOG_LOG, Verbosity, TEXT("%s %f"), *LOG_CALLINFO, Format)

//3. CallInfo�� �Բ� �ؽ�Ʈ�� �α׸� ��� ��ũ��
#define LOG(Verbosity, Format, ...) UE_LOG(BLOG_LOG, Verbosity, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

//4. LOG�� Verbosity�� ���ڿ��� ���� ��ũ��ȭ ��Ų��. 
#define LOG_WARNING(Format, ...) UE_LOG(BLOG_LOG, Warning, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define LOG_ERROR(Format, ...) UE_LOG(BLOG_LOG, Error, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

//5. assert()�� ���� ����� �뵵�� ��밡���� ��ũ��
#define CHECK(Expr, ...) {if(!(Expr)) {LOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}

//6. ���� �÷��� ��ũ�����ٰ� LOG�� ���� ��� ��ũ��
#define	LOG_SCREEN(Format, ...) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(Format, ##__VA_ARGS__))