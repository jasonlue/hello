/*
 * CTree.cpp
 *
 *  Created on: Sep 6, 2018
 *      Author: jason
 */

#include "CTree.h"
#include "CMeasure.h"
#include <queue>
#include <stack>
#include <algorithm>
#include <numeric>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;
using namespace boost;
CTree::CTree() {
	// TODO Auto-generated constructor stub

}

CTree::~CTree() {
	// TODO Auto-generated destructor stub
}

BNode* CTree::buildRightTree(vector<int>&v)
{//only right children.
	BNode* root = new BNode(v[0]);
	BNode* p = root;
	for(int i=1; i<v.size(); i++)
	{
		p->right = new BNode(v[i]);
		p = p->right;
	}
	return root;
}
Node* CTree::buildQuadTree(vector<int>& v)
{
	if(v.empty())
		return NULL;
	int index = 0;
	Node* root = new Node(v[index++]);
	queue<Node*> q({root});
	while(index < v.size())
	{
		Node* n = q.front();
		q.pop();
		int childrenLeft = 4;
		while(index < v.size() && childrenLeft--)
		{
			Node* child = new Node(v[index++]);
			n->children.push_back(child);
			q.push(child);
		}
	}
	return root;
}

vector<vector<int>> CTree::levelOrder(Node* root)
{
	vector<vector<int>> levels;
	if(!root)
		return levels;
	queue<Node*> q({root});
	while(!q.empty())
	{
		int size = q.size();
		levels.push_back(vector<int>(size));
		for(int i=0; i<size;i++)
		{
			Node* n = q.front();
			q.pop();
			levels[levels.size()-1][i] = n->val;
			for(auto child : n->children)
			{
				q.push(child);
			}
		}
	}
	return levels;
}
vector<vector<int>> CTree::levelOrderRecursive(Node* root)
{
	vector<vector<int>> levels;
	if(!root)
		return levels;
	levelOrderRecursive(root,0,levels);
	return levels;
}
void CTree::levelOrderRecursive(Node* root, int height, vector<vector<int>>& levels)
{
	if( levels.size()<=height)
		levels.push_back(vector<int>());
	levels[height].push_back(root->val);
	for(auto child : root->children)
		levelOrderRecursive(child, height+1, levels);
}

string CTree::serializeBT(BNode* root)
{
	if(!root)
		return "[]";

	stringstream ss;
	ss << "[";
	queue<BNode*> q({root});
	while(!q.empty())
	{
		BNode* n = q.front();
		q.pop();
		if(n)
		{
			ss << n->val << ",";
			q.push(n->left);
			q.push(n->right);
		}
		else
		{
			ss<<"null,";
		}
	}
	string s = ss.str();
	string null = "null,";
	int i = s.length()-null.length();
	while(i>=0 && s.substr(i,null.length()) == null)
	{
		i -= null.length();
	}
	//remove last nulls and last ,

	return s.substr(0,i+null.length()-1)+"]";//remove last ,
}

BNode* CTree::deserializeBTBoost(string data)
{
	char_separator<char> sep("[ ,]");
	tokenizer<char_separator<char>> tok(data,sep);
	auto tokIt = tok.begin();
	if( tokIt == tok.end())
		return NULL;
	BNode* root = new BNode(stoi(*tokIt));
	++tokIt;
	queue<BNode*> q;
	q.push(root);
	int i=1;
	while(tokIt !=tok.end())
	{
		BNode* n = q.front();
		q.pop();
		if(*tokIt != "null")
		{
			n->left = new BNode(stoi(*tokIt));
			q.push(n->left);
		}
		if(++tokIt ==tok.end())
			break;
		if(*tokIt != "null")
		{
			n->right = new BNode(stoi(*tokIt));
			q.push(n->right);
		}
		++tokIt;
	}
	return root;
}

