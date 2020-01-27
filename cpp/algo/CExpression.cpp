/*
 * CExpression.cpp
 *
 *  Created on: Nov 6, 2018
 *      Author: jason
 */

#include "CExpression.h"
#include <iostream>
#include <stack>
#include <assert.h>
CExpression::CExpression() {
	// TODO Auto-generated constructor stub

}

CExpression::~CExpression() {
	// TODO Auto-generated destructor stub
}

bool CExpression::Operator(string s)
{
	string oprs = "+-*/";
	return s.length() == 1 && oprs.find(s[0]) != string::npos;
}

int CExpression::Eval(char opr, int op1, int op2)
{
	switch(opr)
	{
	case '+': return op1 + op2;
	case '-': return op1 - op2;
	case '*': return op1 * op2;
	case '/': return op2 ? op1 / op2 : 0; //avoid div by zero.
	}
	return 0;
}

int CExpression::EvalPrefix(vector<string> tokens)
{//push operators and nums into stack until two nums available for calc.
	stack<pair<bool, int>> stk; //bool = true: operator, else operands.
	for(auto t:tokens)
	{
		if( stk.size()<=1 || stk.top().first || Operator(t))
		{
			stk.push(make_pair(Operator(t), Operator(t) ? t[0] : stoi(t)));
			continue;
		}
		//here top is op, t is op. the next in stk must be opr.
		int op2 = stoi(t);
		while(stk.size()>=2 && !stk.top().first)
		{
			int op1 = stk.top().second;
			stk.pop();
			assert(!stk.empty() && stk.top().first);
			char opr = (char)stk.top().second;
			stk.pop();
			op2 = Eval(opr, op1, op2);
		}
		stk.push(make_pair(false,op2));
	}
	return stk.size() == 1 && !stk.top().first ? stk.top().second : 0;
}
int CExpression::EvalPrefixBackward(vector<string> tokens)
{//prefix backward is simpler. push operands to stack. until opr shows up. pop operands and calc and push results back.
	stack<int> stk;
	for(int i=tokens.size()-1; i>=0; i--)
	{
		if( !Operator(tokens[i]))
			stk.push(stoi(tokens[i]));
		else
		{
			assert(stk.size() >= 2);
			int op1 = stk.top();
			stk.pop();
			int op2 = stk.top();
			stk.pop();
			int res = Eval(tokens[i][0], op1, op2);
			stk.push(res);
		}
	}
	return stk.size() == 1 ? stk.top() : 0;
}

int CExpression::EvalPostfix(vector<string> tokens)
{//postfix:
	stack<int> stk;
	for(auto t:tokens)
	{//push nums into stack. when operator shows up. pop nums and calc then push back the result.
		if( Operator(t))
		{
			if( stk.empty())
				return 0; //indicate error.
			int op2 = stk.top();
			stk.pop();
			if( stk.empty())
				return 0;
			int op1 = stk.top();
			stk.pop();
			stk.push(Eval(t[0], op1,op2));
		}
		else
		{
			stk.push(stoi(t));
		}
	}
	return stk.size() == 1 ? stk.top() : 0;
}

int CExpression::EvalPostfixBackward(vector<string> tokens)
{
	stack<pair<bool,int>> stk;
	for(int i=tokens.size()-1; i>=0; i--)
	{
		if( stk.size() <=1 || stk.top().first || Operator(tokens[i]))
		{
			stk.push(make_pair(Operator(tokens[i]), Operator(tokens[i]) ? tokens[i][0] : stoi(tokens[i])));
			continue;
		}
		//tokens[i] is op1. top is op2.
		int op1 = stoi(tokens[i]);
		while(stk.size() >= 2 && !stk.top().first)
		{
			int op2 = stk.top().second;
			stk.pop();
			assert(stk.top().first);
			char opr = (char)stk.top().second;
			stk.pop();
			op1 = Eval(opr,op1,op2);
		}
		stk.push(make_pair(false,op1));
	}
	return stk.size() == 1 && !stk.top().first? stk.top().second : 0;
}

