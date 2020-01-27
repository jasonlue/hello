/*
 * CAvlBst.cpp
 *
 *  Created on: Aug 23, 2018
 *      Author: jason
 */

#include "CAvlBst.h"
#include <assert.h>
CAvlBst::CAvlBst() {
	// TODO Auto-generated constructor stub

}

CAvlBst::~CAvlBst() {
	// TODO Auto-generated destructor stub
}

CBstNode* CAvlBst::NewNode(string name, int key)
{
	return new CAvlBstNode(name,key);
}

int CAvlBst::Height(CBstNode* p)
{
	return p ? ((CAvlBstNode*)p)->h : -1;
}

void CAvlBst::UpdateHeight(CBstNode* p)
{
	((CAvlBstNode*)p)->h = 1 + max(Height(p->l),Height(p->r));
}
CBstNode* CAvlBst::Insert(string name, int key, CBstNode*& pTree)
{
	CBstNode* p = CBst::Insert(name,key,pTree);
	pTree = Rebalance(pTree);
	UpdateHeight(pTree);
	return p;
}
CBstNode* CAvlBst::Remove(int key, CBstNode*& pTree)
{
	CBstNode* p = CBst::Remove(key, pTree);
	assert(pTree);
	pTree = Rebalance(pTree->p);
	UpdateHeight(pTree);
	return p;
}

CBstNode* CAvlBst::Rebalance(CBstNode* pTree)
{
	if( !pTree)
		return nullptr;

	if( Height(pTree->l) > Height(pTree->r)+1)
	{
		if( Height(pTree->l->l) > Height(pTree->l->r))
		{
			return RightRotate(pTree);
		}
		else
		{
			return DoubleRightRotate(pTree);
		}
	}
	else if(Height(pTree->r) > Height(pTree->l)+1 )
	{
		if( Height(pTree->r->r) > Height(pTree->r->l))
		{
			return LeftRotate(pTree);
		}
		else
		{
			return DoubleLeftRotate(pTree);
		}
	}
	return pTree;
}

CBstNode* CAvlBst::LeftRotate(CBstNode*x)
{
	CBstNode* y = x->r;
	x->r = y->l;
	if( y->l)
		y->l->p=x;

	y->p = x->p;
	y->l = x;
	x->p = y;
	return y;
}

CBstNode* CAvlBst::RightRotate(CBstNode*y)
{
	CBstNode* x = y->l;
	y->l = x->r;
	if( x->r )
		x->r->p = y;

	x->r = y;
	x->p = y->p;
	y->p = x;
	return x;
}

CBstNode* CAvlBst::DoubleLeftRotate(CBstNode*x)
{
	x->r = RightRotate(x->r);
	return LeftRotate(x);
}

CBstNode* CAvlBst::DoubleRightRotate(CBstNode*z)
{
	z->l = LeftRotate(z->l);
	return RightRotate(z);
}