bool nextToken(const string& data, const string& sep, int& start, string& tok)
{
	if( start < 0)
		return false;
	int i = data.find_first_not_of(sep,start);
	if( i < 0)
		return false;
	int j = data.find_first_of(sep,i);
	start = j;
	if(j==i || j==data.length())
		return false;
	tok = data.substr(i,j-i);
	return true;
}
BNode* CTree::deserializeBT(string data)
{
	int start = 0;
	string sep("[ ,]");
	string tok;
	if(!nextToken(data,sep,start,tok))
		return NULL;
	BNode* root = new BNode(stoi(tok));
	queue<BNode*> q({root});
	while(true)
	{
		BNode* n = q.front();
		q.pop();

		if(!nextToken(data,sep,start,tok))
			break;
		if(tok != "null")
		{
			n->left = new BNode(stoi(tok));
			q.push(n->left);
		}
		if(!nextToken(data,sep,start,tok))
			break;
		if(tok != "null")
		{
			n->right = new BNode(stoi(tok));
			q.push(n->right);
		}
	}
	return root;
}

string CTree::serialize(Node* root)
{//level. null to indicate end of children.
	string s;
	stringstream ss;
	if(!root)
		return "";
	ss << "[";
	queue<Node*> q({root});
	while(!q.empty())
	{
		root = q.front();
		q.pop();
		if(root)
			ss << root->val << ",";
		else
			ss << "null,";
		for(int i=0; i<root->children.size();i++)
		{
			q.push(root->children[i]);
		}
		q.push(NULL);
	}
	s = ss.str();
	s = s.substr(0,s.length()-1);
	s += "]";
	return s;
}
Node* CTree::deserialize(string data)
{
	int start = 0;
	int end = 0;
	string sep = "[ ,]";
	string tok;
	if(!nextToken(data,sep,start,tok))
		return NULL;
	Node* root = new Node(stoi(tok));
	queue<Node*> q({root});
	while(true)
	{
		Node* n = q.front();
		q.pop();
		if(!nextToken(data,sep,start,tok))
			break;
		do{
			if(tok == "null")
				break;
			Node* t = new Node(stoi(tok));
			n->children.push_back(t);
			q.push(t);
		}while(nextToken(data,sep,start,tok));

	}
	return root;
}

int CTree::findInorder(int x, vector<int>&inorder, int l, int r)
{
	for(int i=l;i<=r; i++)
	{
		if(inorder[i] == x)
			return i;
	}
	return -1;
}
int CTree::findFirstPreNotIn(vector<int>&preorder, int preL, int preR, vector<int>&inorder, int inL, int inR)
{
	int l = preL;
	int r = preR;
	while(l<=r)
	{
		int i=l + (r-l)/2;
		if(-1 == findInorder(preorder[i], inorder, inL, inR))
			r = i-1;
		else
			l = i+1;
	}
	return l;
}

BNode* CTree::buildTree(vector<int>&preorder, int preL, int preR, vector<int>&inorder, int inL, int inR)
{
	if(preL > preR)
		return NULL;
	int val = preorder[preL];
	BNode* root = new BNode(val);
	int i = findInorder(val, inorder, inL, inR);
	int j = findFirstPreNotIn(preorder, preL+1, preR, inorder, inL, i-1);
	root->left = buildTree(preorder, preL+1,j-1,inorder, inL, i-1);
	root->right = buildTree(preorder, j, preR, inorder, i+1, inR);
	return root;
}

BNode* CTree::buildTree(vector<int>&preorder, vector<int>&inorder)
{
	return buildTree(preorder, 0,preorder.size()-1, inorder, 0, inorder.size()-1);
}

vector<int> CTree::preorderTraversalIterative(BNode* root)
{
	vector<int> v;
	if(!root)
		return v;
	stack<BNode*> s({root});
	while(!s.empty())
	{
		BNode* n = s.top();
		s.pop();
		v.push_back(n->val);
		if(n->right)
			s.push(n->right);
		if(n->left)
			s.push(n->left);
	}
	return v;
}

vector<int> CTree::inorderTraversalIterative(BNode* root)
{
	vector<int> v;
	if(!root)
		return v;
	stack<BNode*> s;
	while(root||!s.empty())
	{
		if(root)
		{
			s.push(root);
			root = root->left;
		}
		else
		{
			root = s.top();
			s.pop();
			v.push_back(root->val);
			root = root->right;
		}
	}
	return v;
}

