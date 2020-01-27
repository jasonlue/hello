/*
 * CLinkList.cpp
 *
 *  Created on: Sep 20, 2018
 *      Author: jason
 */

#include "CLinkList.h"
#include <sstream>

CLinkList::CLinkList() {
	// TODO Auto-generated constructor stub

}

CLinkList::~CLinkList() {
	// TODO Auto-generated destructor stub
}

string CLinkList::serialize(ListNode* head)
{
	if(!head)
		return "";
	stringstream ss;
	ss << "[";
	for(ListNode* p=head; p!= NULL; p = p->next)
	{
		ss << p->val << ",";
	}
	string res = ss.str();
	res.pop_back(); //last ,
	return res + "]";
}

bool CLinkList::nextToken(string& s, string& sep, int& start, string& token)
{
	int i = s.find_first_not_of(sep, start);
	if( i==string::npos)
		return false;
	int j = s.find_first_of(sep,i);
	token = s.substr(i,j-i);
	start = j;
	return true;
}
ListNode* CLinkList::deserialize(string s)
{
	string sep = "[, ]";
	int start = 0;
	string token;
	if(!nextToken(s,sep,start, token))
		return NULL;
	ListNode* head = new ListNode(stoi(token));
	ListNode* tail = head;
	while(nextToken(s,sep,start,token))
	{
		ListNode* t = new ListNode(stoi(token));
		tail->next = t;
		tail = tail->next;
	}
	return head;
}

ListNode* CLinkList::middle(ListNode* head)
{
	if(!head)
		return NULL;
	ListNode* slow = head;
	ListNode* fast = head;
	while(fast&&fast->next)
	{
		slow = slow->next;
		fast = fast->next->next;
	}
	return slow;
}

ListNode* CLinkList::remove(ListNode* head, int val)
{
	ListNode* p = NULL;//prev pointer
	ListNode* q = head;//current pointer
	while(q != NULL && q->val != val)
	{
		p = q;
		q = q->next;
	}
	if(q)
	{
		if(p)
		{
			p->next = q->next;
			delete q;
		}
		else
		{
			head = q->next;
			delete q;
		}
	}
	return head;
}

ListNode* CLinkList::reverse(ListNode* head)
{
	if(!head)
		return NULL;
	ListNode* newHead = NULL;
	while(head)
	{
		ListNode* n = head;
		head = head->next;
		n->next = newHead;
		newHead = n;
	}
	return newHead;
}


ListNode* CLinkList::reverseBetween(ListNode* head, int m, int n)
{
	int c = n-m+1;
	m--;
	ListNode* p = NULL;
	if(m>0)
	{
		p = head;
		while(p&&--m)
			p = p->next;
	}

	ListNode* q = p ? p->next : head;
	if(!q->next)
		return head;

	ListNode* r = q->next;
	while(r&&--c)
	{
		q->next = r->next;
		r->next = p?p->next : head;
		p?p->next = r : head = r;
		r = q->next;
	}
	return head;
}

ListNode* CLinkList::removeNthFromEnd(ListNode* head, int n)
{
	n--; //convert to z-based index.
	ListNode* q = head;
	while(n--)
		q = q->next;

	ListNode* pp = NULL;
	ListNode* p = head;

	while(q->next)
	{
		pp = p;
		p = p->next;
		q = q->next;
	}
	//remove p.
	if( !pp )
		head = head->next;
	else
		pp->next = p->next;
	delete p;
	return head;
}

void CLinkList::reorderList(ListNode* head)
{	//break list in two parts.
	ListNode* ps = NULL;
	ListNode* s = head;
	ListNode* f = head;
	while(f&&f->next)
	{
		ps = s;
		s = s->next;
		f = f->next->next;
	}
	if(f)
	{
		ps = s;
		s = s->next;
	}
	ps->next = NULL;
	ListNode* h2 = NULL;
	//reverse h2;
	while(s)
	{
		ListNode* t = s->next;
		s->next = h2;
		h2 = s;
		s = t;
	}

	//merge back.
	ListNode* p = head;
	ListNode* q = h2;
	while(q)
	{
		ListNode* np = p->next;
		ListNode* nq = q->next;
		q->next = p->next;
		p->next = q;
		p = np;
		q = nq;
	}
}