ExprTreeNode* CExpression::Postfix2Tree(vector<string> tokens)
{
	stack<ExprTreeNode*> stk;
	for(auto t:tokens)
	{
		if(Operator(t))
		{
			assert(stk.size()>=2);
			ExprTreeNode* op2 = stk.top();
			stk.pop();
			ExprTreeNode* op1 = stk.top();
			stk.pop();
			ExprTreeNode* n = new ExprTreeNode(t);
			n->left = op1;
			n->right = op2;
			stk.push(n);
		}
		else
		{
			stk.push(new ExprTreeNode(t));
		}
	}
	assert(stk.size() == 1);
	return stk.top();
}

ExprTreeNode* CExpression::Prefix2Tree(vector<string> tokens)
{//backwards.
	stack<ExprTreeNode*> stk;
	for(int i=tokens.size()-1; i>=0; i--)
	{
		if(Operator(tokens[i]))
		{
			assert(stk.size()>=2);
			ExprTreeNode* op1 = stk.top();
			stk.pop();
			ExprTreeNode* op2 = stk.top();
			stk.pop();
			ExprTreeNode* n = new ExprTreeNode(tokens[i]);
			n->left = op1;
			n->right = op2;
			stk.push(n);
		}
		else
		{
			stk.push(new ExprTreeNode(tokens[i]));
		}
	}
	assert(stk.size() == 1);
	return stk.top();
}

vector<string> CExpression::Tree2Postfix(ExprTreeNode* root)
{//post order traversal.
	vector<string> postorder;
	if(!root)
		return postorder;
	stack<ExprTreeNode*> stk;
	ExprTreeNode* p = root;
	ExprTreeNode* visited = NULL;
	while(p || !stk.empty())
	{
		if(p)
		{
			stk.push(p);
			p = p->left;
		}
		else
		{//no left. check right.
			ExprTreeNode* n = stk.top();
			if( !n->right || n->right == visited)
			{
				stk.pop();
				postorder.push_back(n->val);
				visited = n;
			}
			else
			{
				p = n->right;
			}
		}
	}
	return postorder;
}
vector<string> CExpression::Tree2Prefix(ExprTreeNode* root)
{//preorder traversal
	vector<string> preorder;
	if(!root)
		return preorder;
	stack<ExprTreeNode*> stk({root});
	while(!stk.empty())
	{
		ExprTreeNode* n = stk.top();
		stk.pop();
		preorder.push_back(n->val);
		if( n->right)
			stk.push(n->right);
		if( n->left)
			stk.push(n->left);
	}
	return preorder;
}

void CExpression::main()
{
	CExpression ex;
	vector<string> postfix({"10", "6", "9", "3", "+", "-11", "*", "/", "*", "17", "+", "5", "+"});
	vector<string> prefix({"+", "+", "*", "10", "/", "6", "*", "+", "9", "3", "-11", "17", "5"});
	int post = ex.EvalPostfix(postfix);
	int postB = ex.EvalPostfixBackward(postfix);
	int pre = ex.EvalPrefix(prefix);
	int preB = ex.EvalPrefixBackward(prefix);
	assert(post == postB && pre == preB && post == pre);

	ExprTreeNode* preRoot = ex.Prefix2Tree(prefix);
	vector<string> generatedPrefix = ex.Tree2Prefix(preRoot);
	cout << "generatedPrefix from Tree2Prefix():" << endl;
	for(auto s:generatedPrefix)
		cout << s << ",";
	cout << endl;
	vector<string> generatedPostfix = ex.Tree2Postfix(preRoot);
	cout << "generatedPostfix from Tree2Postfix():" << endl;
	for(auto s:generatedPostfix)
		cout << s << ",";
	cout << endl;
	ExprTreeNode* postRoot = ex.Postfix2Tree(postfix);
	vector<string> generatedPrefix2 = ex.Tree2Prefix(postRoot);
	cout << "generatedPrefix from Tree2Prefix():" << endl;
	for(auto s:generatedPrefix2)
		cout << s << ",";
	cout << endl;
	vector<string> generatedPostfix2 = ex.Tree2Postfix(postRoot);
	cout << "generatedPrefix from Tree2Postfix():" << endl;
	for(auto s:generatedPostfix2)
		cout << s << ",";
	cout << endl;


}
