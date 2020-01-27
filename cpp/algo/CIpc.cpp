/*
 * CIPC.cpp
 *
 *  Created on: Sep 29, 2018
 *      Author: jason
 */

#include "CIpc.h"
#include "CRecursion.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

using namespace std;

CIpc::CIpc() {
	// TODO Auto-generated constructor stub

}

CIpc::~CIpc() {
	// TODO Auto-generated destructor stub
}

int count = 0;

pthread_mutex_t mtx;
pthread_spinlock_t spn;
pthread_cond_t cond;
pthread_mutex_t condMtx = PTHREAD_MUTEX_INITIALIZER;
sem_t sem;
sem_t* namedSem = NULL;
void* countUpThread(void* pParam)
{
	int x = *(int*)pParam;
	for(int i=0; i<x; i++)
	{
		sem_wait(namedSem);
//		sem_wait(&sem);
//		pthread_mutex_lock(&mtx);
//		pthread_spin_lock(&spn);
		count++;
//		pthread_spin_unlock(&spn);
//		pthread_mutex_unlock(&mtx);
//		sem_post(&sem);
		sem_post(namedSem);
	}
}

void* countDownThread(void* pParam)
{
	int x = *(int*)pParam;
	for(int i=0; i<x; i++)
	{
		sem_wait(namedSem);
//		sem_wait(&sem);
//		pthread_mutex_lock(&mtx);
//		pthread_spin_lock(&spn);
		count--;
//		pthread_spin_unlock(&spn);
//		pthread_mutex_unlock(&mtx);
//		sem_post(&sem);
		sem_post(namedSem);
	}
}

bool continueRunning = true;
void* conditionalThread(void* pParam)
{
	while(true)
	{
		pthread_mutex_lock(&condMtx);
		while(continueRunning)
		{//unlock condMtx right before going to sleep and lock condMtx again right before it wakes up.
			cout << "Waiting for signal to exit..." << endl;
			pthread_cond_wait(&cond,&condMtx);
		}
		//do sth.
		pthread_mutex_unlock(&condMtx);
		cout << "Exiting ..." << endl;
		break;
	}
}

void* fibBuThread(void* pParam)
{
	int x = *(int*)pParam;
	CRecursion r;
	for(int i=0; i<x; i++)
	{
		cout << "fibBu(" << i << "):" << r.fibBu(i) << endl;
	}
	pthread_exit(0);
}
void* fibBfThread(void* pParam)
{
	int x = *(int*)pParam;
	CRecursion r;
	for(int i=0; i<x; i++)
	{
		cout << "fibBf(" << i << "):" << r.fibBf(i) << endl;
	}
	pthread_exit(0);
}

void CIpc::main()
{
	sem_init(&sem,0,1);//unnamed semaphor.
	namedSem = sem_open("/algorithm", O_RDWR|O_CREAT, 0x644,1);

	pthread_spin_init(&spn,PTHREAD_PROCESS_PRIVATE);
	pthread_mutexattr_t mattr;
	pthread_mutexattr_init(&mattr);
	pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_NORMAL);
	pthread_mutex_init(&mtx, &mattr);


	pthread_condattr_t cattr;
	pthread_condattr_init(&cattr);
	pthread_cond_init(&cond, &cattr);


	pthread_t tid[3];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int x = 100000000;
	pthread_create(&tid[0], &attr, &countUpThread, &x);
	pthread_create(&tid[1], &attr, &countDownThread, &x);
	pthread_create(&tid[2], &attr, &conditionalThread, &x);

	sleep(10);
	pthread_cond_signal(&cond);
	sleep(10);
	continueRunning = false;
	pthread_cond_signal(&cond);

	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	pthread_join(tid[2], NULL);

	sem_close(namedSem);
	sem_destroy(&sem);
	pthread_spin_destroy(&spn);
	pthread_mutex_destroy(&mtx);
	pthread_mutexattr_destroy(&mattr);
	pthread_cond_destroy(&cond);
	pthread_condattr_destroy(&cattr);

	cout << "count=" << count << endl;
}
