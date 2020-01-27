/*
 * CArray.cpp
 *
 *  Created on: Sep 18, 2018
 *      Author: jason
 */
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <set>
#include <deque>
#include <algorithm>
#include <iostream>
#include "CArray.h"
#include <assert.h>
using namespace std;

CArray::CArray() {
	// TODO Auto-generated constructor stub

}

CArray::~CArray() {
	// TODO Auto-generated destructor stub
}

void CArray::nextPermutation(vector<int>& A)
{
	int k=A.size()-2;
	while(k>=0 && A[k] >= A[k+1])
		k--;
	if( k< 0 )
	{
		reverse(A.begin(), A.end());
	}
	else
	{
		auto it = upper_bound(A.rbegin(), A.rbegin()+A.size()-(k+1), A[k]);
		int l = A.size()-1 -(it - A.rbegin());
//		cout << "l=" << l << endl;
		assert(l>k && A[l] > A[k]);
		swap(A[k],A[l]);
		reverse(A.begin()+k+1, A.end());
	}
}

void CArray::prevPermutation(vector<int>& A)
{	//just the opposite of nextPermutation
	//(1) find largest k so that A[k]>A[k+1].
	//(2) [k+1,n-1] ascending order.
	//(3) find l so within [k+1,n-1] so that A[l]<A[k]
	//(4) swap A[k],A[l]
	//(5) reverse [k+1,n-1]
	int n = A.size();
	int k=n-2;
	while(k>=0 && A[k]<=A[k+1])
		k--;
	if(k<0)
	{
		reverse(A.begin(), A.end());
	}
	else
	{	//find l so that A[l] < A[k] with [k+1,n-1] sorted ascending.
		//A[k] is surely not in this range. so lower_bound = upper_bound = next Larger.
		auto it = lower_bound(A.begin()+k+1, A.end(), A[k]);
		--it; //get the smaller one.
		int l = it - A.begin();
		assert(l>k && A[l] < A[k]);
		swap(A[k], A[l]);
		reverse(A.begin()+k+1, A.end());
	}


}
void CArray::moveZerosToRight(vector<int>& A)
{	//fastest: make elements to the right position once for all. not bubbling.
	//because we know the zeros have to go right. no need to swap or track it.
	//just move elements to left and fill the rest with zeros.
	int i=0;
	for(int j=0; j<(int)A.size(); j++)
	{	//move all non zeros to the right place once.
		if(A[j] != 0 )
		{
			A[i] = A[j];
			i++;
		}
	}
	for(;i<(int)A.size(); i++)
	{
		A[i] = 0;
	}
}

void CArray::group3Nums(vector<int>& A) //-1, 0,1 group them together in the array.
{	//idea: going through the array. swap with left or right.
	int l=0;//tracks end of -1 group [0,l) [l,r], (r,n-1] 3 groups. during the process, [i,l] unknown.
	int r = A.size()-1;// tracks start of 1 group.
	int i=0;
	while(i<=r) //when i==r, A[i] could still be 0. for last swap.
	{
		if( 0 ==A[i])
		{//low
			swap(A[l],A[i]); //could i still be 0 after swap? or 1 after swap? can't be.
			l++;
			i++;
		}
		else if( 1 == A[i])
		{//high, the one swapped to i could still be 0 or 1. so i cannot increase.
			swap(A[i],A[r]);
			r--;
		}
		else
		{//middle
			i++;
		}
	}
}
int CArray::unsortedSubArray(vector<int>& A, int& subArrayStart, int&subArrayEnd)
{//(l,r) is the subArray
	int l=0; int r = A.size()-1;
	while(l<r && A[l]<=A[l+1])
		l++;
	if(l>=r)
		return 0; //fully sorted.

	while(A[r]>=A[r-1])
		r--;

	//(l,r) not sorted. but [0,l] may overlap with (l,r); [r,A.size()-1] may overlap with (l,r) too.
	//backtrack until they don't overlap.
	//must include l and r to make sure A[r]>=A[l] when only 2 elements exist.
	int minVal = A[l];
	int maxVal = A[l];
	for(int i=l+1; i<=r;i++)
	{
		if(A[i]<minVal)
			minVal = A[i];
		if(A[i]>maxVal)
			maxVal = A[i];
	}

	//lr can go just out of range. to indicate no sorted part.
	while(l>=0 && A[l] > minVal)
		l--;
	while(r<A.size() && A[r]<maxVal)
		r++;
	subArrayStart = l+1;
	subArrayEnd = r-1;
	return r-l-1;
}

