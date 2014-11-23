
server:threadpool.o server.o
	g++ -g -O0 -o server threadpool.o server.o -pthread

threadpool.o:threadpool.cpp threadpool.h
	g++ -g -O0 -c threadpool.cpp -pthread

server.o:server.cpp server.h
	g++ -g -O0 -c server.cpp

clean:
	rm server *.o
	
