/*
 * CAlgo.h
 *
 *  Created on: Aug 27, 2018
 *      Author: jason
 */

#ifndef CALGO_H_
#define CALGO_H_
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
using namespace std;

struct TreeNode
{
	int val;
	TreeNode* left;
	TreeNode* right;
	TreeNode(int x) : val(x), left(NULL),right(NULL){}
	static int Val(TreeNode* t){return t? t->val : 0;}
	static TreeNode* Left(TreeNode* t) {return t? t->left : NULL;}
	static TreeNode* Right(TreeNode*t){return t? t->right : NULL;}
};
struct Interval
{
	int start;
	int end;
	Interval() : start(0), end(0){};
	Interval(int s, int e) : start(s), end(e){};
};

class CAlgo {
public:
	CAlgo();
	virtual ~CAlgo();
	vector<int> twoSum(vector<int>&nums, int target);
	vector<int> twoSum2(vector<int>nums, int target);

	//easy
	int lengthOfLongestSubstring(string s);
	int numJewelsInStones(string J, string S);
	TreeNode* mergeTrees(TreeNode* t1, TreeNode* t2);
	TreeNode* mergeTrees2(TreeNode* t1, TreeNode* t2);
	TreeNode* heapToTree(vector<int> v);
	vector<int> treeToHeap(TreeNode* t);

	bool oneEditAway(string a, string b);
	bool oneDeleteAway(string a, string b);
	bool oneReplaceAway(string a, string b);

	int reverse(int x);
	bool isPalindrome(int x);
	int romanToInt(string s);
	//hard. O(m+n).
	double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2);

	vector<Interval> insertInterval(vector<Interval>& intervals, Interval newInterval);
	vector<Interval> insertInterval2(vector<Interval>& intervals, Interval newInterval);

	static void main();
};

#endif /* CALGO_H_ */