int CArray::balancedSubArray(vector<int>& A, int& start, int& end) //525, for array of 0 and 1's, find max length subarray with balanced 0&1. the same as zeroSumSubArray
{//to account for empty, has to internally use (start,end], start=-1.
	unordered_map<int,int> sumMp;
	sumMp[0] = -1; //empty.
	int runningTotal = 0;
	int maxLen = 0;
	for(int i=0; i<A.size(); i++)
	{
		runningTotal += A[i]? 1 : -1;
		if( sumMp.count(runningTotal))
		{//dup.
			if( i - sumMp[runningTotal] > maxLen )
			{
				start = sumMp[runningTotal]+1; //make it inclusive. [start,end]
				end = i;
				maxLen = end - start+1;
			}
		}
		else
			sumMp[runningTotal] = i;
	}
	return maxLen;
}

int CArray::zeroSumSubArray(vector<int> A, int& start, int& end)
{	//calculate running total. find repetition of the running total. find the longest.
	int runningTotal = 0;
	unordered_map<int,int> sumMp;
	sumMp[0] = -1; //start = -1. (start,end] internally to include empty range.
	int maxLen = 0;
	for(int i=0; i<A.size(); i++)
	{
		runningTotal += A[i];
		if(sumMp.count(runningTotal))
		{//exists.
			if( i-sumMp[runningTotal] > maxLen )
			{
				start = sumMp[runningTotal]+1;
				end = i;
				maxLen = end - start+1;
			}
		}
		else
		{
			sumMp[runningTotal] = i;
		}
	}
	return maxLen;
}

bool  CArray::contains(unordered_map<char,int>& mp)
{//if all count <=0, the range contains string originally represented by mp.
	for(auto it = mp.begin(); it != mp.end(); ++it)
	{
		if(it->second > 0)
			return false;
	}
	return true;
}

string  CArray::minWindow(string s, string t)
{
	if(t.empty())
		return "";
	unordered_map<char,int> mp;
	for(auto ch:t)
		mp[ch]++;
	int i=0;//start of the range
	//find the start of the intial range.
	while(i<s.length() && !mp.count(s[i]))
	{
		i++;
	}
	int j=i;//end of the range.
	//now find the range contains mp.
	while(j<s.length() )
	{
		if(mp.count(s[j]))
			mp[s[j]]--;
		if( contains(mp))
			break;
		j++;
	}
	if(j>=s.length())
		return ""; //doesn't contain.

	while(i<j && (!mp.count(s[i])||mp[s[i]]<0))
	{
		if(mp.count(s[i]))
			mp[s[i]]++;
		i++;
	}
	//now [i,j] is the intial range.
	int len = j-i+1;
	int minLen = len;
	int start = i;
	int end = j;
	for(j++;j<s.length(); j++)
	{
		if(mp.count(s[j]))
			mp[s[j]]--;

		while(i<j && (!mp.count(s[i])||mp[s[i]]<0))
		{
			if(mp.count(s[i]))
				mp[s[i]]++;
			i++;
		}

		len = j-i+1;
		if( len < minLen)
		{
			minLen = len;
			start = i;
			end = j;
		}
		minLen = min(minLen,len);
	}
	return s.substr(start,end-start+1);
}

