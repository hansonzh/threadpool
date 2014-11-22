
server:threadpool.o server.o
	g++ -g -o server threadpool.o server.o -pthread

threadpool.o:threadpool.cpp threadpool.h
	g++ -g -c threadpool.cpp -pthread

server.o:server.cpp server.h
	g++ -g -c server.cpp

clean:
	rm server *.o
	
