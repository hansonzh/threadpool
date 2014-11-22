#include "threadpool.h"
#include "server.h"
#include <iostream>
#include <vector>
using namespace std;

pthread_mutex_t gMutex_t;

Server::Server()
{	
}

Server::~Server()
{
}

int Server::Execute()
{
	pthread_mutex_lock(&gMutex_t);
	static int i = 0;
	cout << "i: "<< i << " server execute" << endl;
	i ++;
	cout.flush();
	pthread_mutex_unlock(&gMutex_t);

	return 0;
}

int main()
{
	pthread_mutex_init(&gMutex_t, NULL);

	ThreadPool gThreadPool;
	
	EvaluationPU *lpEvaluationPU = gThreadPool.GetEvaluationPU();

	vector<IEvaluation*> lvServer;
	
	//instance 5 sever instance
	for(int i = 0; i < 10000; i ++)
	{
		IEvaluation *lpServer = new Server();
		lvServer.push_back(lpServer);
	}

	lpEvaluationPU->Execute(10000, &(lvServer[0]));
				
	//delete the server instance, TBD
	
	return 0;
}
