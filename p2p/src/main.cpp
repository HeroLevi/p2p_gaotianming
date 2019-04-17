#include "Tracker.h"
#include "ThreadPool.h"

int main(void)
{
	//Tracker().start();
	ThreadPool* ptest = ThreadPool::GetInstance();
	ptest->InitThreadPool(10,1,10);
	delete ptest;
	ptest = NULL;
	return 0;

}

