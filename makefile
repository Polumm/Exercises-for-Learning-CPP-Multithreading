# Remember to add "-std=c++20" for semaphore
producer_consumer: main.cpp
	g++ -g -std=c++20 -Wall main.cpp -o producer_consumer
clean:
	rm -f *.o producer_consumer