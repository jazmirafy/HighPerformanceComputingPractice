//Given the serial code that adds two vectors, convert the code into a parallel program and use it to find the sum
//of the elements of the array given in the code.Implement MUTEX locks to the required parts of your code.

// iostream library for input output via console/terminal
#include <iostream>
// thread library for threading operations
#include <thread>
//need this for the mutex lock
#include <mutex>
//stopwatch library
#include "StopWatch.h"
// std namespace for standard library functions and classes
using namespace std;

// given the arrays
int a[] = { 4, 3, 1, 6, 8, 9, 100, 23, 13, 44, 14, 67, 89, 34, 23, 69, 100, 65, 32, 33, 4 ,5 ,6 ,18};
int b[] = { 1, 7, 8, 4, 9, 12, 10, 3, 1, 14, 7, 7, 9, 3, 3, 6, 11, 165, 2, 1, 14, 15, 8, 33 };
int c[24]; // there are 24 elements in a and b
//performance here could be increased by extending the array length/spacing the array so caching doesn't block any threads temporarily


// thread fuction with two arguments
//thread_id = the thread id for current active thread
//size = data partition size
void myThreadMethod(int thread_id, int size) {
	for (int i = thread_id * size; i < (thread_id + 1) * size; i++)
	{
		// adds elements of a and b together and assigns it to the corresponding index in c
		for (int i = 0; i < 24; i++)
		{
			c[i] = a[i] + b[i];
		}
	}
	//we wont need a mutex lock in this case because each thread is manipulating a different part of the array
	//since the threads aren't manipulating the same elements of array c, we won't have to worry about non deterministic results
}


int main() {
	//assume that the number and the processor numbers are given as:

	int p = 3; // thread count
	int n = 24; // array length
	int size = n / p; // partition size

	// thread array for holding the thread instances
	thread* t = new thread[p];

	// thread creation (actual creation and assignment) loop
	for (int i = 0; i < p; i++)
	{
		// create a thread with the function and required arguments
		// the creation operation instantly starts the thread to run
		t[i] = thread(myThreadMethod, i, size);
	}

	// joining loop to wait all thread finish operations
	for (int i = 0; i < p; i++)
	{
		// join function is a blocking function, it returns when thread finishes.
		t[i].join();
	}

	// showing the elements of array c
	for (int i = 0; i < 24; i++)
	{
		cout << c[i] << " ";
	}

}