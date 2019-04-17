#include <iostream>
#include <atomic>
#include <pthread.h>
using namespace std;

class TestA
{
	private:
		TestA(){};
		~TestA(){};
	private:
		static TestA* m_instance;
	public:
		static TestA* Getinstance()
		{
			for(;;)
			{   
				if(m_instance)
					return m_instance;
				TestA* tmp = new TestA();
				if(!m_instance)
				{
					if(__sync_bool_compare_and_swap(&m_instance,NULL,tmp))
					{
						tmp = NULL;
						return m_instance;
					}
					else
					{
						delete tmp;
						tmp = NULL;
					}
				}
			}

		}
};
TestA* TestA::m_instance = NULL;

static void* pTest(void* arg)
{
	TestA* ptr = TestA::Getinstance();
	cout<<ptr<<endl;
}

int main(void)
{
	int num = 100;
	pthread_t pid[num];
	for(int i=0;i<num;i++)
	{
		pthread_create(&pid[i],NULL,pTest,(void*)NULL);
	}

	return 0;
}
