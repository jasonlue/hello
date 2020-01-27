/*
 * CRecursion.cpp
 *
 *  Created on: Sep 5, 2018
 *      Author: jason
 */

#include <climits>
#include <iostream>
#include <set>
#include "CRecursion.h"
#include "assert.h"
#include <vector>
using namespace std;

CRecursion::CRecursion() {
	// TODO Auto-generated constructor stub

}

CRecursion::~CRecursion() {
	// TODO Auto-generated destructor stub
}


uint64_t CRecursion::fib(int n)
{
	unordered_map<int,uint64_t> mp;
	uint64_t f1= fibBf(n);
	uint64_t f2 = fibDp(n,mp);
	uint64_t f3 = fibBu(n);
	assert(f1==f2 && f2==f3);
	return f1;
}

int CRecursion::minCost(int n)
{
	vector<vector<int>> tbl;
	tbl.resize(n);
	for(int i=0; i<n; i++)
	{
		tbl[i].resize(n,-1);
	}
	int c1 = minCostBf(1,n);
	int c2 = minCostDp(1,n,tbl);
	int c3 = minCostBu(n);
	assert(c1==c2&&c2==c3);
	return c1;
}

//////////////////////////////////////////////////////////////
//Private internal calls.
uint64_t CRecursion::fibBf(int i) //brute force
{

	uint64_t f = 1;
	if( i>2)
	{
		f = fibBf(i-1) + fibBf(i-2);
	}
	return f;
}

uint64_t CRecursion::fibDp(int i, unordered_map<int,uint64_t>& mp) //dynamic programming memoizing
{
	uint64_t f = 1;
	if(mp.find(i) != mp.end())
		f = mp[i];
	else
	{
		if( i>2)
		{
			f = fibDp(i-1,mp) + fibDp(i-2,mp);
			mp[i] = f;
		}
	}
	return f;
}

uint64_t CRecursion::fibBu(int i) //bottom up
{
	if( i<3 )
		return 1;

	uint64_t fib1=1,fib2=1, fib3 = fib1+fib2;
	for(int j=3; j<=i; j++)
	{
		fib3 = fib1 + fib2;
		fib1 = fib2;
		fib2 = fib3;
	}
	return fib3;
}

int CRecursion::minCostBf(int l, int r)
{
	if(l>r)
		cout << __FUNCTION__ << "([])" << endl;
	else
		cout << __FUNCTION__ << "([" << l << "," << r << "])" << endl;
	if(l>=r)
		return 0;

	int res = INT_MAX;
	for(int i=l; i<=r; i++)
	{
		res = min(res, i+max(minCostBf(l,i-1), minCostBf(i+1,r)));
	}
	return res;
}

int CRecursion::minCostDp(int l, int r, vector<vector<int>>& tbl)
{
	if(l>=r)
		return 0;

	if(tbl[l-1][r-1]>=0)
		return tbl[l-1][r-1];

	int res = INT_MAX;
	for(int i=l; i<=r; i++)
	{
		res = min(res, i+max(minCostDp(l,i-1,tbl), minCostDp(i+1,r,tbl)));
	}

	tbl[l-1][r-1] = res;
	return res;
}

int CRecursion::minCostBu(int n)
{
	if(n<=1)
		return 0;

	vector<vector<int>> tbl;
	tbl.resize(n);
	for(int i=0;i<tbl.size();i++)
	{
		tbl[i].resize(n,-1);
		tbl[i][i] = 0; //one item. cost is 0.
	}
	for(int len=1; len<=n; len++)
	{ 	//cost[1,len]
		for(int l=1; l<=len; l++)
		{
			for(int r=l; r<=len; r++)
			{
				int cost = INT_MAX;
				for(int k=l; k<=r; k++)
				{
					int lc = 0;
					if(l-1<=k-2)
					{
						cout << "tbl[" << l-1 << "," << k-2 << "]="<<tbl[l-1][k-2]<<endl;
						assert(tbl[l-1][k-2]>=0);
						lc = tbl[l-1][k-2];
					}
					int rc = 0;
					if(k<=r-1)
					{
						cout << "tbl[" << k << "," << r-1 << "]="<<tbl[k][r-1]<<endl;
						assert(tbl[k][r-1]>=0);
						rc = tbl[k][r-1];
					}
					cost = min(cost, k+max(lc, rc));
				}
				tbl[l-1][r-1] = cost;
			}
		}
	}
	return tbl[0][n-1];
}

