// Fill out your copyright notice in the Description page of Project Settings.


#include "JesusThreadManager.h"

JesusThreadManager::JesusThreadManager() : StopAllThread(false)
{
	//스레드 생성
	Threads.Reserve(threadNum);
	for (int i = 0; i < threadNum; i++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ThreadCreate"));
		Thread = FRunnableThread::Create(this, TEXT("JesusWorkerThread"));
		Threads.Push(Thread);
	}
}

JesusThreadManager::~JesusThreadManager()
{
	StopAllThread = true;
	cv.notify_all();

	for (auto& t : Threads) {
		t->Kill();
	}
}

bool JesusThreadManager::Init()
{
	//UE_LOG(LogTemp, Warning, TEXT("Thread Init"));
	return true;
}

uint32 JesusThreadManager::Run()
{
	//UE_LOG(LogTemp, Warning, TEXT("Thread Run"));
	Work();
	return 0;
}

void JesusThreadManager::Exit()
{
	//UE_LOG(LogTemp, Warning, TEXT("Thread Exit"));
}

void JesusThreadManager::Work()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(m);
		
		//조건 확인.
		//조건이 만족하면 빠져나와서 이어서 코드를 진행.
		//조건이 만족하지 않으면 lock을 풀어주고 대기 상태로 전환한다.
		cv.wait(lock, [this]() {return !this->Job.IsEmpty() || StopAllThread; });

		if (StopAllThread && this->Job.IsEmpty())
			return;

		Job.Dequeue(CallBack);

		//UE_LOG(LogTemp, Warning, TEXT("Thread Work"));
		CallBack();
		lock.unlock();
	}
}

void JesusThreadManager::EnqueueJob(TFunction<void(void)> job)
{
	if (StopAllThread)
		throw std::runtime_error("ThreadPool Stop");

	{
		std::lock_guard<std::mutex> lock(m);
		Job.Enqueue(std::move(job));
	}

	cv.notify_one();
}

