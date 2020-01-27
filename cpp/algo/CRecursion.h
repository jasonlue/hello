/*
 * CRecursion.h
 *
 *  Created on: Sep 5, 2018
 *      Author: jason
 */

#ifndef CRECURSION_H_
#define CRECURSION_H_
#include <set>
#include <vector>
#include <unordered_map>
using namespace std;

struct CRect
{
	int x1,y1,x2,y2;
	CRect(int x1=0,int y1=0,int x2=0,int y2=0) :x1(x1),y1(y1),x2(x2),y2(y2){}
};
class CRecursion {
public:
	static void main();
public:
	CRecursion();
	virtual ~CRecursion();

	uint64_t fib(int n);

	//search cost of n from [1,n], given cost of n is n. success is 0.
	int minCost(int n);

public:
	uint64_t fibBf(int n);
	uint64_t fibDp(int n, unordered_map<int,uint64_t>& mp);
	uint64_t fibBu(int n);

	int minCostBf(int l, int r);
	int minCostDp(int l, int r, vector<vector<int>>& tbl);
	int minCostBu(int n);

	int numWays(string data, int l, unordered_map<int,int>& lMap);
	int numWays(string data);

	int numWaysOfStaircase(int n);
	int numWaysOfStaircase(int n, set<int>& allowedSteps);

	int maxSizeOfBlock1s(vector<vector<int>>& A, int x, int y, CRect& maxBlock);
	int largestBlock1s(vector<vector<int>>& A);

	void printAllSubsets(vector<int>& v, int i, vector<int>& subset);
	void printAllSubsets(vector<int>& v);
};

#endif /* CRECURSION_H_ */
