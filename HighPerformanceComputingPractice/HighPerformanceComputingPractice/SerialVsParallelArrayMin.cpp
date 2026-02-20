//LAB: Serial vs.Parallel Array Operations(Random Initialization + Elementwise Sum + Parallel Minimum)
//Write a C / C++ (or your course language) program that compares a serial implementation to a parallel implementation
//using threads.
//Part A — Serial baseline
//• Create two arrays A and B, each with 1, 000, 000 elements.
//• Populate A and B with random integers in the range[0, 100].
//• Compute a third array C where C[i] = A[i] + B[i](element - by - element).
//This completes your serial version.
//Part B — Parallel minimum(threaded)
//• Using a parallel / threaded approach, compute the minimum value in array C.
//• Run the minimum - finding routine multiple times using different thread counts(e.g., 1, 2, 4, 8, 16 — or
//	whatever your system supports).
//	Output requirements
//	• For each run, report:
//o The thread count
//o The minimum value found in C
//Performance measurement
//• Measure and report timing for :
//	o Serial total time(initialization + sum + min, or clearly separated if you prefer)
//	o Parallel min time for each thread count(and optionally total runtime per thread count)
//	

// 
// iostream library for input output via console/terminal
#include <iostream>
// thread library for threading operations
#include <thread>
//need this for the mutex lock
#include <mutex>
//stopwatch library
#include "StopWatch.h"

//libraries needed for random numbers
#include <cstdlib>
#include <ctime>     
//library needed for max integer
#include <climits>   // INT_MAX

// std namespace for standard library functions and classes
using namespace std;

// creating two arrays each with 1000000 elements. 
const int N = 1000000;

int a[N];
int b[N];

//array that will contain the sum
int c[N];

//global minimum
int sharedMin = INT_MAX;

// mutex for protecting sharedMin updates in parallel region
mutex mtx;

void mySerialMethod(int array[]) {
	//create stopwatch object
	StopWatch sw;

	//start the stopwatch to track the serial program time
	sw.start();
	// adds elements of a and b together and assigns it to the corresponding index in c
	for (int i = 0; i < N; i++)
	{
		c[i] = a[i] + b[i];
		//track the minimum for array c
		if (c[i] < sharedMin) {
			sharedMin = c[i];
		}
	}

	//end the stopwatch and print how long the serial program too
	sw.stop();
	cout << "The serial program took " << sw.elapsedTime() << " seconds to finish" << endl;
}

void myThreadMethod(int array[], int thread_id, int threadCount) {
	//create stopwatch object
	StopWatch sw;

	//start the stopwatch to track individual thread time
	sw.start();

	//allow the thread to determine what section of data it needs to work on
	//use the thread id, size of the data, and amount of threads to determine what section of data the thread should work on
	int start = thread_id * (N / threadCount);
	int end = (thread_id + 1) * (N / threadCount);

	int myMin = INT_MAX;

	// adds elements of a and b together and assigns it to the corresponding index in c
	for (int i = start; i < end; i++)
	{
		//compute c array value
		c[i] = a[i] + b[i];

		//track the minimum for designated chunk of data
		if (c[i] < myMin) {
			myMin = c[i];
		}
	}

	//find the minimum of the threads local section of c

	//after finding the minimum, compare if the local minimum is less than the global minimum
	{
		//use a lock before adjusting the global min variable to prevent non deterministic results
		mtx.lock();
		if (myMin < sharedMin) {
			sharedMin = myMin;
		}
	}
	//stop the stopwatch and print how long the individual thread took
	sw.stop();
	cout << "Thread " << thread_id << " took " << sw.elapsedTime() << " seconds to finish" << endl;
	//unlock now that thread is finished
	mtx.unlock();
	
}

void startThreads(int threadCount) {
	// thread array for holding the thread instances
	thread* t = new thread[threadCount];

	// thread creation (actual creation and assignment) loop
	for (int i = 0; i < threadCount; i++)
	{
		// create a thread with the function and required arguments
		// the creation operation instantly starts the thread to run
		t[i] = thread(myThreadMethod, c, i, threadCount);
	}

	// joining loop to wait all thread finish operations
	for (int i = 0; i < threadCount; i++)
	{
		// join function is a blocking function, it returns when thread finishes.
		t[i].join();
	}

	delete[] t;
}


int main() {

	//populate arrays a and b with random integers
	srand((unsigned)time(nullptr));
	for (int i = 0; i < N; i++) {
		a[i] = rand() % 101; // [0, 100]
		b[i] = rand() % 101; // [0, 100]
	}

	//run the serial program
	sharedMin = INT_MAX;
	mySerialMethod(c);
	//print the serial min
	cout << "The serial programs min is: " << sharedMin << endl;

	//run the parallel version
	sharedMin = INT_MAX;
	startThreads(4);
	//print the parallel min
	cout << "The parallel programs min is: " << sharedMin << endl;

}