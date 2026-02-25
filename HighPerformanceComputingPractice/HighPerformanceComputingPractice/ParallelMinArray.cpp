//Given the serial code that finds the minimum of an array, convert the code into a parallel program, create at
//least three threads and use them to find the min of the elements of the array given in the code.Implement
//MUTEX locks to the required parts of your code.
#include <iostream>
#include <thread>
#include <mutex>
#include "StopWatch.h"
#include <climits>

using namespace std;

// given the array 
int a[] = { 4, 3, 1, 6, 8, 9, 100, 23, 13, 44, 14, 67, 89, 34, 23, 69, 100, 65, 32, 33, 4 ,5 ,6 ,18 };
int sharedMin = INT_MAX;

mutex mutexLock;


void myThreadMethod(int dataSize, int thread_id, int threadCount) {

	StopWatch stopWatch;
	stopWatch.start();

	//determine the section of data the thread should work on
	int start = thread_id * (dataSize / threadCount);
	int end = (thread_id + 1) * (dataSize / threadCount);

	int myMin = INT_MAX;

	//track min of dedicated array sections
	for (int i = start; i < end; i++)
	{
		if (a[i] < myMin) {
			myMin = a[i];
		}
	}

	//locking before submitting local min to global min variable to prevent non deterministic behavior
	mutexLock.lock();
	if (myMin < sharedMin) {
		sharedMin = myMin;
	}
	stopWatch.stop();
	cout << "Thread " << thread_id << " took " << stopWatch.elapsedTime() << " ms to finish" << endl;
	mutexLock.unlock();
}

void startThreads(int threadCount) {

	//create and start thread instances
	thread* t = new thread[threadCount];
	for (int i = 0; i < threadCount; i++)
	{
		t[i] = thread(myThreadMethod, 24, i, threadCount);
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
	cout << "The minimum is: " << sharedMin << endl;
}