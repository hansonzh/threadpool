#include "threadpool.h"
#include <iostream>
#include <unistd.h>
using namespace std;

//ThreadPool EvaluationPU::mThreadPool;

extern "C"
void * StartExecute(void *ipThreadPool)
{
	ThreadPool *lpThreadPool = (ThreadPool*)ipThreadPool;
	lpThreadPool->Execute();
	return NULL;
}

EvaluationPU::EvaluationPU()
{
	pthread_mutex_init(&mMutex, NULL);
	pthread_cond_init(&mCond, NULL);

	pthread_cond_init(&mCondTask, NULL);
	
	//clear the queue
	while(!mqEvaluation.empty())
		mqEvaluation.pop();
}

EvaluationPU::~EvaluationPU()
{
}

int EvaluationPU::hAddEvaluation(IEvaluation *ipEvaluation)
{
	pthread_mutex_lock(&mMutex);
	
	mqEvaluation.push(ipEvaluation);

	mAddedTask ++;
	
	pthread_cond_signal(&mCond);

	pthread_mutex_unlock(&mMutex);	

	return 0;	
}

int EvaluationPU::AddEvaluation(int iSize, IEvaluation **ippEvaluation)
{
	for(int i = 0; i < iSize; i ++)
	{
		if(hAddEvaluation(*(ippEvaluation + i)) != 0)
		{
			cout << "add Evaluaton error" << endl;
			return -1;	
		}
	}

	return 0;
}

//this function should be not parallel called
int EvaluationPU::Execute(int iSize, IEvaluation **ippEvaluation)
{
	//add the evaluation
	mAddedTask = 0;
	mFinishedTask = 0;
	mExpectedTask = iSize;
	AddEvaluation(iSize, ippEvaluation);

	//return untill the iSize of evalution is finished
	//need add synchronization
	//sleep(5);
	
	pthread_mutex_lock(&mMutex);
	
	pthread_cond_wait(&mCondTask, &mMutex);

	pthread_mutex_unlock(&mMutex);
	
	//sleep(5);
	
	return 0;
}

int EvaluationPU::GetEvaluation(IEvaluation **oppEvaluation)
{
	if(oppEvaluation == NULL)
		return -1;

	pthread_mutex_lock(&mMutex);

	if(mqEvaluation.empty())
		pthread_cond_wait(&mCond, &mMutex);

	*oppEvaluation = mqEvaluation.front();
	mqEvaluation.pop();

	pthread_mutex_unlock(&mMutex);
	
	return 0;
}

int EvaluationPU::TrySignalCondTask()
{
	pthread_mutex_lock(&mMutex);

	mFinishedTask ++;
	
	if(mAddedTask == mExpectedTask && mFinishedTask == mAddedTask)
		pthread_cond_signal(&mCondTask);
	
	pthread_mutex_unlock(&mMutex);

	return 0;
}

//it is not a good design to expose a private class member
//TBD, any better solution?
EvaluationPU* ThreadPool::GetEvaluationPU()
{
	return &mEvaluationPU;
}

ThreadPool::ThreadPool()
{
	Init();	
}

ThreadPool::~ThreadPool()
{
}

int ThreadPool::Init()
{
	for(int i = 0; i < MAX_THREADS; i ++)
	{
		if(pthread_create(mThreads + i, NULL, StartExecute, this) != 0)
		{
			cout << "create " << i << "th thread failed" << endl;
			return -1;			
		}	
	}

	return 0;
}

int ThreadPool::Execute()
{
	while(true)
	{
		IEvaluation *lpEvaluation = NULL;
			
		mEvaluationPU.GetEvaluation(&lpEvaluation);
		
		lpEvaluation->Execute();

		mEvaluationPU.TrySignalCondTask();
	}
	
	return 0;
}
