/*
 * CTrie.h
 *
 *  Created on: Oct 10, 2018
 *      Author: jason
 */

#ifndef CTRIE_H_
#define CTRIE_H_
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
struct TrieNode
{
	int val;
	bool endOfWord;
	unordered_map<char,TrieNode*> children;
	TrieNode():endOfWord(false),val(0){};
};
struct BinaryNode
{
    BinaryNode* children[2]; //[0] digit 0 [1] digit 1
    BinaryNode(): children({NULL,NULL}){}
};
class CTrie {
protected:
	TrieNode* root;
	TrieNode* suffixRoot;
	BinaryNode* binaryRoot;
public:
	CTrie();
	virtual ~CTrie();

	void buildDict(vector<string> dict);
	//it's naturally fit in trie for this one character difference.
	bool search(string word);
	bool searchMagic(string word, int start, int mismatches, TrieNode* p);
	bool searchMagic(string word); //676: search word with exactly one character into another character in this word.

	void buildFilter(vector<string>& dict);
	void words(vector<int>& weights, TrieNode* p);
	TrieNode* search(string prefix, TrieNode* root);
	int filter(string prefix, string suffix);

	void buildBinary(vector<int>& nums);
	int findMaximumXOR(vector<int>& nums);

	void allPrefixes(string word, int start, vector<int>& prefixes, TrieNode* p);
	bool isConcatenated(string w, int start);
	vector<string> findAllConcatenatedWordsInADict(vector<string>& words);

public:
	static void findAllConcatenatedWordsInADict();
	static void searchMagic();
	static void findMaximumXOR();
	static void filter();
	static void main();



};

#endif /* CTRIE_H_ */