ListNode* CLinkList::insertionSortList(ListNode* head)
{
	if(!head || !head->next)
		return head;
	ListNode* p = head->next;
	head->next = NULL;
	while(p)
	{
		ListNode* np = p->next;
		ListNode* pq = NULL;
		ListNode* q = head;
		while(q && q->val <= p->val)
		{
			pq = q;
			q = q->next;
		}
		if(pq)
		{
			p->next = q;
			pq->next = p;
		}
		else
		{
			p->next = head;
			head = p;
		}
		p = np;
	}
	return head;
}

ListNode* CLinkList::partition(ListNode* head, int x)
{
	if(!head || !head->next)
		return head;
	ListNode* pw = NULL;
	ListNode* w = head;
	ListNode* pp = head;
	ListNode* p = head->next;
	while(p)
	{
		ListNode* np = p->next;
		if( p->val <= x)
		{//swap p and w. when p right behind w. bug.
			if(pw)
			{
				p->next = w->next!=p?w->next:w;
				pw->next = p;
				w->next = np;
				pp->next = w;

				pw = pw->next;
				w = pw->next;
			}
			else
			{
				p->next = w->next!=p?w->next : w;
				head = p;
				w->next = np;
				pp->next = w;

				pw = head;
				w = pw->next;
			}
		}
		p = np;
	}
	return head;
}

ListNode* CLinkList::swapPairs(ListNode* head)
{
	if( !head || !head->next)
		return head;
	ListNode* pp = NULL;
	ListNode* p = head;
	ListNode* q = p->next;
	while(q)
	{
		ListNode* nq = q->next;
		if(pp)
			pp->next = q;
		else
			head = q;
		p->next = nq;
		q->next = p;

		pp = p;
		p = nq;
		q = nq?nq->next:NULL;
	}
	return head;
}

ListNode* CLinkList::getIntersectionNode(ListNode* headA, ListNode* headB)
{
	ListNode* p = headA;
	ListNode* q = headB;
	int as = 0;
	int bs = 0;
	while(p != NULL)
	{
		p = p->next;
		as++;
	}
	while(q != NULL)
	{
		q = q->next;
		bs++;
	}
	p = headA;
	q = headB;
	if( as < bs)
		swap(p,q);
	for(int i=0; i<abs(bs-as); i++)
		p = p->next;
	while(p!= NULL)
	{
		if( p==q)
			return p;
		p = p->next;
		q = q->next;
	}
	return NULL;
}

ListNode* CLinkList::addTwoNumbers(ListNode* l1, ListNode* l2)
{
	ListNode* result = NULL, *last = NULL;
	int x = 0;
	while(l1||l2||x)
	{
		int y = (l1? l1->val : 0) + (l2 ? l2->val : 0) + x;
		x = y / 10;
		if( !result)
		{
			last = result = new ListNode(y%10);
		}
		else
		{
			last->next = new ListNode(y%10);
			last = last->next;
		}
		l1 = l1->next;
		l2 = l2->next;
	}
	return result;
}

RandomListNode* CLinkList::copyRandomList(RandomListNode* head)
{
    if(!head)
        return NULL;

    RandomListNode* p = head;
    while(p)
    {
        RandomListNode* q = new RandomListNode(p->label);
        q->next = p->next;
        q->random = p->random;
        p->next = q;
        p = q->next;
    }
    //fix random pointer.
    p = head;
    while(p&&p->next)
    {
    	p->next->random = p->next->random? p->next->random->next : NULL;
    	p = p->next->next;
    }
    //now extract new list and fix the random pointer.
    RandomListNode* newHead = NULL;
    RandomListNode* newTail = NULL;
    p = head;
    while(p&&p->next)
    {
        RandomListNode* q = p->next;
        p = p->next;
        if(newTail)
        {
            newTail->next = q;
            newTail = q;
        }
        else
        {
            newHead = q;
            newTail = q;
        }
        p->next = q->next;
        p = q->next;
    }
    return newHead;
}

void CLinkList::main()
{
	CLinkList ll;
	RandomListNode* r = new RandomListNode(-1);
	RandomListNode* s = ll.copyRandomList(r);
	ListNode* head = ll.deserialize("[1,2,3,4,5,6,7,8,9]");
	cout << ll.serialize(head) << endl;
	cout << ll.serialize(ll.reverse(head)) << endl;
}

