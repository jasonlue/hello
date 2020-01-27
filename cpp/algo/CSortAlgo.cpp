/*
 * CSortAlgo.cpp
 *
 *  Created on: Aug 29, 2018
 *      Author: jason
 */

#include "CSortAlgo.h"
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <unordered_map>
#include <queue>
CSortAlgo::CSortAlgo() {
	// TODO Auto-generated constructor stub

}

CSortAlgo::~CSortAlgo() {
	// TODO Auto-generated destructor stub
}

void CSortAlgo::selectionSort(vector<int>&v)
{
	for(int i=0; i<v.size(); i++)
	{
		int minIndex = i;
		for(int j=i+1; j<v.size(); j++)
		{
			if(v[j] < v[minIndex])
				minIndex = j;
		}
		swap(v[i],v[minIndex]);
	}
}
void CSortAlgo::bubbleSort(vector<int>&v)
{
	size_t end = v.size();
	for( size_t i=0; i<v.size(); i++)
	{
		for( size_t j=1; j<end; j++)
		{
			if( v[j-1] > v[j])
			{
				int t = v[j-1];
				v[j-1] = v[j];
				v[j] = t;
			}
		}
		end--; //v[end-1] is in right position now.
	}
}
void CSortAlgo::insertSort(vector<int>& v)
{
	for(size_t i=1; i<v.size(); i++)
	{//insert v[i] into v[0..i] thus make [0..i] sorted.
		size_t j=0;
		for(; j<i; j++)
		{
			if( v[j] > v[i])
				break;
		}
		if( j<i)
		{
			int t = v[i];
			for(size_t k=i; k>j; k--)
			{
				v[k]=v[k-1];
			}
			v[j] = t;
		}
	}
}

void CSortAlgo::heapify(vector<int>&v, int root, int size/*actual size of array*/)
{
	int l = 2*root+1;
	int r = 2*root+2;
	if(l>=size)
		return;

	if(r < size && v[l] < v[r])
		swap(l,r);
	if(v[l]>v[root])
	{
		swap(v[root],v[l]);
		heapify(v,l,size);
	}
}

void CSortAlgo::heapSort(vector<int>&v)
{
	//build max heap. from bottom up. leaves are good.
	for(int i=v.size()/2; i>=0; i--)
		heapify(v,i,v.size());

	for(int j=v.size()-1; j>0; j--)
	{
		swap(v[0],v[j]);
		heapify(v,0, j);
	}

}

bool CSortAlgo::findKthViaHeap(vector<int>&v, int k, int& val)
{
	if(k > (int)v.size())
		return false;
	//maintain max heap with size k
	vector<int> kv(v.begin(), v.begin()+k);
	heapify(kv,0,kv.size()-1);
	for(int i=k; i<v.size(); i++)
	{
		if(v[k] < kv[0])
		{
			kv[0] = v[k];
			heapify(kv,0,kv.size()-1);
		}
	}
	val = kv[0];
	return true;
}

void CSortAlgo::mergeSort(vector<int>&v)
{
	if( v.size() <= 1)
		return;

	size_t k = v.size()/2;
	auto v1 = vector<int>(v.begin(), v.begin()+k);
	auto v2 = vector<int>(v.begin()+k, v.end());
	v.clear();

	mergeSort(v1);
	mergeSort(v2);

	//merge two arrays.
	size_t i=0;
	size_t j=0;
	while(i < v1.size() || j < v2.size())
	{//not empty.
		if( i >= v1.size() )
		{
			v.push_back(v2[j++]);
		}
		else if( j >= v2.size() )
		{
			v.push_back(v1[i++]);
		}
		else if( v1[i] < v2[j] )
		{
			v.push_back(v1[i++]);
		}
		else
		{
			v.push_back(v2[j++]);
		}
	}
}

int CSortAlgo::partition(vector<int>&v, int l, int r)
{
	int p = r;
	int w = l;
	for(int i=l; i<r; i++)
	{
		if( v[i] <= v[p])
		{
			swap(v[i], v[w]);
			w++;
		}
	}
	swap(v[p], v[w]);
	return w;
}

void CSortAlgo::quickSort(vector<int>&v)
{
	quickSort(v, 0,v.size()-1);
}
void CSortAlgo::quickSort(vector<int>&v, int l, int r)
{
	if(l>=r)
		return;
	int w = partition(v,l,r);
	//w in right place.
	quickSort(v, l, w-1);
	quickSort(v, w+1, r);
}