vector<int> CTree::postorderTraversalIterative(BNode* root)
{
	vector<int> v;
	if(!root)
		return v;
	stack<BNode*> s;
	BNode* lastTraversed = NULL;
	while(root || !s.empty())
	{
		if(root)
		{
			s.push(root);
			root = root->left;
		}
		else
		{
			root = s.top();
			if(root->right && root->right != lastTraversed)
			{//has right. need to handle right first.
				root = root->right;
			}
			else
			{//no right. already no left. visit.
				s.pop();
				v.push_back(root->val);
				lastTraversed = root;
				root = NULL;
			}
		}
	}

}

vector<int> CTree::preorderTraversalIterative(Node* root)
{
	vector<int> v;
	if(!root)
		return v;
	stack<Node*> s({root});
	while(!s.empty())
	{
		root = s.top();
		s.pop();
		v.push_back(root->val);
		for(int i=root->children.size()-1; i>=0; i--)
			s.push(root->children[i]); //opposite order in stack. to make it normal order to traverse.
	}
	return v;
}
vector<int> CTree::postorderTraversalIterativeCheat(Node* root)
{//use preorder. then reverse in the end. reverse children in the middle.
	vector<int> v;
	if(!root)
		return v;
	stack<Node*> s({root});
	while(!s.empty())
	{
		root = s.top();
		s.pop();
		v.push_back(root->val);
		for(auto ch:root->children)
			s.push(ch);
	}
	reverse(v.begin(),v.end());
	return v;
}
vector<int> CTree::postorderTraversalIterative(Node* root)
{//actually output in the post order, not adjusting later on. no extra space, just one stack.
	vector<int> v;
	int curIndex = 0;//the index in parent of the current root..
	int lastTraversed = 0;

	stack<pair<Node*,int>> s; //pointer and index and count. i/c in the parent node.
	while(root||!s.empty())
	{
		if(root)
		{
			s.push(make_pair(root,curIndex));
			root = root->children.empty()? NULL : root->children[0];
			curIndex = 0;
		}
		else
		{
			root = s.top().first;
			if(!root->children.empty() && lastTraversed != root->children.size()-1)
			{	//still have children not visited.
				curIndex = lastTraversed+1;
				root = root->children[curIndex];
			}
			else
			{//no children or children all visited.
				lastTraversed = s.top().second;
				s.pop();
				v.push_back(root->val);
				root = NULL;
			}
		}
	}
	return v;
}


void CTree::binaryTreePaths(BNode* root, vector<string>& paths,int height, vector<int>& path )
{
	assert(root);
	if(!root->left && !root->right)
	{
		stringstream ss;
		for(int i=0; i<height; i++)
			ss << path[i] << "->";
		ss << root->val;
		paths.push_back(ss.str());
	}
	else
	{
		if(path.size() < height+1)
			path.push_back(root->val);
		else
			path[height] = root->val;
		if(root->left)
			binaryTreePaths(root->left, paths, height+1, path);
		if(root->right)
			binaryTreePaths(root->right, paths, height+1, path);
	}
}
vector<string> CTree::binaryTreePaths(BNode* root)
{
	vector<string> v;
	if(!root)
		return v;

	vector<int> path;
	binaryTreePaths(root,v,0,path);
	return v;
}

vector<vector<int>> CTree::zigzagLevelOrder(BNode* root)
{
	vector<vector<int>> v;
	if(!root)
		return v;
	queue<BNode*> q({root});
	while(!q.empty())
	{
		int size = q.size();
		v.push_back(vector<int>(size));
		for(int i=0; i<size; i++)
		{
			BNode* n = q.front();
			q.pop();

			if(v.size()&1)
				v[v.size()-1][i] = n->val;
			else
				v[v.size()-1][size-1-i] = n->val;
			if(n->left)
				q.push(n->left);
			if(n->right)
				q.push(n->right);
		}
	}
	return v;
}

int CTree::unilevels(BNode* root, int val, int level)
{
	if(!root)
		return level;
	if(root->val != val)
		return level;

	level++;

	int l = 0;
	int r = 0;
	if(root->left)
		l = unilevels(root->left, val, level);
	if(root->right )
		r = unilevels(root->right, val, level);
	return max(l,r);
}

