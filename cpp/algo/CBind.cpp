/*
 * CBind.cpp
 *
 *  Created on: Aug 24, 2018
 *      Author: jason
 */

#include "CBind.h"
#include <boost/bind.hpp>
#include <iostream>
using namespace std;
CBind::CBind() {
	// TODO Auto-generated constructor stub

}

CBind::~CBind() {
	// TODO Auto-generated destructor stub
}


void F1()
{
	std::cout << __FUNCTION__ << std::endl;
}

void F2(int i, float f)
{
	std::cout << __FUNCTION__ <<"(i: " << i << ",f: " << f << ")" << std::endl;
}

struct Fr1
{
	int operator()(int a, int b)
	{
		std::cout << "int " << __FUNCTION__ <<"(a: " << a << ",b: " << b << ")" << std::endl;
		return a-b;
	}
	bool operator()(long a, long b)
	{
		std::cout << "bool " << __FUNCTION__ <<"(a: " << a << ",b: " << b << ")" << std::endl;
		return a==b;
	}
	int mf(int a)
	{
		std::cout << "int " << __FUNCTION__ <<"(a: " << a << ")" << std::endl;
		return a;
	}
};

struct Fr2
{
	int s;
	typedef void result_type;
	void operator()(int a)
	{
		std::cout << "void " << __FUNCTION__ <<"(a: " << a << ")" << std::endl;
		s += a;
	}
};

void TestBind()
{
	boost::bind(F1)();
	boost::bind(&F2,1,2)();
	boost::bind(F2,_1,_1)(2);
	boost::bind(F2,_1,_2)(3,4,5);
	boost::bind(F2,_2,_1)(6,7,8);
	Fr1 fr1;
	Fr2 fr2{0}, fr22{0};
	for(int i=9; i<11; i++)
	{
		boost::bind(F2,_1,_1)(i);
		boost::bind(F2,i,i+1);
		boost::bind(F2,i,i+1)();
		boost::bind(F2,i,_1)(i+1);
		boost::bind(F2,ref(i),_1)(i+1);
		boost::bind(F2,cref(i),_1)(i+1);

		boost::bind<int>(fr1,i,_1)(i);
		boost::bind<bool>(fr1,_1,_1)(i); //param type must match exactly. it matches param, not return type.
		boost::bind<bool>(fr1,_1,_1)((long)i); //param type must match exactly.
		boost::bind<bool>(fr1,long(i),_1)((long)i); //param type must match exactly.

		boost::bind(&Fr1::mf,fr1, _1)(i); //fr1.f(i)
		boost::bind(&Fr1::mf,&fr1, _1)(i); //fr1.f(i)

		boost::bind(fr2,i)();
		cout << "fr2.s=" << fr2.s << endl;
		boost::bind(ref(fr22),i)();
		cout << "fr22.s=" << fr22.s << endl;
	}

}
