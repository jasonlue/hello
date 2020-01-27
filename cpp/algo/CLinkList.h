/*
 * CLinkList.h
 *
 *  Created on: Sep 20, 2018
 *      Author: jason
 */

#ifndef CLINKLIST_H_
#define CLINKLIST_H_
#include <iostream>
#include <vector>
using namespace std;
struct ListNode
{
	int val;
	ListNode* next;
	ListNode(int x) : val(x),next(NULL){}
};

struct RandomListNode
{
	int label;
	RandomListNode* random;
	RandomListNode* next;
	RandomListNode(int label) : label(label), random(NULL), next(NULL){}
};
class CLinkList {
public:
	CLinkList();
	virtual ~CLinkList();
	bool nextToken(string& s, string& sep, int& start, string& token);
	string serialize(ListNode* head);
	ListNode* deserialize(string s);
	ListNode* middle(ListNode* head);
	ListNode* remove(ListNode* head, int val);
	ListNode* reverse(ListNode* head);
	ListNode* reverseBetween(ListNode* head, int m, int n);
	ListNode* removeNthFromEnd(ListNode* head, int n);
	void reorderList(ListNode* head);
	ListNode* insertionSortList(ListNode* head);
	ListNode* partition(ListNode* head, int x);
	ListNode* swapPairs(ListNode* head);
	ListNode* getIntersectionNode(ListNode* headA, ListNode* headB);

	ListNode* addTwoNumbers(ListNode* l1, ListNode* l2);

	RandomListNode* copyRandomList(RandomListNode* head);

	static void main();
};

#endif /* CLINKLIST_H_ */
