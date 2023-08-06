// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <condition_variable>
#include <mutex>
#include <vector>

/**
 * 
 */
class SERVAME_API JesusThreadManager : public FRunnable
{
public:
	static JesusThreadManager& GetInstance()
	{
		static JesusThreadManager ThreadManager;
		return ThreadManager;
	}

	bool Init() override;
	uint32 Run() override;
	void Exit() override;

	void Work();
	void EnqueueJob(TFunction<void(void)> job);

	TFunction<void(void)> CallBack;
	TQueue<TFunction<void(void)>> Job;
private:
	JesusThreadManager();
	~JesusThreadManager();
	JesusThreadManager(const JesusThreadManager& ref) {}
	JesusThreadManager& operator=(const JesusThreadManager& ref) { return *this; }

	FRunnableThread* Thread;
	TArray<FRunnableThread*> Threads;
	std::condition_variable cv;
	std::mutex m;
	int threadNum = 4;
	bool StopAllThread;
};

