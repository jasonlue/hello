/*
 * CSearch.cpp
 *
 *  Created on: Sep 4, 2018
 *      Author: jason
 */

#include "CSearch.h"
#include <iostream>
#include <climits>
#include <assert.h>
#include <algorithm>
#include <map>
#include <queue>
using namespace std;
CSearch::CSearch() {
	// TODO Auto-generated constructor stub

}

CSearch::~CSearch() {
	// TODO Auto-generated destructor stub
}

int CSearch::val(vector<int>&nums,int i)
{
	return i>=0 && i<nums.size()?nums[i]:INT_MIN;
}

int CSearch::findPeakElement(vector<int>& nums)
{
	int l=0;
	int r=nums.size()-1;
	while(l<=r)
	{
		int i = (l+r)/2;
		if(val(nums,i-1) < val(nums,i) && val(nums,i) > val(nums,i+1))
		{
			return i;
		}
		else if(val(nums,i-1) < val(nums,i) && val(nums,i) < val(nums,i+1))
		{//going up
			l = i+1;
		}
		else if(val(nums,i-1) > val(nums,i) && val(nums,i) > val(nums,i+1))
		{//going down
			r = i-1;
		}
		else
		{//valley. any direction.
			r = i-1;
		}
	}
	return -1;
}

bool CSearch::sorted(vector<int>&nums, int l, int r)
{
	if( nums[l] < nums[r])
		return true;
	else if(nums[l] == nums[r])
	{//special case. only if all the same it's sorted.
		for(int i=l+1; i<=r-1; i++)
		{
			if( nums[i] != nums[l])
				return false;
		}
		return true;
	}
	else
	{
		return false;
	}
}
int CSearch::findPivotInRotatedArray(vector<int>&nums)
{
	int l = 0;
	int r = nums.size()-1;
	while(l<r)
	{
		int i = (l+r)/2;
		if( sorted(nums, i,r)) // [i,r] sorted. i could be pivot, but [i+1,r] can't be. search [l,i] range.
		{
			r = i;
		}
		else
		{//[i,r] unsorted. i can't be pivot as it's larger. at most i+1 can be pivot. search [i+1,r] range.
			l = i+1;
		}
	}
	return l;
}

int CSearch::findInRotatedArray(vector<int>& nums, int target)
{
	int l = 0;
	int r = nums.size() - 1;
	while(l<=r)
	{
		int i = (l+r)/2;
		if( target == nums[i] )
			return i;
		//[l,i][i,r]
		if( sorted(nums,l,i))
		{//[l,i] in order.
			if( nums[l] <= target && target < nums[i])
			{
				r = i-1;
			}
			else
			{
				l = i+1;
			}
		}
		else
		{//[i,r] in order.
			if( nums[i] < target && target <= nums[r])
			{
				l = i+1;
			}
			else
			{
				r = i-1;
			}
		}
	}
	return -1;
}
int CSearch::guess(int num)
{
	if(num==6)
		return 0;
	else if(num<6)
		return -1;
	else
		return 1;
}

int CSearch::guessNumber(int n)
{
	int l = 1;
	int r = n;
	while(l<=r)
	{
		int i=l+(r-l)/2;
		switch(guess(n))
		{
		case 0:
			return i;
		case -1:
			l=i+1;
			break;
		case 1:
			r=i-1;
			break;
		}
	}
	assert(false);
	return -1;
}

int CSearch::findRadius(vector<int>&houses, vector<int>&heaters)
{
	sort(heaters.begin(), heaters.end());
	int maxD = 0;
	for(int i=0; i<houses.size();i++)
	{
		auto bound = equal_range(heaters.begin(),heaters.end(),houses[i]);
		int minD = 0;
		if( bound.first == bound.second)
		{//no exact match. cal distance for prev and after. first is insertion point: just larger.
			if(bound.first == heaters.begin())
				minD = *heaters.begin() - houses[i];
			else if(bound.first == heaters.end())
				minD = houses[i] - *(heaters.end()-1);
			else
				minD = min(houses[i] - *(bound.first-1), *bound.first - houses[i]);
		}
		maxD = max(maxD, minD);
	}
	return maxD;
}