bool CSortAlgo::findKth(vector<int>&v, int k, int& val)
{
	if(k > (int)v.size())
		return false;
	val = findKth(v, k-1, 0, v.size()-1); //internally starts from 0.
	return true;
}
int CSortAlgo::findKth(vector<int>&v, int k, int l, int r)
{
	int w = partition(v,l,r);
	if( k == w )
		return v[w];
	else if( k < w )
		return findKth(v,k,l,w-1);
	else
		return findKth(v, k, w+1, r);
}

bool CSortAlgo::findKthLargest(vector<int>&v, int k, int& kv)
{
	int l = v.size() - k;
	if( l<=0 || l >= v.size())
		return false;
	kv = findKth(v,l,0, v.size()-1);
	return true;
}
bool CSortAlgo::findMNth(vector<int>&v, int m, int n, int& mv, int&nv)
{
	int M = min(m,n);
	int N = max(m,n);
	if( M <= 0 || N > (int)v.size())
		return false;

	bool mf = false;
	bool nf = false;
	findMNth(v,M-1,N-1, mv, nv, mf, nf, 0, v.size()-1);
	return true;
}
void CSortAlgo::findMNth(vector<int>&v, int m, int n, int& mv, int&nv, bool&mf, bool& nf, int l, int r)
{
	int w = partition(v, l, r);
	if(w == m)
	{
		mf = true;
		mv = v[w];
	}

	if(w ==n)
	{
		nf = true;
		nv = v[w];
	}
	if( mf && nf)
		return; //found both.

	if( w < m)
		findMNth(v,m,n,mv,nv,mf,nf,w+1,r);
	else if( w > n)
		findMNth(v,m,n,mv,nv,mf,nf,l,w-1);
	else
	{//m left, n right.
		if( !mf)
			findMNth(v,m,n,mv,nv,mf,nf,l,w-1);
		if( !nf)
			findMNth(v,m,n,mv,nv,mf,nf,w+1,r);
	}
}

float CSortAlgo::findMedian(vector<int>&v)
{
	assert(!v.empty());
	int m = (v.size()-1)/2;
	int n = v.size()/2;
	int mv = 0;
	int nv = 0;
	findMNth(v, m+1,n+1,mv,nv);
	return (float(mv)+nv)/2;
}
int CSortAlgo::findMedianOfTwoSortedArrays(vector<int>& v1, vector<int>& v2)
{
	return findMedianOfTwoSortedArrays(v1, 0, v1.size()-1, v2, 0, v2.size()-1);
}
int CSortAlgo::findMedianOfTwoSortedArrays(vector<int>& v1, int l1, int r1, vector<int>& v2, int l2, int r2)
{	//binary search on the longer array, the element that breaks v1 into 2 parts.
	if( v1.size() < v2.size())
		swap(v1,v2);
	int lElements = (v1.size() + v2.size()+1)/2;

	int l = 0;
	int r = v1.size()-1;
	while(l<r)
	{
		int i = (l+r)/2;
		int j = lElements - i;
		//see if (i,j) separates two subsets.
		if( v1[i-1] <= v2[j] && v2[j-1] <= v1[i])
		{//found.

		}

	}
	return -1;
}

void CSortAlgo::radixSort(vector<int>&v)
{

}

int CSortAlgo::binarySearchIterative(vector<int>& v, int k)
{//return index.
	int l = 0;
	int r = v.size()-1;
	while(l <= r)
	{
		int i = (l+r)/2;
		if( k == v[i] )
			return i;
		else if( k < v[i] )
			r = i-1;
		else
			l = i+1;
	}
	return -1;
}

int CSortAlgo::binarySearchInsert(vector<int>& v, int k)
{//return index of the k, or the index k should be.
	int l = 0;
	int r = v.size()-1;
	while(l <= r)
	{
		int i = (l+r)/2;
		if( k == v[i] )
			return i;
		else if( k < v[i] )
			r = i-1;
		else
			l = i+1;
	}
	return l;
}

int CSortAlgo::binarySearch(vector<int>& v, int k)
{
	return binarySearch(v, k, 0, v.size()-1);
}

int CSortAlgo::binarySearch(vector<int>& v, int k, int l, int r)
{
	if( l > r )//subset empty
		return -1;
	int i = (l+r) / 2;
	if( k == v[i] )
		return i;
	else if( k < v[i])
		return binarySearch(v, k, l, i-1);
	else
		return binarySearch(v, k, i+1, r);
}

