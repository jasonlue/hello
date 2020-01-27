/*
 * CExpression.h
 *
 *  Created on: Nov 6, 2018
 *      Author: jason
 */

#ifndef CEXPRESSION_H_
#define CEXPRESSION_H_
#include <string>
#include <vector>
using namespace std;

struct ExprTreeNode
{
	string val;
	ExprTreeNode* left;
	ExprTreeNode* right;
	ExprTreeNode(string val): val(val), left(NULL),right(NULL){}
};
class CExpression {
public:
	CExpression();
	virtual ~CExpression();

	bool Operator(string s);
	int Eval(char opr, int op1, int op2);
	int EvalPostfix(vector<string> tokens);
	int EvalPrefix(vector<string> tokens);
	int EvalPostfixBackward(vector<string> tokens);
	int EvalPrefixBackward(vector<string> tokens);

	ExprTreeNode* Postfix2Tree(vector<string> tokens);
	ExprTreeNode* Prefix2Tree(vector<string> tokens);
	vector<string> Tree2Postfix(ExprTreeNode* root);
	vector<string> Tree2Prefix(ExprTreeNode* root);
	static void main();
};

#endif /* CEXPRESSION_H_ */