vector<int> CArray::runningMax(vector<int>& A, int k)
{	//O(N) no looking back. max of running K elements.
	//deque maintain only max elements, it's actually a monotonous decrease stack to maintain next larger
	vector<int> res(A.size());
	deque<int> dq; //monotonous descending(from bottom to top) stack.
	for(int i=0; i<A.size(); i++)
	{
		//remove index out of running range of k
		if( !dq.empty() && dq.front() <= i-k)
			dq.pop_front();
		//check the back. remove anything smaller than A[k]. which cannot be max with A[k] in the range.
		while(!dq.empty() && A[dq.back()] <= A[i])
			dq.pop_back();
		//put me in.
		dq.push_back(i);
		res[i] = A[dq.front()];
	}
	return res;
}

vector<int> CArray::nextGreaterElement(vector<int>&A)
{	//many similar ways to use this stack.
	//(1)push from left to right or from right to left.
	//(2) push larger or smaller ones to stack. for stack to be increasing or decreasing.
	//(3)
	vector<int> next(A.size());
	stack<int> stk;//monotonous descending stack. put stack horizontally to understand it easily.
	unordered_map<int,int> mp;
	for(int i=0; i<A.size(); i++)
	{
		while(!stk.empty() && stk.top() > A[i])
		{
			mp[stk.top()] = A[i]; //A[i] is top()'s next greater, elements between top and A[i] ar
			stk.pop();
		}
		stk.push(A[i]);
	}
	for(int i=0; i<A.size(); i++)
	{
		next[i] = mp.count(A[i]) ? mp[A[i]] : -1;
	}
	return next;
}

int CArray::numSubArrayProductLessThanK(vector<int>&A,int k)//windows of size x so that product <k.
{	//all numbers positive.
	//count [i,j] as j-i+1 if [i,j] product < k.
	if(A.empty())
		return 0;
	int count = A[0]<k?1:0;
	int product = A[0];
	int i=0;
	for(int j=1; j<A.size(); j++)
	{
		product *= A[j];
		while(i<=j && product >= k)
			product /= A[i++];
		count += j-i+1;
	}
	return count;
}
int CArray::maxSumSubArray(vector<int> A, int& subArrayStart, int& subArrayEnd)
{	//the idea: use dynamic programming. need to reuse previous calculation to achieve O(n)
	//calculate maxSum for ranges ends with i, suppose [n,i]
	//then maxSum for ranges ends with i+1 will be [n,i+1] or [i+1] if [n,i]<0
	//the globalMaxSum will be the max of these ranges ending with 0 to n.
	if(A.empty())
		return 0;

	int globalMaxSum = A[0];
	subArrayStart = subArrayEnd = 0; //set the first element to start with.
	int maxSumEndsWithI = A[0];
	int maxSumEndsWithIStart = 0;
	for(int i=1; i<(int)A.size(); i++)
	{
		if(maxSumEndsWithI > 0)
		{
			maxSumEndsWithI += A[i];
		}
		else
		{
			maxSumEndsWithI = A[i];
			maxSumEndsWithIStart = i;
		}
		if(maxSumEndsWithI > globalMaxSum)
		{	//update globalMaxSum
			globalMaxSum = maxSumEndsWithI;
			subArrayStart = maxSumEndsWithIStart;
			subArrayEnd = i;
		}
	}
	return globalMaxSum;
}

