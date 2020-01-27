/*
 * CTree.h
 *
 *  Created on: Sep 6, 2018
 *      Author: jason
 */

#ifndef CTREE_H_
#define CTREE_H_
#include <vector>
#include <unordered_set>
#include <string>
#include <stack>
using namespace std;
struct Node
{
	int val = NULL;
	vector<Node*> children;
	Node(int val): val(val){}
	Node(int _val, vector<Node*> _children): val(_val), children(children){	}
};

struct BNode
{
	int val = NULL;
	BNode* left;
	BNode* right;
	BNode(int val): val(val),left(NULL),right(NULL){}
};

class CTree {
public:
	CTree();
	virtual ~CTree();
	BNode* buildRightTree(vector<int>&v);
	Node* buildQuadTree(vector<int>& v);
	vector<vector<int>> levelOrderRecursive(Node* root);
	vector<vector<int>> levelOrder(Node* root);
	vector<vector<int>> zigzagLevelOrder(BNode* root);

	vector<int> preorderTraversalIterative(BNode* root);
	vector<int> inorderTraversalIterative(BNode* root);
	vector<int> postorderTraversalIterative(BNode* root);
	vector<int> preorderTraversalIterative(Node* root);
	vector<int> postorderTraversalIterativeCheat(Node* root);
	vector<int> postorderTraversalIterative(Node* root);

	int kthSmallest(BNode* root, int& k);
	vector<string> binaryTreePaths(BNode* root);
	void binaryTreePaths(BNode* root, vector<string>& paths,int height, vector<int>& path );
	string serialize(Node* root);
	Node* deserialize(string data);

	string serializeBT(BNode* root);
	BNode* deserializeBTBoost(string data);
	BNode* deserializeBT(string data);

	BNode* buildTree(vector<int>&preorder, vector<int>&inorder);
	int unilevels(BNode* root, int val, int level);

	int widthOfBinaryTree(BNode* root);
	BNode* lowestCommonAncestor(BNode* root, BNode* p, BNode* q);
	BNode* find(BNode* root, int val);

	BNode* subtreeWithAll(BNode* root, unordered_set<BNode*>& nodes);
	BNode* subtreeWithAllDeepest(BNode* root);
	bool isValidBST(BNode* root);
	void maxBST(BNode* root, bool& valid, int& size, BNode*& largest, int& low, int& high);
	int maxBST(BNode* root);

	void longestUnivaluePath(BNode* root, int& l, int& r, int& max);
	int longestUnivaluePath(BNode* root);

	bool path(BNode* root, int n, stack<int>& p );
	static void main();
private:
	bool isValidBST(BNode* root, BNode*& prev);
	bool isValidBST(BNode* root, int& low, int& high);
	void levelOrderRecursive(Node* root, int height, vector<vector<int>>& levels);

	int findInorder(int x, vector<int>&inorder, int l, int r);
	int findFirstPreNotIn(vector<int>&preorder, int preL, int preR, vector<int>&inorder, int inL, int inR);
	BNode* buildTree(vector<int>&preorder, int preL, int preR, vector<int>&inorder, int inL, int inR);

};

#endif /* CTREE_H_ */
