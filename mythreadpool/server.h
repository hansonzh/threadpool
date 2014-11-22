#ifndef SERVER_H
#define SERVER_H

#include "threadpool.h"

class Server: public IEvaluation {
public:
	Server();
	~Server();	

	//IEvaluation
	int Execute();
};

#endif