int CTree::widthOfBinaryTree(BNode* root)
{
	if(!root)
		return 0;

	int w = 0;
	queue<pair<BNode*,int>> q({{root,0}});
	while(!q.empty())
	{
		int size = q.size();
		int l = 0;
		int r = 0;
		for(int i=0; i<size; i++)
		{
			auto n = q.front();
			q.pop();
			if(i==0)
				l = n.second;
			if(i==size-1)
				r = n.second;
			if(n.first->left)
				q.push(make_pair(n.first->left,n.second*2+1));
			if(n.first->right)
				q.push(make_pair(n.first->right, n.second*2+2));
		}
		w = max(w,r-l+1);
	}
	return w;
}

BNode* CTree::lowestCommonAncestor(BNode* root, BNode* p, BNode* q)
{
	if(!root || root == p || root == q)
		return root;

	BNode* l = lowestCommonAncestor(root->left,p,q);
	BNode* r = lowestCommonAncestor(root->right,p,q);
	return !l ? r : !r ? l : root;
}

BNode* CTree::subtreeWithAll(BNode* root, unordered_set<BNode*>& nodes)
{//find node that's in nodes.
	if(!root || nodes.find(root) != nodes.end())
		return root;
	BNode* l = subtreeWithAll(root->left,nodes);
	BNode* r = subtreeWithAll(root->right,nodes);
	return !l?r:!r?l:root;
}
BNode* CTree::subtreeWithAllDeepest(BNode* root)
{
	if(!root)
		return NULL;

	unordered_set<BNode*> nodes;
	queue<BNode*> q({root});
	while(!q.empty())
	{
		int size = q.size();
		nodes.clear();
		for(int i=0; i<size; i++)
		{
			BNode* n = q.front();
			q.pop();
			nodes.insert(n);

			if(n->left)
				q.push(n->left);
			if(n->right)
				q.push(n->right);
		}
	}
	return subtreeWithAll(root, nodes);
}

BNode* CTree::find(BNode* root, int val)
{
	if(!root)
		return NULL;
	if(root->val == val)
		return root;
	BNode* n = find(root->left,val);
	if( n )
		return n;
	n = find(root->right,val);
	return n;
}

int CTree::kthSmallest(BNode* root, int& k)
{//left, myself, right.
	cout << __FUNCTION__ << "(" << (root? root->val : 0) << "," << k << ")" << endl;
	if(!root)
		return 0;
	int x = kthSmallest(root->left,k); //inorder recursion.
	if( k == 0)
		return x;
	else
	{
		k--;
		if( k == 0)
			return root->val;
		return kthSmallest(root->right,k);
	}
}

bool CTree::isValidBST(BNode* root, BNode*& prev)
{//inorder traversal
	if(!root)
		return true;
	if( !isValidBST(root->left, prev))
		return false;

	if(prev && prev->val >= root->val)
		return false;

	prev = root;
	return isValidBST(root->right, prev);
}
bool CTree::isValidBST(BNode* root)
{
	if(!root)
		return true;
	BNode* prev = NULL;
	return isValidBST(root, prev);
}

bool CTree::isValidBST(BNode* root, int& low, int& high)
{	//post order traversal.
	bool lValid, rValid;
	int lLow, rLow, lHigh, rHigh;
	if(root->left && !isValidBST(root->left, lLow, lHigh))
		return false;
	if(root->right && !isValidBST(root->right, rLow, rHigh))
		return false;
	if(root->left && root->val < lHigh)
		return false;
	if(root->right && root->val > rLow)
		return false;

	//at this point. root is valid. update return.
	low = root->left? lLow : root->val;
	high = root->right? rHigh : root->val;
	return true;
}
void CTree::maxBST(BNode* root, bool& valid, int& size, BNode*& largest, int& low, int& high)
{//postorder traversal. ie. look left, look right, then self.
	bool lValid=true,rValid=true;
	int lSize=0, rSize=0;
	BNode* lLargest=NULL, *rLargest=NULL;
	int lLow, rLow;
	int lHigh, rHigh;
	if(root->left)
		maxBST(root->left, lValid, lSize, lLargest, lLow, lHigh);
	if(root->right)
		maxBST(root->right, rValid, rSize, rLargest, rLow, rHigh);

	valid = lValid && rValid && (!root->left || root->val >= lHigh) && (!root->right || root->val <= rLow);
	size = valid ? lSize + rSize + 1 : max(lSize,rSize);
	largest = valid ? root : lSize >= rSize ? lLargest : rLargest;
	low = valid ? root->left ? lLow : root->val : 0;//useless when invalid.
	high = valid ? root->right ? rHigh : root->val : 0;//useless when invalid.
}