int CArray::maxProductSubArray(vector<int> A, int& subArrayStart, int& subArrayEnd)
{	//similar as maxSumSubArray, but special: maxSum -> maxProduct, maxNegProduct, 0 Product
	//on A[i], 0, then product 0; >0; multiply by maxProduct; <0, multiple by maxNegProduct
	if(A.empty())
		return 0;
	int globalMaxProduct = A[0];
	int maxProductEndsWithI = A[0];
	int maxProductEndsWithIStart= 0;
	int minProductEndsWithI = A[0];
	int minProductEndsWithIStart = 0;
	subArrayStart = 0;
	subArrayEnd = 0;

	for(int i=1; i<(int)A.size(); i++)
	{
		if( A[i] < 0 )
		{
			swap(maxProductEndsWithI,minProductEndsWithI);
			swap(maxProductEndsWithIStart, minProductEndsWithIStart);
		}

		if( maxProductEndsWithI * A[i] > A[i] )
		{
			maxProductEndsWithI *= A[i];
		}
		else
		{
			maxProductEndsWithIStart = i;
			maxProductEndsWithI = A[i];
		}
		if( minProductEndsWithI * A[i] < A[i] )
		{
			minProductEndsWithI *= A[i];
		}
		else
		{
			minProductEndsWithIStart = i;
			minProductEndsWithI = A[i];
		}

		if(maxProductEndsWithI > globalMaxProduct)
		{
			globalMaxProduct = maxProductEndsWithI;
			subArrayStart = maxProductEndsWithIStart;
			subArrayEnd = i;
		}
	}
	return globalMaxProduct;
}

int CArray::subArrayBitwiseORs(vector<int>& A) //898: return unique numbers of results for all possible subarray item ors.
{	//brute force. will be O(n^3)
	//reuse previous work will reduce to O(n^2).
	//reduce immediate results will reduce to O(n), O(32n)
	//the first key is gained from maxSumSubArray: always think subarray ends with i instead of start with i.
	//ends with i can generate useful recurrence on i+1.
	//here [0,j] subarray will generate middle results dp[0,j]; for j+1. dp[0,j+1] will be all middle results | A[j+1], plus A[j+1]
	//we only use previous result. so 1d array to store temp results.
	unordered_set<int> st;
	/*
	vector<int> dp(A.size(),0);
	for(int j=0; j<A.size(); j++)
	{
		for(int i=0; i<j; i++)
		{//[i,j]
			dp[i] |= A[i];
			st.insert(dp[i]);
		}
		dp[j] = A[j];
		st.insert(dp[j]);
	}
	*/
	//however, it uses prev step results but still O(n^2) due to i from 0-j.
	//give the nature of Or, we can expect there are a lot of dups in dp[0,j]. and these dups will create dups for j round.
	//so the key is to remove the dups. so use set instead of vector. to use set, we lose one to one relationship from j-1 round to j.
	//so we need to save the results to a temp to be used for the next round.
	//use unordered_set if 2x faster than set.
	//set is at most 32 long.
	unordered_set<int> dp;
	for(int j=0; j<A.size(); j++)
	{
		unordered_set<int> t;
		for(auto i:dp)
		{
			t.insert(i|A[j]);
			st.insert(i|A[j]);
		}
		t.insert(A[j]);
		st.insert(A[j]);
		swap(dp,t);
	}

	return st.size();
}

int CArray::numSubArayBoundedMax(vector<int>& A, int L, int R)
{	//https://www.youtube.com/watch?v=WhKrPDS1VAg
	//(1) like all SubArray Dp problems. think subarrays ends with index i.
	//(2) for subarrays ends with index i. a specific subarray (start,end],
	//where start points to i where A[i] > R (bad) anything subarray starts start or before will have max >R
	// and end points to the last index with value in [L,R]. end - start is the count for subarrays ends with i.
	int start = -1;
	int end = -1;
	int count = 0;
	for(int i=0; i<A.size(); i++)
	{
		if( A[i] > R)
		{	//reset the range
			start = end = i;
		}
		else if( A[i] >= L)
		{
			end = i;
		}
		else
		{	//do nothing for smaller values.
		}
		count += end - start;
	}
	return count;
}

