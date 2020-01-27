/*
 * CBST.cpp
 *
 *  Created on: Aug 19, 2018
 *      Author: jason
 */

#include "CBst.h"

#include <assert.h>
#include <iostream>
#include <stdlib.h> //srand, rand
#include <time.h> //time
CBst::CBst() : r(NULL)
{

}

CBst::~CBst()
{
	ClearAll(); //remove and delete.
}

CBstNode* CBst::Insert(string name, int k)
{
	return Insert(name, k, r);
}

CBstNode* CBst::Remove(int k)
{
	return Remove(k, r);
}

CBstNode* CBst::Find(int k)
{
	return Find(k,r);
}

CBstNode* CBst::Min()
{
	return Min(r);
}

CBstNode* CBst::Max()
{
	return Max(r);
}

CBstNode* CBst::Prev(CBstNode* p)
{
	if( p == NULL)
		return NULL;
	if( p->l != NULL)
	{
		return Max(p->l);
	}
	else
	{//first parent on the left.
		CBstNode* q = p->p;
		while( q != NULL && q->r != p )
		{
			q = p;
			p = p->p;
		}
		return p;
	}
}

CBstNode* CBst::Next(CBstNode* n)
{
	if(n == NULL)
		return NULL;
	if( n->r != NULL)
	{
		return Min(n->r);
	}
	else
	{//first parent on the right path.
		CBstNode* q = n->p;
		while( q != NULL && q->l != n )
		{
			q = n;
			n = n->p;
		}
		return n;
	}
}

CBstNode* CBst::NewNode(string name, int k)
{
	return new CBstNode(name,k);
}
CBstNode* CBst::Insert(string name, int k, CBstNode*& t)
{
	CBstNode* pReturn = t; //default is t->k == k.
	if( NULL == t)
	{//the right place to insert the node. parent pointer unknown at this point, fixed when return.
		t = pReturn = NewNode(name,k);
	}
	else if( k != t->k)
	{
		if( k < t->k)
		{	//insert left.
			pReturn = Insert(name,k,t->l);
		}
		else
		{	//insert right.
			pReturn = Insert(name,k,t->r);
		}
		if( !pReturn->p)
			pReturn->p = t;//fix the parent pointer the very first return.
	}
	return pReturn;
}

CBstNode* CBst::Remove(int k, CBstNode*& pTree)
{
	if( NULL == pTree)
	{//not found.
		return NULL;
	}

	CBstNode* pReturn = NULL;
	if( k < pTree->k)
	{
		pReturn = Remove(k, pTree->l);
		if( !pReturn)
			return NULL;
	}
	else if( k > pTree->k)
	{
		pReturn = Remove(k, pTree->r);
		if(!pReturn)
			return NULL;
	}
	else
	{//found. remove pTree. based on children.
		if( pTree->l && pTree->r)
		{//3.both children. find a replacement child. left most child of right child.
			CBstNode* pReplace = Next(pTree);//must exist and pReplace at most one child.
			assert(pReplace != NULL);
			assert(pReplace->l == NULL || pReplace->r == NULL);
			//switch places between pReplace and pTree. as I need to return pTree node after removal.
			//however, change to be returned k to this replacement k for later removal.

			//switch children.
			pReplace->l = pTree->l;
			pTree->l = NULL;//left most. no left.
			CBstNode* p = pReplace->r;
			pReplace->r = pTree->r;
			pTree->r = p;
			pTree->p = pReplace->p;
			pReplace->p->l = pTree;

			//switch parent.
			pReplace->p->l = pTree;
			if( pTree->p->l == pTree)
				pTree->p->l = pReplace;
			else
				pTree->p->r = pReplace;
			p = pTree->p;
			pReplace->p = pTree->p;
			pTree->p = p;

			//fake the k.
			pTree->k = pReplace->k;

			pReturn = Remove(pTree->k, pReplace->r);
		}
		else
		{
			//Remove here. 1 child max.
			pReturn = pTree;
			if( !pTree->p)
			{//This is root.
				pTree = pTree->l ? pTree->l : pTree->r;
			}
			else
			{//not root.
				if( pTree->l )
				{
					pTree->l->p = pTree->p;
				}
				if( pTree->r)
				{
					pTree->r->p = pTree->p;
				}
				if( pTree->p->l == pTree)
				{
					pTree->p->l = pTree->l ? pTree->l : pTree->r;
				}
				else //right.
				{
					pTree->p->r = pTree->l ? pTree->l : pTree->r;
				}
			}
		}
	}
	return pReturn;
}



void CBst::ClearAll()
{

}
void CBst::Traverse()
{
	Traverse(r);
}

string CBst::Name(CBstNode*p)
{
	return p ? p->name : "";
}

CBstNode* CBst::Find(int k, CBstNode* t)
{
	if(NULL == t)
		return NULL;
	if( t->k == k)
		return t;
	else if( t->k < k)
		return Find(k, t->r);
	else
		return Find(k, t->l);
}

CBstNode* CBst::Min(CBstNode* t)
{
	if( t == NULL)
		return NULL;
	CBstNode* p = t;
	while(p->l != NULL)
		p = p->l;
	return p;
}
CBstNode* CBst::Max(CBstNode* t)
{
	if( t == NULL)
		return NULL;
	CBstNode* p = t;
	while(p->r != NULL)
		p = p->r;
	return p;
}

void CBst::Traverse(CBstNode* t)
{
	if( t == NULL )
		return;
	Traverse(t->l);
	cout << t->name << endl;
	Traverse(t->r);
}

void CBst::Main()
{
	srand(time(NULL));

	CBst bst;
	cout << "Inserting..." << endl;
	int k[] = {20,15,18,10,30,8,5,35,67,43,21,10,89,38,69};
	for( int i=0; i<int(sizeof(k)/sizeof(k[0])); i++)
	{
		int r = k[i];//rand()%10;
		cout << "insert " << r << endl;
		CBstNode* p = bst.Insert(to_string(r),r);
		cout << "inserted:" << Name(p->p) << "->"
			<< p->name << "(" << Name(p->l)  << "," << Name(p->r) << ")" << endl;
	}

	bst.Traverse();

	cout << "Finding..." << endl;
	for( int i=0; i<10; i++)
	{
		int r = rand()%10;
		CBstNode* p = bst.Find(r);
		cout << " k=" << r << "name is: " << (p == NULL? "not found" : bst.Find(r)->name) << endl;
	}

	cout << "Removing..." << endl;
	for( int i=0; i<10; i++)
	{
		int r = rand()%10;
		cout << "Remove " << r << endl;
		bst.Remove(r);
	}
	bst.Traverse();
}
