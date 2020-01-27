/*
 * CAlgo.cpp
 *
 *  Created on: Aug 27, 2018
 *      Author: jason
 */
#include <assert.h>
#include "CAlgo.h"
#include <algorithm>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <stack>
#include <tuple>
#include <climits>
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

CAlgo::CAlgo() {
	// TODO Auto-generated constructor stub

}

CAlgo::~CAlgo() {
	// TODO Auto-generated destructor stub
}

vector<int> CAlgo::twoSum(vector<int>&nums, int target)
{
	vector<int> result;
	if( nums.size() >= 2)
	{
		sort(nums.begin()+1, nums.end());
		for(vector<int>::iterator it = nums.begin(); it != nums.end(); ++it)
		{
			int a = *it;
			int b = target - a;
			vector<int>::iterator itB = lower_bound(it+1, nums.end(), b);
			//assume one solution.
			if( itB != nums.end() && *itB == b)
			{
				result.push_back(it - nums.begin());
				result.push_back(itB - nums.begin());
				break;
			}
		}
	}
	return result;
}

vector<int> CAlgo::twoSum2(vector<int>nums, int target)
{
	vector<int> result;
	unordered_map<int,int> numMap;
	for(int i=0; i<(int)nums.size(); i++)
	{
		auto a = nums[i];
		auto b = target - nums[i];
		unordered_map<int,int>::iterator itB = numMap.find(b);
		if( itB != numMap.end() && numMap[b] != i)
		{
			result.push_back(i);
			result.push_back(itB->second);
			break;
		}
		numMap[a] = i;
	}
	return result;
}

int CAlgo::lengthOfLongestSubstring(string s)
{
	unordered_map<char, int> uniqueChars;
	int start = 0;
	int maxLength = 0;
	for(int i=0; i<(int)s.length(); i++)
	{
		if( uniqueChars.find(s[i]) != uniqueChars.end() )
		{//remove all chars in the map with index <= the duplicate index.
			int end = uniqueChars[s[i]];
			maxLength = max(maxLength, i - start);
			for(int j=start; j<=end; j++)
			{
				uniqueChars.erase(s[j]);
			}
			start = end+1;
		}
		uniqueChars[s[i]] = i;
	}
	return maxLength;
}

int CAlgo::numJewelsInStones(string J, string S)
{
	unordered_set<char> jewels;
	for(int i=0; i<J.size(); i++)
		jewels.insert(J[i]);

	int n = 0;
	for(int j=0; j<S.size(); j++)
	{
		if( jewels.find(S[j]) != jewels.end())
			n++;
	}
	return n;
}

TreeNode* CAlgo::mergeTrees(TreeNode* t1, TreeNode* t2)
{
	if( !t1 && !t2)
		return NULL;

	TreeNode* t = new TreeNode(TreeNode::Val(t1) + TreeNode::Val(t2));
	t->left = mergeTrees(TreeNode::Left(t1), TreeNode::Left(t2));
	t->right = mergeTrees(TreeNode::Right(t1), TreeNode::Right(t2));
	return t;
}
TreeNode* CAlgo::mergeTrees2(TreeNode* t1, TreeNode* t2)
{
	if( !t1 && !t2)
		return NULL;
	TreeNode* t = new TreeNode(TreeNode::Val(t1) + TreeNode::Val(t2));
	stack<tuple<TreeNode*, TreeNode*, TreeNode*>> s;
	s.push(make_tuple(t,t1,t2));
	while(!s.empty())
	{
		auto tup = s.top();
		s.pop();
		TreeNode* a = get<0>(tup);
		TreeNode* b = get<1>(tup);
		TreeNode* c = get<2>(tup);
		a->left = new TreeNode(TreeNode::Val(TreeNode::Left(b)) + TreeNode::Val(TreeNode::Left(c)));
		a->right = new TreeNode(TreeNode::Val(TreeNode::Right(b)) + TreeNode::Val(TreeNode::Right(c)));
		if( TreeNode::Left(b) || TreeNode::Left(c))
		{
			s.push(make_tuple(a->left, TreeNode::Left(b), TreeNode::Left(c)) );
		}
		if( TreeNode::Right(b) || TreeNode::Right(c) )
		{
			s.push(make_tuple(a->right, TreeNode::Right(b), TreeNode::Right(c)) );
		}
	}
	return t;
}

