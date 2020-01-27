/*
 * CAvlBst.h
 *
 *  Created on: Aug 23, 2018
 *      Author: jason
 */

#ifndef CAVLBST_H_
#define CAVLBST_H_

#include "CBst.h"

struct CAvlBstNode : public CBstNode
{
	int h;//for balanced tree purposes.
	CAvlBstNode(string name, int k) : CBstNode(name,k),h(0){}
};
class CAvlBst: public CBst {
public:
	CAvlBst();
	virtual ~CAvlBst();

protected:
	virtual CBstNode* NewNode(string name, int k);
	virtual CBstNode* Insert(string name, int k, CBstNode*& t);
	virtual CBstNode* Remove(int k, CBstNode*& t);

private:
	void UpdateHeight(CBstNode*n);
	int Height(CBstNode*n);
	CBstNode* Rebalance(CBstNode*n);
	CBstNode* LeftRotate(CBstNode*x); //given tree root, return new tree root.
	CBstNode* DoubleLeftRotate(CBstNode* x);
	CBstNode* RightRotate(CBstNode*y);
	CBstNode* DoubleRightRotate(CBstNode*y);
};

#endif /* CAVLBST_H_ */