int CTree::longestUnivaluePath(BNode* root)
{
	if(!root)
		return 0;
	int l,r,m;
	longestUnivaluePath(root, l,r,m);
	return m;
}

void CTree::longestUnivaluePath(BNode* root, int& l, int& r, int& m)
{//post order traversal
	if(!root|| (!root->left && !root->right) )
	{
		l=r=m = 0;
		return;
	}

	int ll,lr,lMax;
	int rl,rr,rMax;
	longestUnivaluePath(root->left, ll, lr, lMax);
	longestUnivaluePath(root->right,rl,rr,rMax);

	l = root->left && root->left->val == root->val ? max(ll,lr) + 1 : 0;
	r = root->right && root->right->val == root->val ? max(rl,rr) + 1 : 0;
	m = max(max(lMax,rMax), l+r);
}

int CTree::maxBST(BNode* root)
{
	if(!root)
		return 0;
	bool valid = false;
	int size, low, high;
	BNode* largest;
	maxBST(root, valid, size, largest, low, high);
	return size;
}

bool CTree::path(BNode* root, int n, stack<int>& p)
{//post order traversal. find first. then record from bottom up.
	if(!root)
		return false;
	if(root->val == n)
	{
		p.push(root->val);
		return true;
	}
	if(path(root->left, n,p))
	{
		p.push(root->val);
		return true;
	}
	else if(path(root->right, n, p))
	{
		p.push(root->val);
		return true;
	}
	return false;
}

void CTree::main()
{
	CTree tree;
	BNode* x,*y,*z;
	Node* X,*Y,*Z;
	vector<int> v;
	vector<int> level({4,2,6,1,3,5,7});
	vector<int> pre({4,2,1,3,6,5,7});
	vector<int> in({4,2,6,1,3,5,7});
	vector<int> post({1,3,2,5,7,6,4});

	X = tree.deserialize("[1,2,3,4,null,5,6,7,null,8,9,null,10,null,null,11,12,null]"); //level representation.
	v = tree.preorderTraversalIterative(X);
	v = tree.postorderTraversalIterative(X);

	y = tree.deserializeBT("[0,1,2,3,4,5,6,7,8,9]"); //level representation.
	stack<int> path;
	bool yes = tree.path(y, 6, path);
	while(!path.empty())
	{
		cout << path.top() << "->";
		path.pop();
	}

	y = tree.deserializeBT("[1,1,1,null,1,1,null,1,null,null,1]"); //level representation.
	int longest = tree.longestUnivaluePath(y);
	int maxBSTSize =  tree.maxBST(y);
	yes = tree.isValidBST(y);
	for(int i=1; i<=7; i++)
	{
		int k = i;
		int kth = tree.kthSmallest(y,k);
	}
	pre = tree.preorderTraversalIterative(y);
	in = tree.inorderTraversalIterative(y);
	post = tree.postorderTraversalIterative(y);

	x = tree.subtreeWithAllDeepest(y);
	BNode* p = tree.find(y,5);
	BNode* q = tree.find(y,1);
	z = tree.lowestCommonAncestor(y,p,q);
	int l = tree.unilevels(y->left,y->val,0);
	int r = tree.unilevels(y->right,y->val,0);
	x = tree.buildTree(pre, in);
	cout << tree.serializeBT(x) << endl;
	int w = tree.widthOfBinaryTree(x);
	cout <<"width=" << w << endl;
	vector<vector<int>> levels = tree.zigzagLevelOrder(x);
	vector<string> paths = tree.binaryTreePaths(x);
	vector<int> values(5);
	iota(values.begin(), values.end(), 0); //fill 0-99

	BNode* b = tree.buildRightTree(values);


	string s = tree.serializeBT(b);
	BNode* c = tree.deserializeBT(s);

	Node* root = tree.buildQuadTree(values);
	{
		CMeasure("levelOrder()");
		vector<vector<int>> levels = tree.levelOrder(root);
	}
	{
		CMeasure("levelOrderRecursive()");
		vector<vector<int>> levels2 = tree.levelOrderRecursive(root);
	}
}
