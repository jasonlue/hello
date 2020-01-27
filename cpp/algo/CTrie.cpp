/*
 * CTrie.cpp
 *
 *  Created on: Oct 10, 2018
 *      Author: jason
 */
#include <algorithm>
#include <iostream>
#include "CTrie.h"

CTrie::CTrie() {

}

CTrie::~CTrie() {
	// TODO Auto-generated destructor stub
}

void CTrie::buildDict(vector<string> dict)
{
	root = new TrieNode();
	for(auto w:dict)
	{
		TrieNode* p = root;
		for(auto ch:w)
		{
			if(!p->children.count(ch))
			{
				p->children[ch] = new TrieNode;
			}
			p = p->children[ch];
		}
		p->endOfWord = true;
	}
}

bool CTrie::search(string word)
{
	TrieNode* p = root;
	for(int i=0; i<(int)word.length(); i++)
	{
		if( p->children.count(word[i]))
			p = p->children[word[i]];
		else
			return false;
	}
	return p->endOfWord;
}

bool CTrie::searchMagic(string word, int start, int mismatches, TrieNode* p)
{//DFS, with early cutoffs.
	if(mismatches > 1)
		return false; //early cut off.

	if(start == (int)word.length()) //normal end.
		return p->endOfWord && mismatches == 1;

	for(auto child : p->children)
	{
		if( child.first == word[start])
		{//match
			if( searchMagic(word, start+1, mismatches, p) )
				return true;//early return. one match is enough.
		}
		else
		{
			if(searchMagic(word, start+1, mismatches+1, p))
				return true;//early return. one match is enough.
		}
	}
	return false;
}
bool CTrie::searchMagic(string word)
{
	return searchMagic(word, 0, 0, root);
}

void CTrie::buildFilter(vector<string>& words)
{
    root = new TrieNode();
    suffixRoot = new TrieNode();
    for(int i=0; i<words.size(); i++)
    {
        TrieNode* p = root;
        for(auto ch : words[i])
        {//prefix trie
            if(!p->children.count(ch))
                p->children[ch] = new TrieNode();
            p = p->children[ch];
        }
        p->endOfWord = true;
        p->val = i;

        p = suffixRoot;
        for(int j=words[i].length()-1; j>=0; j--)
        {
            char ch = words[i][j];
            if(!p->children.count(ch))
                p->children[ch] = new TrieNode();
            p = p->children[ch];
        }
        p->endOfWord = true;
        p->val = i;
    }
}

void CTrie::words(vector<int>& weights, TrieNode* p)
{//preorder traversal
    if(p->endOfWord)
        weights.push_back(p->val);
    for(auto ch:p->children)
        words(weights,ch.second);
}

TrieNode* CTrie::search(string fix, TrieNode* root)
{
    TrieNode* p = root;
    for(auto ch:fix)
    {
        if(!p->children.count(ch))
            return NULL;
        p = p->children[ch];
    }
    return p;
}
int CTrie::filter(string prefix, string suffix)
{
    TrieNode* pre = search(prefix, root);
    if(!pre)
        return -1;

    reverse(suffix.begin(), suffix.end());
    TrieNode* suf = search(suffix, suffixRoot);
    if(!suf)
        return -1;

    //get all weights in prefix and suffix.
    //sort them
    vector<int> prefixes;
    words(prefixes, pre);
    sort(prefixes.begin(), prefixes.end());

    vector<int> suffixes;
    words(suffixes, suf);
    sort(suffixes.begin(), suffixes.end());

    //find the max which exists in both.
    int i=prefixes.size()-1;
    int j=suffixes.size()-1;
    while(i>=0 && j>=0)
    {
        if(prefixes[i] == suffixes[j])
            return prefixes[i];
        else if( prefixes[i] > suffixes[j])
            i--;
        else
            j--;
    }
    return -1;
}

void CTrie::buildBinary(vector<int>& nums)
{
    binaryRoot = new BinaryNode();
    for(auto n:nums)
    {   //from msb to lsb.
        BinaryNode* p = binaryRoot;
        for(int i=0; i<32; i++)
        {
            if(n>=0)
            {//highest bit 0
                if(!p->children[0])
                    p->children[0] = new BinaryNode();
                p = p->children[0];
            }
            else
            {//highest bit 1
                if( !p->children[1])
                    p->children[1] = new BinaryNode();
                p = p->children[1];
            }
            n <<=1;
        }
    }
}
int CTrie::findMaximumXOR(vector<int>& nums)
{//for every n in nums, find the other m to make n^m max. going through all n to get the global max.
    buildBinary(nums);
    int globalMax = 0;
    for(int i=0; i<nums.size(); i++)
    {
    	int n = nums[i];
        BinaryNode* p = binaryRoot;
        int localMax = 0;
        for(int i=0; i<32; i++)
        {
            localMax <<=1;
            if(n>=0)
            {//MSB 0, find 1.
                if(p->children[1])
                {
                    localMax |= 1;
                    p = p->children[1];
                }
                else
                {
                    p = p->children[0];
                }
            }
            else
            {//MSB 1, find 0
                if(p->children[0])
                {
                    localMax |= 1;
                    p = p->children[0];
                }
                else
                {
                    p = p->children[1];
                }
            }
            n<<=1;
        }
        globalMax = max(globalMax, localMax);
    }
    return globalMax;
}

void CTrie::allPrefixes(string word, int start, vector<int>& prefixes, TrieNode* p)
{//DFS to find all prefixes. prefixes stores the length of each prefix.
    if(p->endOfWord )
        prefixes.push_back(start);
    if( start == word.length())
        return;
    if(!p->children.count(word[start]))
        return; //cut off early
    allPrefixes(word, start+1, prefixes, p->children[word[start]]);
}

bool CTrie::isConcatenated(string w, int start)
{//recursion.
    vector<int> prefixes;
    allPrefixes(w, start, prefixes, root);
    if(prefixes.empty())
        return false;
    if(start == 0 && prefixes.size() == 1)
        return false;
    sort(prefixes.begin(), prefixes.end(), greater<int>());
    for(int i=0; i<prefixes.size(); i++)
    {
        if(prefixes[i] == w.length())
        {
            if( start != 0) //not self.
                return true;
        }
        else
        {
            if( isConcatenated(w, prefixes[i]))
                return true;
        }
    }
    return false;
}
vector<string> CTrie::findAllConcatenatedWordsInADict(vector<string>& words)
{
    vector<string> res;
    buildDict(words);
    for(auto w : words)
    {
        if(isConcatenated(w,0))
            res.push_back(w);
    }
    return res;
}

void CTrie::findAllConcatenatedWordsInADict()
{
	CTrie t;
	vector<string> words =
		{"cat","cats","catsdogcats","dog","dogcatsdog","hippopotamuses","rat","ratcatdogcat"};
	t.buildDict(words);
	vector<string> res = t.findAllConcatenatedWordsInADict(words);
	for(auto r : res)
		cout << r << ",";
	cout << endl;
}
void CTrie::filter()
{
	CTrie trie;
	vector<string> words({"abbbababbb","baaabbabbb","abababbaaa","abbbbbbbba","bbbaabbbaa","ababbaabaa","baaaaabbbb","babbabbabb","ababaababb","bbabbababa"});
	trie.buildFilter(words);
	cout << trie.filter("babbab", "") << endl;
}

void CTrie::findMaximumXOR()
{
	CTrie t;
	vector<int> nums = {1};
	t.findMaximumXOR(nums);
}

void CTrie::main()
{
	findAllConcatenatedWordsInADict();
	findMaximumXOR();
	filter();
}
