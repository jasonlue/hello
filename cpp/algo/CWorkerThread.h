/*
 * CWorkerThread.h
 *
 *  Created on: Aug 24, 2018
 *      Author: jason
 */

#ifndef CWORKERTHREAD_H_
#define CWORKERTHREAD_H_
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
class CWorkerThread {
public:
	CWorkerThread();
	virtual ~CWorkerThread();
	void Stop();
	void ThreadProc();
	static void Main();

private:
	boost::mutex mt;
	boost::condition_variable stopCondition;
	bool stop;
	int count;
};

#endif /* CWORKERTHREAD_H_ */