bool CSearch::searchOrderedMatrix(vector<vector<int>>& matrix, int target)
{//matrix with asending rows and decending cols.
	int rows = matrix.size();
	if(!rows)
		return false;
	int cols = matrix[0].size();
	int row = 0;
	int col = cols -1;
	while(row < rows && col >= 0)
	{
		if( matrix[row][col] == target)
			return true;
		else if(matrix[row][col] < target)
			row++;
		else
			col--;
	}
	return false;
}

int CSearch::kthSmallestInOrderedMatrix(vector<vector<int>>& A, int k)
{//min heap
	priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>, greater<tuple<int,int,int>>> pq;
	for(int col=0; col<A[0].size(); col++)
	{
		pq.push(make_tuple(A[0][col], 0,col));
	}
	int res = 0;
	while(--k)
	{
		res = get<0>(pq.top());
		int row = get<1>(pq.top());
		int col = get<2>(pq.top());
		pq.pop();
		if( col < A[0].size())
			pq.push(make_tuple(A[row+1][col], row+1,col));
	}
	return res;
}

void CSearch::print(vector<int>& A)
{
	if( A.empty() )
	{
		cout << "[]" << endl;
		return;
	}
	cout << "[" << A[0];
	for(int i=1; i<A.size(); i++)
		cout << "," << A[i];
	cout << "]" << endl;
}

void CSearch::main()
{
	{	//findings:
		//when l==r, no match, all pointers to the next greater element. this is also the insertion point. insert before this element.
		//when there's no next greater element, both points to v.end(). v.end is virtually the next greater index for last element.
		//when there's one match. l=match, r=next greater.
		vector<int> v({1,3,3,3,5,5,6,7,7,7,7,8,9});
		for(int i=0; i<=10; i++)
		{
			auto range = equal_range(v.begin(), v.end(), i);
			auto l = range.first;
			auto r = range.second;
			for(auto it=l; it<=r; ++it)
			{
				if(it == v.end())
					cout << "^,";
				else
					cout << *it << ",";
			}
			cout << endl;
		}
	}

	{//works like magic. supply greater<int>() instead of the default less<int>().
		vector<int> v({9,8,7,7,7,7,6,5,5,3,3,3,1});
		for(int i=0; i<=10; i++)
		{
			print(v);
			cout << "equal_range(" << i << ")" << endl;
			auto range = equal_range(v.begin(), v.end(), i, greater<int>());
			auto l = range.first;
			auto r = range.second;
			for(auto it=l; it<=r; ++it)
			{
				if(it == v.end())
					cout << "^,";
				else
					cout << *it << ",";
			}
			cout << endl;
		}
	}
	{	//use reverse iterator.
		//l points to the last equal element; r points to the element just greater.
		//different behavior from greater<int>(). still makes sense. be need to be careful.
		vector<int> v({9,8,7,7,7,7,6,5,5,3,3,3,1});
		for(int i=0; i<=10; i++)
		{
			print(v);
			cout << "equal_range(" << i << ")" << endl;
			auto range = equal_range(v.rbegin(), v.rend(), i);
			auto l = range.first;
			auto r = range.second;
			for(auto it=l; it<=r; ++it)
			{
				if(it == v.rend())
					cout << "^,";
				else
					cout << *it << ",";
			}
			cout << endl;
		}
	}
	CSearch search;
	{
		vector<int> rotated({4,5,6,7,0,1,2});
		int k = search.findPivotInRotatedArray(rotated);
		int i = search.findInRotatedArray(rotated, 3);
	}
	vector<int> sortedVector({0,2,4,6,8,10});
	for(int i=-2; i<11; i++)
	{
		auto bound = equal_range(sortedVector.begin(), sortedVector.end(),i);
		cout << i << "[" << bound.first - sortedVector.begin() << ","
				<<bound.second - sortedVector.begin() << "]" << endl;
	}
	vector<vector<int>> sortedMatrix({{1,4,7,11,15},{2,5,8,12,19}, {3,6,9,13,22},{4,8,12,16,30}});
	for(int k=1; k<20; k++)
	{
		cout << "kthSmallest: " << search.kthSmallestInOrderedMatrix(sortedMatrix, k) << endl;
	}
	bool found = search.searchOrderedMatrix(sortedMatrix, 2);


	vector<int> houses({1,2,3,8,9,11,13,22});
	vector<int> heaters({2,12,20});
	int radius = search.findRadius(houses,heaters);

	int i = search.guessNumber(10);
	cout << i << endl;
	vector<int> v({1,2,3,1});
	int peak = search.findPeakElement(v);
}
