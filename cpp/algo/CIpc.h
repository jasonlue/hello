/*
 * CIPC.h
 *  Inter Process Communication, including synchronization.
 *  
 *  Created on: Sep 29, 2018
 *      Author: jason
 */

#ifndef CIPC_H_
#define CIPC_H_
#include <pthread.h>
#include <semaphore.h>

class CIpc {
public:
	CIpc();
	virtual ~CIpc();

	static void main();
};

#endif /* CIPC_H_ */