vector<int > CSortAlgo::topKFrequentMaxHeap(vector<int>& nums, int k)
{
	unordered_map<int,int> freq;
	for(auto n:nums)
		freq[n]++;

	priority_queue<pair<int,int>> pq; //pair less used the first for comparison.
	vector<int> res;
	for(auto& i:freq)
	{
		pq.push(make_pair(i.second,i.first));
		if(pq.size() >= freq.size())
		{//k elements outside could be larger or smaller than top. even all larger, top is K-th.
			res.push_back(pq.top().second);
			pq.pop();
		}
	}
	return res;
}

vector<int > CSortAlgo::topKFrequentMinHeap(vector<int>& nums, int k)
{//maintain k min heap.
	unordered_map<int,int> freq;
	for(auto n:nums)
		freq[n]++;

//	priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
	priority_queue<tuple<int,int>, vector<tuple<int,int>>, greater<tuple<int,int>>> pq;
	for(auto& i:freq)
	{
		if(pq.size() < k )
			pq.push(make_tuple(i.second, i.first));
		else
		{
			if(i.second > get<0>(pq.top()))
			{
				pq.push(make_tuple(i.second, i.first));
				pq.pop(); //pop the smallest.
			}
		}
	}
	vector<int> res;
	while(!pq.empty())
	{
		res.push_back(get<0>(pq.top()));
		pq.pop();
	}
	return res;
}

vector<int > CSortAlgo::topKFrequentCountingSort(vector<int>& nums, int k)
{
	unordered_map<int,int> freq;
	for(auto n:nums)
	{
		freq[n]++;
	}
	vector<vector<int>> v(freq.size()+1);
	for(auto f:freq)
	{
		v[f.second].push_back(f.first);
	}
	vector<int> res;

	for(int i=v.size()-1; i>=v.size()-k; i--)
	{
		res.insert(res.end(), v[i].begin(), v[i].end());
	}
	return res;
}

vector<string> CSortAlgo::topKFrequent(vector<string>& words, int k) //alphabetical order comes first!
{
	unordered_map<string,int> freq;
	for(auto& w : words)
		freq[w]++;
//	using pr = pair<string,int>;
	auto lessCmp = [](const pair<string,int>& l, const pair<string,int>& r) {return l.second > r.second || (l.second == r.second && l.first<r.first);};
//	auto cmp = [](pair<string,int> l, pair<string,int> r) {return l.first>r.first;};

//	priority_queue<pair<string,int>,vector<pair<string,int>>, decltype(cmp)> pq(cmp);
	priority_queue<pair<string,int>, vector<pair<string,int>>, decltype(lessCmp)> pq(lessCmp);
	for(auto& f:freq)
	{
		if(pq.size() < k)
			pq.push(f);
		else
		{
			if( pq.top().second < f.second || pq.top().second == f.second && pq.top().first > f.first )
			{
				pq.push(f);
				pq.pop();
			}
		}
	}
	vector<string> v(pq.size());
	int i = v.size()-1;
	while(!pq.empty())
	{
		v[i--] = pq.top().first;
		pq.pop();
	}
	return v;
}
void CSortAlgo::print(vector<int>& v)
{
	for(auto i=0u; i<v.size();i++)
		cout << v[i] << ",";
	cout << endl;
}
void CSortAlgo::print(vector<string>& v)
{
	for(auto i=0u; i<v.size();i++)
		cout << v[i] << ",";
	cout << endl;
}

int partition(vector<int>& v, int l, int r)
{//return the index of pivot.
  int w = l; //wall.
  int p = r; //pivot.
  for(int i=l; i<r; i++)
  {
    if(v[i] <= v[p])
    {
      swap(v[w],v[i]);
      w++;
    }
  }
  swap(v[w],v[p]);
  return w;
}
void partitionM2nIndex(vector<int>&v, int m, int n,
                       bool& mf, bool& nf, int l, int r)
{
	if(l>r)
		return;

  int p = partition(v,l,r);
  if(p==m)
    mf = true;
  if(p==n)
    nf = true;
  if(mf&&nf)
    return;
  if(p<=m)
    partitionM2nIndex(v,m,n,mf,nf,p+1,r);
  else if(p>=n)
    partitionM2nIndex(v,m,n,mf,nf,l,p-1);
  else
  {//p in the middle of m..n
    if(!mf)
      partitionM2nIndex(v,m,n,mf,nf,l,p-1);
    if(!nf)
      partitionM2nIndex(v,m,n,mf,nf,p+1,r);
  }
}

