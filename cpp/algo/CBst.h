/*
 * CBST.h
 *
 *  Created on: Aug 19, 2018
 *      Author: jason
 */

#ifndef CBST_H_
#define CBST_H_

#include <string>
using namespace std;
struct CBstNode
{
	int k;
	string name;
	CBstNode* l;
	CBstNode* r;
	CBstNode* p;//to find the next, prev.
	CBstNode(string name, int k)
	: k(k),name(name), l(NULL),r(NULL),p(NULL){}
};

class CBst {
public:
protected:
	CBstNode* r;
	
public:
	CBst();
	virtual ~CBst();

public:
	CBstNode* Insert(string name, int k);//return the newly inserted node.
	CBstNode* Remove(int k);//return the node removed from the tree.
	CBstNode* Find(int k);
	CBstNode* Min();
	CBstNode* Max();
	CBstNode* Next(CBstNode* n);
	CBstNode* Prev(CBstNode* n);
	void Traverse();

protected:
	virtual CBstNode* NewNode(string name, int k);
	virtual CBstNode* Insert(string name, int k, CBstNode*& t);
	virtual CBstNode* Remove(int k, CBstNode*& n);

protected://internal recursive functions.
	CBstNode* Find(int k, CBstNode* t);
	CBstNode* Min(CBstNode* t);
	CBstNode* Max(CBstNode* t);
	void Traverse(CBstNode* t);

private:
	void ClearAll();
	static string Name(CBstNode* n);
	
public:
	static void Main(); //test cases.
};

#endif /* CBST_H_ */
