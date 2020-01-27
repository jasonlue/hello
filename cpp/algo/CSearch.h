/*
 * CSearch.h
 *
 *  Created on: Sep 4, 2018
 *      Author: jason
 */

#ifndef CSEARCH_H_
#define CSEARCH_H_
#include <vector>
using namespace std;
class CSearch {
public:
	CSearch();
	virtual ~CSearch();
	int val(vector<int>&nums, int i);
	int findPeakElement(vector<int>&nums);
	bool sorted(vector<int>&nums, int l, int r);
	int findPivotInRotatedArray(vector<int>&nums);
	int findInRotatedArray(vector<int>& nums, int target);
	int findRadius(vector<int>&houses, vector<int>&heaters);
	int guess(int);
	int guessNumber(int);

	bool searchOrderedMatrix(vector<vector<int>>& matrix, int target);
	int kthSmallestInOrderedMatrix(vector<vector<int>>& matrix, int k);
	static void print(vector<int>& A);
	static void main();
};

#endif /* CSEARCH_H_ */
