#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>
#include <queue>
using namespace std;

//abstract class to define the execute interface
//currently, only the execute function
class IEvaluation {
public:
	virtual int Execute() = 0;
};

//the evaluate pu is used to push the task into the queue
//and threadpool can get the task from the queue to execute
class EvaluationPU {

friend class ThreadPool;

public:
	EvaluationPU();
	~EvaluationPU();
	
	int AddEvaluation(int iSize, IEvaluation **ippEvaluation);
	
	int GetEvaluation(IEvaluation **oppEvaluation);

	int Execute(int iSize, IEvaluation **ippEvaluation);

	int TrySignalCondTask();
private:
	int hAddEvaluation(IEvaluation *ipEvaluation);

private:
	queue<IEvaluation*> mqEvaluation;
	pthread_mutex_t	mMutex;
	pthread_cond_t	mCond;//used for the Evaluation queue

	pthread_cond_t	mCondTask; 
};

//the threadpool is a typical producter/consumer model, so we 
//have a queue to hold the task, and the task is pushed into the queue by caller
//while the pre-created threads will consum the the task
//TBD, it is better to implment as singleton
#define MAX_THREADS 10 //the pre_allocated threads

class ThreadPool {
public:
	ThreadPool();
	~ThreadPool();	
	
	int Init();	
	
	int Execute();

	EvaluationPU* GetEvaluationPU();
private:
	pthread_t mThreads[MAX_THREADS];	
	EvaluationPU mEvaluationPU;	
};
#endif

