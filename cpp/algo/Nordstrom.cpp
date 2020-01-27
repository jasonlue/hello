/*

Given a 64 bit integer, what is the fastest way to find the count of 1s in the binary representation of the number assuming no memory limitations? Imagine this function will be included in a Math library , and can be called thousands of time , and needs to be as fast as possible across all those calls.


Given 2 sorted arrays, where the 2nd array has room at the end for all of the entries in the 1st array, how do you merge the 1st array into the 2nd array?

Given employee (id, name, email) and manager info,how can you find the peers of an employee and their manager's peers?


rajesh.honnawarkar@nordstrom.com

*/

//n & (n-1)

//if n is all 1's

//A[1,3,5,7,0,0,0]  i=0
//B[2,4,6] j

//j=0 i=1 [3,5,7] ->


#include <iostream>
#include <vector>
#include <queue>
#include <sstream>
#include <assert.h>

using namespace std;

struct TreeNode
{
    int id;
    string name;
    string email;
    vector<TreeNode*> employees;
    TreeNode* manager;

    TreeNode(int id, string& name, string& email, TreeNode* manager=NULL) : id(id),name(name),email(email),manager(manager){};
};


bool nextGroup(string& s, int& start, string& group)
{
  int i = s.find_first_of('{', start);
  if( i == string::npos)
    return false;
  int j = s.find_first_of('}', i+1);
  if( j == string::npos )
    return false;
  group = s.substr(i+1,j-i-1);
  start = j+1;
  return true;
}

bool nextToken(string& s, int& start, string& token)
{
  string sep(",");
  int i = s.find_first_not_of(sep, start);
  if( i == string::npos)
    return false;
  int j = s.find_first_of(sep, i);
  token = s.substr(i,j-i);
  start = j;
  return true;
}

bool parse(string& s, int& id, string& name, string&email)
{
  id = 0;
  name = "";
  email="";
  int start = 0;
  string idString;
  if(!nextToken(s, start, idString) || !nextToken(s,start,name))
    return false;
  id = stoi(idString);
  nextToken(s,start, email); //email optional
  return true;
}

TreeNode* deserialize(string s)
{
  int start = 0;
  string group;
  if(!nextGroup(s,start,group))
    return NULL;

  int id = 0;
  string name, email;
  if(!parse(group, id, name, email))
    return NULL;

  TreeNode* root = new TreeNode(id,name,email);
  queue<TreeNode*> q({root});
  while(!q.empty())
  {
    TreeNode* n = q.front();
    q.pop();
    while(nextGroup(s,start,group))
    {
      if(!parse(group, id, name, email))
        break; //reach the end of children.

      TreeNode* employee = new TreeNode(id,name,email, n);
      n->employees.push_back(employee);
      q.push(employee);
    }
  }
  return root;
}

string serialize(TreeNode* root)
{
  if(!root)
    return "[]";
  stringstream ss;
  ss << "[";
  queue<TreeNode*> q({root});
  while(!q.empty())
  {
    int size = q.size();
    for(int i=0; i<size; i++)
    {
      TreeNode* n = q.front();
      q.pop();
      if(n)
      {
        ss << "{" << n->id << "," << n->name << "," << n->email << "},";
        for(auto e: n->employees)
        {
          q.push(e);
        }
        q.push(NULL); //indicate ends.
      }
      else
        ss << "{},";
    }
  }
  string s = ss.str();
  s.pop_back();
  return s + "]";
}

TreeNode* find(TreeNode* root, int id)
{
  if(!root)
    return NULL;

  if(root->id == id)
    return root;

  for(auto employee : root->employees)
  {
    TreeNode* n = find(employee, id);
    if( n )
      return n;
  }
  return NULL;
}

vector<string> findPeers(TreeNode* employee)
{
  vector<string> peers;
  if(!employee || !employee->manager)
    return peers;
  for(auto e: employee->manager->employees)
  {
    if(e != employee)
    {
      peers.push_back(e->name);
    }
  }
  return peers;
}

vector<string> findPeers(TreeNode* root, int employeeId)
{
  return findPeers(find(root,employeeId));
}

vector<string> findManagerPeers(TreeNode* root, int employeeId)
{
  vector<string> peers;
  TreeNode* employee = find(root, employeeId);
  if(!employee)
    return peers;
  return findPeers(employee->manager);
}


void print(vector<int>& A)
{
  for( auto a: A)
    cout << a << ",";
  cout << endl;
}
void merge2SortedArrays(vector<int>& A, int aSize, vector<int>& B)
{
  assert(A.size() >= aSize + B.size());
  int i=0;
  for(int j=0; j<B.size(); j++)
  {
    while(i < aSize && A[i] < B[j])
      i++;
    for(int k=aSize; k>i; k--)
    {
      A[k] = A[k-1];
    }
    A[i] = B[j];
    aSize++;
  }
}

int  count1s(long long n)
{
  int count = 0;
  while(n)
  {
    n = n&(n-1);
    count++;
  }
  return count;
}

void print( vector<string>& v)
{
	for(auto i:v)
		cout << i << ",";
	cout << endl;
}

// To execute C++, please define "int main()"
int nmain() {

  TreeNode* root = deserialize("[{1,1,e.m@gmail.com},{2,2, r.h@nordstrom.com},{3,3,am@hotmail.com},{},{4,4,jasonlue@hotmail.com},{5,5,},{6,6,6@6.com}]");
  cout << serialize(root) << endl;

  for(int i=0; i<10; i++)
  {
	  vector<string> peers = findPeers(root, i);
	  cout << "peers of id: " << i << endl;
	  print(peers);
	  vector<string> managers = findManagerPeers(root, i);
	  cout << "manager  peers of id: " << i << endl;
	  print(managers);
  }
  vector<int> A({8,9,10,11,0,0,0});
  vector<int> B({2,3,7});

  merge2SortedArrays(A, 4, B);
  print(A);

  long long i = 0xFFFFFFFFFFFFFF;
  cout << "count of 1s: " <<  count1s(i) << endl;

  auto words = { "Hello, ", "World!", "\n" };
  for (const string& word : words) {
    cout << word;
  }
  return 0;
}