bool isValid(vector<int>& bits, int l, int r)
{
	if(l>r)
		return true;
	if(!bits[l])
	{
		return isValid(bits, l+1,r);
	}
	else
	{
		if(l==r) //only one bit.
			return false;
		return isValid(bits, l+2,r);
	}
}
bool isOneBitCharacter(vector<int>& bits) {
	if(bits.size()<=1)
		return true;
	int i=bits.size()-2;
	if(0==bits[i])
		return true; //00 invalid.
	else
	{//10
		return !isValid(bits,0,i-1);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TEST CASES
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CArray::print(vector<int>& A, int l, int r)
{
	if( A.empty() || l > r)
	{
		cout << "[]" << endl;
		return;
	}
	cout << "[" << A[l];
	for(int i=l+1; i<=r; i++)
		cout << "," << A[i];
	cout << "]" << endl;
}

void CArray::print(vector<int>& A)
{
	print(A,0,A.size()-1);
}
void CArray::permutation()
{
	cout << "\n" <<__FUNCTION__ << "()" << endl;
	CArray ar;
	vector<vector<int>> A
	({
		{1,2},
		{1,4,3,2},
		{1,2,3},
		{0,1},
		{1,1,1},
		{0,0,0,0,0,1,2,3,4,5,6,7,8,9},
	});
	for(auto a: A)
	{
		for(int i=0; i<2; i++)
		{
			random_shuffle(a.begin(), a.end());
			ar.print(a);
			cout << "nextPermutation()" << endl;
			ar.nextPermutation(a);
			ar.print(a);
			cout << "prevPermutation()" << endl;
			ar.prevPermutation(a);
			ar.print(a);
		}
	}
}
void CArray::moveZerosToRight()
{
	cout << "\n" <<__FUNCTION__ << "()" << endl;
	CArray ar;
	vector<vector<int>> A
	({
		{},
		{0},
		{1},
		{0,0},
		{1,1,1},
		{0,0,0,0,0,1,2,3,4,5,6,7,8,9},
		{-5,-4,0,-3,-2,-1,0,1,2,3,0,4,5}
	});
	for(auto a: A)
	{
		for(int i=0; i<2; i++)
		{
			random_shuffle(a.begin(), a.end());
			ar.print(a);
			ar.moveZerosToRight(a);
			ar.print(a);
		}
	}
}
void CArray::group3Nums()
{
	cout << "\n" <<__FUNCTION__ << "()" << endl;
	CArray ar;
	vector<vector<int>> A
	({
		{0,0,0,0,0,1,1,1,-1,-1,-1,-1,-1,-1},
		{},
		{0},
		{1},
		{0,0},
		{-1,1,1},
	});
	for(auto a: A)
	{
		for(int i=0; i<2; i++)
		{
			random_shuffle(a.begin(), a.end());
			ar.print(a);
			ar.group3Nums(a);
			ar.print(a);
		}
	}
}
void CArray::unsortedSubArray()
{
	cout << "\n" <<__FUNCTION__ << "()" << endl;
	CArray ar;
	vector<vector<int>> A
	({
		{},
		{0},
		{1},
		{2,1},
		{1,1,1},
		{0,0,0,0,0,1,2,3,4,5,6,7,8,9},
		{-5,-4,0,-3,-2,-1,0,1,2,3,0,4,5}
	});
	for(auto a: A)
	{
		for(int i=0; i<2; i++)
		{
			random_shuffle(a.begin(), a.end());
			ar.print(a);
			int start=0, end=-1;
			int len = ar.unsortedSubArray(a, start, end);
			ar.print(a, start, end);
		}
	}
}

void CArray::runningMax()
{
	cout << "\n" <<__FUNCTION__ << "()" << endl;
	CArray ar;
	vector<vector<int>> A
	({
		{},
		{0},
		{1},
		{0,0},
		{1,1,1},
		{0,0,0,0,0,1,2,3,4,5,6,7,8,9},
		{-5,-4,0,-3,-2,-1,0,1,2,3,0,4,5}
	});
	for(auto a: A)
	{
		for(int i=0; i<2; i++)
		{
			random_shuffle(a.begin(), a.end());
			ar.print(a);
			for(int k=0; k<=a.size(); k++)
			{
				cout << "runningMax(" << k << ")" << endl;
				vector<int> b = ar.runningMax(a,k);
				ar.print(b);
			}
		}
	}
}

void CArray::minWindow()
{
	cout << "\n" <<__FUNCTION__ << "()" << endl;
	CArray ar;

	vector<vector<string>> A
	({
		{"a","a"},
		{"ab","a"},
		{"ab","ab"},
		{"ADOBECODEBANC", "ABC"},
	});
	for(auto a: A)
	{
		for(int i=0; i<2; i++)
		{
			cout << "minWindow(" << a[0] << "," << a[1] << ")="<< ar.minWindow(a[0], a[1]) << endl;
		}
	}
}

void CArray::balancedSubArray()
{
	cout << "\n" <<__FUNCTION__ << "()" << endl;
	CArray ar;
	vector<vector<int>> A
	({
		{},
		{0},
		{1},
		{0,0},
		{0,1},
		{0,1,1},
		{1,1,1},
		{0,0,0,0,0,1,1,1,1,1},
		{0,0,0,0,0,1,1,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,1,1,1,1,1,1},
	});
	for(auto a: A)
	{
		for(int i=0; i<2; i++)
		{
			random_shuffle(a.begin(), a.end());
			ar.print(a);
			int start=0, end=-1;
			int maxLen = ar.balancedSubArray(a,start,end);
			ar.print(a,start,end);
		}
	}

}
void CArray::zeroSumSubArray()
{
	cout << "\n" <<__FUNCTION__ << "()" << endl;
	CArray ar;
	vector<vector<int>> A
	({
		{},
		{0},
		{1},
		{2,1},
		{1,1,1},
		{0,0,0,0,0,1,2,3,4,5,6,7,8,9},
		{-5,-4,0,-3,-2,-1,0,1,2,3,0,4,5}
	});
	for(auto a: A)
	{
		for(int i=0; i<2; i++)
		{
			random_shuffle(a.begin(), a.end());
			ar.print(a);
			int start = 0, end = -1;
			int len = ar.zeroSumSubArray(a, start, end);
			ar.print(a, start, end);
		}
	}
}
void CArray::maxSumSubArray()
{
	cout << "\n" <<__FUNCTION__ << "()" << endl;
	CArray ar;
	vector<vector<int>> A
	({
		{},
		{0},
		{1},
		{2,1},
		{1,1,1},
		{0,0,0,0,0,1,2,3,4,5,6,7,8,9},
		{-5,-4,0,-3,-2,-1,0,1,2,3,0,4,5}
	});
	for(auto a: A)
	{
		for(int i=0; i<2; i++)
		{
			random_shuffle(a.begin(), a.end());
			ar.print(a);
			int start = 0, end = -1;
			int len = ar.maxSumSubArray(a, start, end);
			ar.print(a, start, end);
		}
	}
}
void CArray::maxProductSubArray()
{
	cout << "\n" <<__FUNCTION__ << "()" << endl;
	CArray ar;
	vector<vector<int>> A
	({
		{},
		{0},
		{1},
		{2,1},
		{1,1,1},
		{0,0,0,0,0,1,2,3,4,5,6,7,8,9},
		{-5,-4,0,-3,-2,-1,0,1,2,3,0,4,5}
	});
	for(auto a: A)
	{
		for(int i=0; i<2; i++)
		{
			random_shuffle(a.begin(), a.end());
			ar.print(a);
			int start = 0, end = -1;
			int len = ar.maxProductSubArray(a, start, end);
			ar.print(a, start, end);
		}
	}
}
void CArray::numSubArrayProductLessThanK()
{

}

void CArray::main()
{
	minWindow();
	permutation();
	moveZerosToRight();
	group3Nums();
	runningMax();
	unsortedSubArray();
	balancedSubArray();
	zeroSumSubArray();
	maxSumSubArray();
	maxProductSubArray();
	numSubArrayProductLessThanK();

	vector<int> z({1,0});
	bool yes = isOneBitCharacter(z);
}