TreeNode* CAlgo::heapToTree(vector<int> v)
{
	if( v.empty() )
			return NULL;
	TreeNode* r = new TreeNode(v[0]);
	deque<TreeNode*> nodes;
	nodes.push_back(r);
	deque<TreeNode*>::iterator it;
	int i=1;
	while( !nodes.empty() && i < v.size())
	{
		TreeNode* t = nodes.front();
		nodes.pop_front();
		t->left = new TreeNode(v[i++]);
		if( i >= v.size())
			break; //end of heap.
		nodes.push_back(t->left);
		t->right = new TreeNode(v[i++]);
		nodes.push_back(t->right);
	}
	return r;
}
vector<int> CAlgo::treeToHeap(TreeNode* r)
{
	vector<int> v;
	if(!r)
		return v;
	v.push_back(r->val);
	deque<TreeNode*> nodes;
	nodes.push_back(r);
	while(!nodes.empty())
	{
		TreeNode* t = nodes.front();
		nodes.pop_front();
		if( !t->left )
			break;
		v.push_back(t->left->val);
		nodes.push_back(t->left);
		if(!t->right)
			break;
		v.push_back(t->right->val);
		nodes.push_back(t->right);
	}
	return v;
}
double CAlgo::findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2)
{
	int i = (nums1.size() + nums2.size())/2;
	int j = (nums1.size() + nums2.size())/2;
	return 0;
}

bool CAlgo::oneEditAway(string a, string b)
{
	return oneDeleteAway(a,b) || oneReplaceAway(a,b);
}

bool CAlgo::oneDeleteAway(string a, string b)
{
	if( a.length() > b.length())
		swap(a,b);
	if( b.length() != a.length()+1 )
		return false;
	for(int i=0; i<(int)b.length(); i++)
	{
		string x = b.substr(0,i) + b.substr(i+1);
		if(a == x)
			return true;
	}
	return false;
}
bool CAlgo::oneReplaceAway(string a, string b)
{
	if( a.length() != b.length())
		return false;
	bool diffOnce = false;
	for(int i=0; i<(int)a.length(); i++)
	{
		if( a[i] != b[i])
		{
			if( diffOnce)
				return false;
			diffOnce = true;
		}
	}
	return diffOnce;
}

bool CAlgo::isPalindrome(int x)
{//no overflow situation.
	if( x < 0 )
		return false;

	int y = 0;
	int z = x;
	while(x)
	{
		y *= 10;
		y += x %10;
		x /= 10;
	}
	return z == y;
}

int CAlgo::reverse(int z)
{//handle overflow carefully and return 0 on overflow. the right way to detect overflow.
//y and d same sign. % returns sign.
	int y = 0;
	int x = z;
	while(x)
	{
		int d = x % 10;
		if( (y > 0 && y > INT_MAX/10) || (y < 0 && y < INT_MIN/10) )
			return 0; //overflow.
		y *= 10;
		if( (y > 0 && y > INT_MAX-d)|| (y < 0 && y < INT_MIN-d) )
			return 0; //overflow.
		y += d;
		x /= 10;
	}
	return y;
}

int CAlgo::romanToInt(string s)
{//assume s is valid representation of roman numbers.
	unordered_map<char,int> m({{'I',1}, {'V',5}, {'X', 10}, {'L', 50}, {'C', 100}, {'D',500}, {'M', 1000}});
	int y = 0;
	for(int i=0; i<s.length(); i++)
	{
		int x = m[s[i]];
		//peek next.
		if( i+1 < s.length() && m[s[i+1]] > x)
			x = -x;
		y += x;
	}
	return y;
}