vector<int> smallestM2nElements(vector<int>&v, int m, int n)
{
  if( m>n || m <= 0 || n <= 0 || m > v.size() || n >v.size())
    return vector<int>();
  m--;
  n--; //convert to smallest index.
  bool mf=false, nf=false;
  partitionM2nIndex(v,m,n,mf,nf,0,v.size()-1);
  return vector<int>(v.begin()+m, v.begin()+n+1);
}


void CSortAlgo::main()
{

	CSortAlgo algo;
	vector<int> v({0,1,2,3,4,5,6,7,8,9});
	  for(int i=1; i<10; i++)
	  {
	    for(int j=6; j<10; j++)
	    {
	      random_shuffle(v.begin(), v.end());
	      vector<int> res = smallestM2nElements(v,i,j);
	      algo.print(v);
	      cout << "smallest " << i << "->" << j << "Elements: "<< endl;
	      algo.print(res);
	    }
	  }

//	random_shuffle(v.begin(),v.end());
	algo.quickSort(v);

	vector<int> v0({1,1,1,2,2,3});
	vector<int> v1({1,2,3,4,5,7,8,9,20});
	vector<int> v2={9,8,7,6,5,4,3,2,1};
	vector<int> v3={1,3,5,7,9,2,4,6,8};
	vector<int> v4={6,8,3,5,2,4,9,1};
	vector<int> v5={1,2,3,5,4,6,7,8,9};
	vector<int> v6={9,8,7,6,5,1,2,3,4};
	vector<int> v7={9,8,7,6,0,1,2,3,4};
	vector<string> words({"1", "2","3", "4", "5", "6", "7"});
	assert(string("1") < string("2"));
	assert(string("a") < string("b"));
	assert(string("abc") < string("ac"));
//	assert(string("abc") == string("ac"));
	for(int k=1; k<=words.size(); k++)
	{
		vector<string> strs = algo.topKFrequent(words, k);
		algo.print(strs);
	}
	v = algo.topKFrequentMaxHeap(v0,2);
	v = algo.topKFrequentMinHeap(v0,2);
	v = algo.topKFrequentCountingSort(v0,2);
	algo.heapSort(v4);
	algo.print(v4);
	sort(v2.begin(),v2.end());
	auto it = lower_bound(v2.begin(), v2.end(),5);
	assert(*it == 5);
	sort(v7.begin(),v7.end());
	it = lower_bound(v7.begin(), v7.end(),5);
	assert(*it == 6);

	float m = algo.findMedian(v3);
	algo.print(v3);
	cout << "findMedian(): " << m << endl;
	v3.push_back(0);
	algo.print(v3);
	m = algo.findMedian(v3);
	cout << "findMedian(): " << m << endl;
	for( int k=1; k<=10; k++)
	{
		int kv = 0;
		vector<int> v({6,8,3,5,2,4,9,1,7});
		algo.print(v);
		bool found = algo.findKth(v,k, kv);
		cout << "findKth(): " << (found? "found! " : "not found! ") << k << ": " << kv << endl;
		vector<int> vv({6,8,3,5,2,4,9,1,7});
		algo.print(vv);
		found = algo.findKthLargest(vv,k, kv);
		cout << "findKthLargest(): " << (found? "found! " : "not found! ") << k << ": " << kv << endl;
		vector<int> vvv({6,8,3,5,2,4,9,1,7});
		algo.print(vvv);
		int mv = 0, nv = 0;
		found = algo.findMNth(vvv,k, k+1, mv, nv);
		cout << "findMNth(): " << (found? "found! " : "not found! ") << "(" << k << "," << k+1 << "): (" << mv << "," << nv << ")" << endl;

	}
	for(int i=0; i<30; i++)
	{
		vector<int> vvvv({i,i+1});
		int x = algo.binarySearch(v1,i);
		int y = algo.binarySearchIterative(v1,i);
		int z = algo.binarySearchInsert(v1,i);
		cout << "binarySearch() [" << x <<"]" << i << endl;
		cout << "binarySearchIterative() [" << y <<"]" << i << endl;
		cout << "binarySearchInsert() [" << z <<"]" << i << endl;
	}
	algo.quickSort(v2);
//	algo.bubbleSort(v2);
	algo.insertSort(v2);
	algo.mergeSort(v4);
	algo.radixSort(v6);
	algo.heapSort(v6);


}