int CRecursion::numWays(string data)
{
	unordered_map<int,int> lMap;
	return numWays(data, 0, lMap);
}
int CRecursion::numWays(string data, int l, unordered_map<int,int>& lMap)
{
	if(lMap.find(l) != lMap.end())
		return lMap[l];

	int ways = 0;
	if (l > (int)data.length()-1)
		return 1;

	if( l == (int)data.length()-1)
		return data[l] != '0' ? 1 : 0;

	int i = (data[l]-'0')*10 + data[l+1]-'0';
	if( i>=10 && i <=26 )
	{//1 valid split.
		ways = numWays(data, l+2, lMap);
	}

	if( data[l] != '0' )
		ways += numWays(data,l+1, lMap);

	lMap[l] = ways;
	return ways;
}

int CRecursion::numWaysOfStaircase(int n)
{//1 step or 2 step at a time allowed.
	if( n<=1)
		return 1;
	int num = 0;
	//1 step.
	num += numWaysOfStaircase(n-1);
	num += numWaysOfStaircase(n-2);
	return num;
}

int CRecursion::numWaysOfStaircase(int n, set<int>& allowedSteps)
{
	if(allowedSteps.empty())
		return 0;

	if (n == 0)
		return 1;
	else if(n < *allowedSteps.begin())
		return 0;
	else if(n == *allowedSteps.begin())
	{
		return 1;
	}
	else
	{
		int numWays = 0;
		for(auto it=allowedSteps.begin(); it!=allowedSteps.end(); ++it)
		{
			if(n<*it)
				break;
			numWays += numWaysOfStaircase(n-*it, allowedSteps);
		}
		return numWays;
	}
}

int CRecursion::maxSizeOfBlock1s(vector<vector<int>>& A, int x, int y, CRect& maxBlock)
{
	return 0;
}
int CRecursion::largestBlock1s(vector<vector<int>>& A)
{
	if(A.empty() || A[0].empty())
		return 0;
	CRect maxBlock;
	int size = maxSizeOfBlock1s(A,0,0, maxBlock);
	return size;
}


void CRecursion::printAllSubsets(vector<int>&  v, int i, vector<int>& subset)
{
	if(i>=v.size())
	{
		for(auto i: subset)
		{
			if(i != 0)
				cout << i << ",";
		}
		cout << endl;
		return;
	}
	subset[i] = 0;
	printAllSubsets(v,i+1,subset);
	subset[i] = v[i];
	printAllSubsets(v,i+1,subset);
}
void CRecursion::printAllSubsets(vector<int>&  v)
{
	vector<int> subset(v.size());
	printAllSubsets(v,0, subset);
}
void CRecursion::main()
{
	CRecursion r;
	vector<int> v({1,2,3});
	r.printAllSubsets(v);
	vector<vector<vector<int>>> A({{{}}, {{1}},
			{{0,0},{0,0}},{{0,0},{0,1}},{{1,0},{0,1}}, {{0,1},{1,1}},{{1,1},{1,1}}});
	for(auto a:A)
		cout << r.largestBlock1s(a) << endl;


	for(int i=1; i<20; i++)
	{
		int n = r.numWaysOfStaircase(i);
		cout << "ways to go " << i << "steps : " << n << endl;
		set<int> s({1,3,5});
		n = r.numWaysOfStaircase(i,s);
		cout << "ways to go " << i << "steps : " << n << endl;

	}
	for( string s: {"","0","01", "1", "12", "123", "1234", "12340", "12120"})
	{
		int n = r.numWays(s);
		cout << "ways to decode " << s << ": " << n << endl;
	}
	cout << "minCost" << endl;
//	for(int i=2; i<10; i++)
	{
//		cout << "minCost(" << i << "):" << r.minCost(i) << endl;
	}
	cout << "Fibonacci" << endl;
	for(int i=1; i<50; i++)
	{
		cout << "fib(" << i << "):" << r.fib(i) << endl;
	}
}

