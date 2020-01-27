/**
 * CArray.h
 *
 *  Created on: Sep 18, 2018
 *      Author: jason
 */

#ifndef CARRAY_H_
#define CARRAY_H_

#include <vector>
using namespace std;

/// Array data structure related algorithms. mostly for O(n) optimization.
class CArray {
public:
	CArray();
	virtual ~CArray();

	void moveZerosToRight(vector<int>& A);///<insertion sort.
	void group3Nums(vector<int>& A); ///<0,1,2 group them together in the array.
	int unsortedSubArray(vector<int>& A, int& subArrayStart, int&subArrayEnd); //<leetCode 581, min/max
	void prevPermutation(vector<int>& A);
	void nextPermutation(vector<int>& A);

	///sliding window, also idea of dynamic programming. reuse previous work as much as possible.
	vector<int> runningMax(vector<int>& A, int k);///<windows of size k
	vector<int> nextGreaterElement(vector<int>&A);
	bool contains(unordered_map<char,int>&mp);
	string minWindow(string s, string t);

	///runningTotal
	int balancedSubArray(vector<int>& A, int& start, int& end); ///<525, for array of 0 and 1's, find max length subarray with balanced 0&1. the same as zeroSumSubArray
	int zeroSumSubArray(vector<int> A, int& start, int& end);
	int maxLenSubArraySumsToK(vector<int>& A, int k);//325


	///dynamic programming.
	///the key insight is to calculate the subarray ending with i, which can form useful recurrence from [0,i] to [0,i+1] in most of cases.
	int maxAverageSubArray(vector<int>&nums, int k); ///<find K length subArray which has max average.
	int maxSumSubArray(vector<int> A, int& subArrayStart, int& subArrayEnd); ///<LeetCode 53.
	int maxSumSubArrayCircular(vector<int>& A);///<918
	int maxProductSubArray(vector<int> A, int& subArrayStart, int& subArrayEnd); ///<LeetCode 152.
	int subArrayBitwiseORs(vector<int>& A); ///<898: return unique numbers of results for all possible subarray item ors.
	int sumSubArrayMins(vector<int>& A); ///<907
	int minSubArrayLen(int s, vector<int>& nums);///<209 n positive ints and positive s. find minLen subarray which sum>=s

	///count permutations.
	int numSubArrayProductLessThanK(vector<int>&A,int k);///<windows of size x so that product <k.
	int numSubArayBoundedMax(vector<int>& A, int L, int R); ///<795: number of subarrays max in [L,R]


public:///testcases.
	static void permutation();
	static void moveZerosToRight();
	static void group3Nums();
	static void unsortedSubArray();
	static void runningMax();
	static void minWindow();
	static void balancedSubArray();
	static void zeroSumSubArray();
	static void maxSumSubArray();
	static void maxProductSubArray();
	static void numSubArrayProductLessThanK();
	static void print(vector<int>& A);
	static void print(vector<int>& A, int l, int r);
	static void main();
};

#endif /* CARRAY_H_ */