vector<Interval> CAlgo::insertInterval(vector<Interval>& intervals, Interval newInterval)
{//intervals are sorted by start and non-overlapping. newInterval may overlap with existing intervals.
	vector<Interval> r;
	int i = 0;
	while(i < intervals.size() && intervals[i].end < newInterval.start)
	{
		r.push_back(intervals[i++]);
	}
	while(i<intervals.size() && intervals[i].start <= newInterval.end)
	{
		newInterval.start = min(newInterval.start, intervals[i].start);
		newInterval.end = max(newInterval.end, intervals[i].end);
		i++;
	}
	r.push_back(newInterval);
	while(i<intervals.size())
		r.push_back(intervals[i++]);

	return r;
}

bool compare(const Interval& l, const Interval& r)
{
	cout << "l(" << l.start << "," << l.end << ");" << "r(" << r.start << "," << r.end << ")" << endl;
	return l.end < r.start;
}

vector<Interval> CAlgo::insertInterval2(vector<Interval>& intervals, Interval newInterval)
{
	auto range = equal_range(intervals.begin(), intervals.end(), newInterval, compare);
	auto l = range.first;
	auto r = range.second;
	if( l == r)
	{
		intervals.insert(r, newInterval);
	}
	else
	{
		--r;
		r->end = max(newInterval.end, r->end);
		r->start = min(newInterval.start, l->start);
		intervals.erase(l,r);
	}
	return intervals;
}

int strStr(string haystack, string needle)
{
	if( needle.empty())
		return 0;
	int max = (int)haystack.length()-(int)needle.length();
	for(int i=0; i<=max; i++)
	{
		bool match = true;
		for(int j=0; j<needle.length(); j++)
		{
			if( haystack[i+j] != needle[j] )
			{
				match = false;
				break;
			}
		}
		if( match)
			return i;
	}
	return -1;
}

bool repeatedSubstringPattern(string s)
{
	int len = s.length();
	for(int i=2; i<=len; i++)
	{
		if( len % i )
			continue; //not divisible.

		int subLen = len / i;
		bool allMatch = true;
		for(int j=1; j<i; j++)
		{
			bool twoMatch = true;
			for(int k=0; k<subLen; k++)
			{
				if( s[j*subLen+k] != s[k])
				{
					twoMatch = false;
					break;
				}
			}
			if(!twoMatch)
			{
				allMatch = false;
				break;
			}
		}
		//match or not this round?
		if( allMatch )
			return true;
	}
	return false;
}

string countAndSay(int n)
{//1,11,21,1211,111221,312211,13112221,1113213211
	string s = "1";
	for(int i=1; i<n; i++)
	{
		string t;
		char ch = s[0];
		int count = 1;
		for(int j=1; j<s.length(); j++)
		{
			if( s[j] == ch)
			{
				count++;
			}
			else
			{
				t += to_string(count);
				t.push_back(ch);
				ch = s[j];
				count = 1;
			}
		}
		t += to_string(count);
		t.push_back(ch);
		s = t;
	}
	return s;
}
int getSum(int a, int b)
{
	int s = 0;
	int digitC = 0;
	for(int i=0; i<32; i++)
	{
		int digitA = a & 1;
		int digitB = b & 1;
		int digit = digitA^digitB^digitC;
		digitC = (digitA&digitB) | (digitB&digitC) | (digitC&digitA);
		s |= digit << i;
		if(!a&&!b&&!digitC)
			break;
		a >>=1;
		b >>=1;
	}
	return s;
}

int getSum2(int a, int b)
{
	return b ? a : getSum2(a^b, (a&b)<<1);
}



string frequencySort(string s)
{
	int maxFreq = 0;
	unordered_map<char,int> freqMap;
	for(auto ch : s)
	{
		freqMap[ch]++;
		maxFreq = max(maxFreq,freqMap[ch]);
	}
	vector<vector<pair<char,int>>> freqVec(maxFreq);
	for(auto& i : freqMap)
		freqVec[i.second].push_back(i);

	stringstream ss;
	for(int i=freqVec.size()-1; i>=0; i--)
	{
		for(int j=0; j<freqVec[i].size();j++)
		{
			ss << string(freqVec[i][j].first, freqVec[i][j].second);
		}
	}
}

