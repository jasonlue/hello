/*
 * CSortAlgo.h
 *
 *  Created on: Aug 29, 2018
 *      Author: jason
 */

#ifndef CSORTALGO_H_
#define CSORTALGO_H_
#include <vector>
#include <string>
using namespace std;

class CSortAlgo {
public:
	CSortAlgo();
	virtual ~CSortAlgo();

	void selectionSort(vector<int>&v);
	void bubbleSort(vector<int>&v);
	void insertSort(vector<int>& v);
	void countingSort(vector<int>&v);

	void heapify(vector<int>&v, int i, int n);
	void heapSort(vector<int>&v);
	bool findKthViaHeap(vector<int>&v, int k, int& kv);

	void mergeSort(vector<int>&v);

	int partition(vector<int>&v, int l, int r);
	void quickSort(vector<int>&v);
	void quickSort(vector<int>&v, int l, int r);
	bool findKth(vector<int>&v, int k, int& kv);
	bool findKthLargest(vector<int>&v, int k, int& kv);
	bool findMNth(vector<int>&v, int m, int n, int& mv, int&nv);
	void findMNth(vector<int>&v, int m, int n, int& mv, int&nv, bool&mf, bool& nf, int l, int r);
	float findMedian(vector<int>&v);
	int findKth(vector<int>&v, int k, int l, int r);
	int findMedianOfTwoSortedArrays(vector<int>& v1, vector<int>& v2);
	int findMedianOfTwoSortedArrays(vector<int>& v1, int l1, int r1, vector<int>& v2, int l2, int r2);

	void radixSort(vector<int>&v);
	int binarySearch(vector<int>& v, int k);
	int binarySearch(vector<int>& v, int k, int l, int r);
	int binarySearchIterative(vector<int>& v, int k);
	int binarySearchInsert(vector<int>& v, int k);
	void print(vector<int>&v);
	void print(vector<string>&v);

	vector<int > topKFrequentMaxHeap(vector<int>& nums, int k);
	vector<int > topKFrequentMinHeap(vector<int>& nums, int k);
	vector<int > topKFrequentCountingSort(vector<int>& nums, int k);
	vector<string> topKFrequent(vector<string>& words, int k); //alphabetical order comes first!

	static void main();

private:
};

#endif /* CSORTALGO_H_ */
