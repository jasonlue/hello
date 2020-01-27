/*
 * CMeasure.h
 *
 *  Created on: Sep 6, 2018
 *      Author: jason
 */

#ifndef CMEASURE_H_
#define CMEASURE_H_
#include <chrono>
#include <string>
using namespace std;
//measure milliseconds.
class CMeasure {
public:
	string name;
	chrono::high_resolution_clock::time_point s;
	int64_t nanoseconds;

public:
	CMeasure(string n="");
	virtual ~CMeasure();
};

#endif /* CMEASURE_H_ */