int longestPalindrome(string s)
{
    int len = 0;
    for(int i=0; i<s.length();i++)
    {//build palindrome around i.
        //i as center, odd
        int oddLen = 1;

        for(int j=1;i+j<s.length() && i-j>=0; j++)
        {
            if(s[i-j] == s[i+j])
                oddLen+=2;
            else
                break;
        }
        len = max(len,oddLen);
        //i as left, even.
        int evenLen = 0;
        for(int j=0; i-j>=0 && i+j+1<s.length(); j++)
        {
            if(s[i-j] == s[i+j+1])
                evenLen += 2;
            else
                break;
        }
        len = max(len,evenLen);
    }
    return len;
}

string toEnglish(int n)
{//3 digits.
	vector<string> s0 = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven","Eight", "Nine"};
	vector<string> s1 = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
	vector<string> s2 = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
	stringstream ss;
	if( n/100 )
		ss << s0[n/100] << " Hundred " << (n%100 ? "And " : "");
	n %= 100;

	if( !n )
		return ss.str();

	if( n < 10)
		ss << s0[n];
	else if( n<20 )
		ss << s1[n-10];
	else
	{//20-99
		ss << s2[n/10];
		if(n%10)
		{
			ss << "-" << s0[n%10];
		}
	}
	return ss.str();
}

string numberToEnglish(int n)
{	// english of n.
	if(!n)
		return "Zero";

	//first break n into thousands.
	bool neg = n < 0;
	n = abs(n);
	vector<int> nums;
	while(n)
	{
		nums.push_back(n%1000);
		n /= 1000;
	}
	vector<string> s = {"", " Thousand, ", " Million, ", " Billion, ", " Trillion, "};
	vector<string> res;
	for(int i=0; i<nums.size(); i++)
	{
		if(nums[i])
		{
			res.push_back(s[i]);
			res.push_back(toEnglish(nums[i]));
		}
	}
	if(neg)
		res.push_back("Negative ");
	stringstream ss;
	for(int i=res.size()-1; i>=0; i--)
		ss << res[i];
	return ss.str();
}

void CAlgo::main()
{
	{
		for(int i=-100; i<=INT_MAX; i++)
		{
			cout << i << "->" << numberToEnglish(i) << endl;
		}
	}
	{
		int l = longestPalindrome("abccccdd");
	}
	CAlgo algo;
	{
		string s = frequencySort("tree");
	}

	int k = ~0;
	int s = getSum(1,11);

	for(int i=1; i<100; i++ )
	{
		int arr[i][i];
		arr[0][0] = 0;
		cout << countAndSay(i) << endl;
	}
	bool b = repeatedSubstringPattern("aa");
	for(int i=0; i<-1; i++)
	{
		cout << i << endl;
	}
	int i = strStr("a", "a");
	cout << i <<endl;
	vector<Interval> intervals({{1,3},{5,6},{9,10},{15,17},{30,40}});
	auto i1 = algo.insertInterval(intervals, Interval{4,11});
	auto i2 = algo.insertInterval2(intervals, Interval{4,11});
	for(string s: {"III", "IV", "IX", "LVIII", "MCMXCIV"})
	{
		int y = algo.romanToInt(s);
		cout << y << endl;
	}
	for( int i: {2147483647, 123, -123, INT_MAX, INT_MIN})
	{
		int y = algo.reverse(i);
	}
	algo.isPalindrome(121);
	 b = algo.oneEditAway("abc", "ab ");
	TreeNode* t1 = algo.heapToTree({1,3,2,5});
	TreeNode* t2 = algo.heapToTree({2,1,3,4,7});
	TreeNode* t = algo.mergeTrees2(t1,t2);
	vector<int> v = algo.treeToHeap(t);
	for( auto i : v)
	{
		cout << i;
	}
	cout << endl;

	int l = algo.lengthOfLongestSubstring("abcabcbb");


	algo.twoSum2({13,8,3,5,2,7,11,15}, 9);

	vector<int> nums = {13,8,3,5,2,7,11,15};
	algo.twoSum2(nums, 9);

}
