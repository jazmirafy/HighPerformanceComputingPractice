//Given the serial code that finds the dot product of two vectors, convert the code into a parallel program, create
//at least three threads and use them to find the dot product of the arrays given in the code.Implement MUTEX
//locks to the required parts of your code.
#include <iostream>
#include <thread>
#include <mutex>
#include "StopWatch.h"
#include <climits>

using namespace std;

// given the arrays 
int a[] = { 4, 3, 1, 6, 8, 9, 100, 23, 13, 44, 14, 67, 89, 34, 23, 69, 100, 65, 32, 33, 4 ,5 ,6 ,18 };
int b[] = { 1, 7, 8, 4, 9, 12, 10, 3, 1, 14, 7, 7, 9, 3, 3, 6, 11, 165, 2, 1, 14, 15, 8, 33 };
int sharedResult = 0;

mutex mutexLock;


void myThreadMethod(int dataSize, int thread_id, int threadCount) {

	StopWatch stopWatch;
	stopWatch.start();

	//determine the section of data the thread should work on
	int start = thread_id * (dataSize / threadCount);
	int end = (thread_id + 1) * (dataSize / threadCount);

	int myResult = 0;

	//compute dot product of dedicated vector sections
	for (int i = start; i < end; i++)
	{
		myResult += a[i] * b[i];
	}

	//locking before submitting local results to global result variable to prevent non deterministic behavior
	mutexLock.lock();
	sharedResult += myResult;
	stopWatch.stop();
	cout << "Thread " << thread_id << " took " << stopWatch.elapsedTime() << " ms to finish" << endl;
	mutexLock.unlock();
}

void startThreads(int threadCount) {

	//create and start thread instances
	thread* t = new thread[threadCount];
	for (int i = 0; i < threadCount; i++)
	{
		t[i] = thread(myThreadMethod, 24,  i, threadCount);
	}

	//join and delete threads when threads finish task
	for (int i = 0; i < threadCount; i++)
	{
		t[i].join();
	}
	delete[] t;
}


int main() {

	startThreads(4);
	cout << "The result is: " << sharedResult << endl;
}