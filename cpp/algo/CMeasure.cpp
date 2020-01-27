/*
 * CMeasure.cpp
 *
 *  Created on: Sep 6, 2018
 *      Author: jason
 */

#include "CMeasure.h"
#include <iostream>
using namespace std;
CMeasure::CMeasure(string name) : name(name)
{
	s= std::chrono::high_resolution_clock::now();
}

CMeasure::~CMeasure()
{
	auto d = chrono::high_resolution_clock::now() - s;
	nanoseconds = chrono::duration_cast<chrono::nanoseconds>(d).count();
	cout << name << ":" << nanoseconds << " nanoseconds" << endl;
}

