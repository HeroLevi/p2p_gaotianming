#include <iostream>
#include <pthread.h>
#include <atomic>
using namespace std;

bool shutdown = false;
bool Des()
{
	for(;;)
	{
		if(shutdown)
			return true;
		else
		{
			cout<<"a"<<endl;
			if(__sync_bool_compare_and_swap(&shutdown,false,true))
			{
				cout<<"b"<<endl;
				return true;
			}
		}
	}
	return true;
}
bool flag;
void* Test(void* arg)
{
	flag = Des();
	cout<<&flag<<endl;
}

int main(void)
{
	pthread_t pid[2048];
	for(int i=0;i<2048;i++)
	{
		pthread_create(&pid[i],NULL,Test,(void*)NULL);
	}

	return 0;
}
