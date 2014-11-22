#include "threadpool.h"
#include "server.h"
#include <iostream>
#include <vector>
using namespace std;


Server::Server()
{	
}

Server::~Server()
{
}

int Server::Execute()
{
	cout << "server execute" << endl;
	return 0;
}

int main()
{
	ThreadPool gThreadPool;
	
	EvaluationPU *lpEvaluationPU = gThreadPool.GetEvaluationPU();

	vector<IEvaluation*> lvServer;
	//instance 5 sever instance
	for(int i = 0; i < 5; i ++)
	{
		IEvaluation *lpServer = new Server();
		lvServer.push_back(lpServer);
	}

	lpEvaluationPU->Execute(5, &(lvServer[0]));
				
	//delete the server instance, TBD
	
	return 0;
}
