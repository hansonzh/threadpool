
./test/server:threadpool.o ./test/server.o
	g++ -g -O0 -o ./test/server threadpool.o ./test/server.o -pthread

threadpool.o:threadpool.cpp threadpool.h
	g++ -g -O0 -c threadpool.cpp -pthread

./test/server.o:./test/server.cpp ./test/server.h
	g++ -g -O0 -o ./test/server.o -c ./test/server.cpp

clean:
	rm ./test/server ./test/*.o *.o
	
