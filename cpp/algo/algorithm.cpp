/**
 *
 * \author Jason Lu
 * \version 1.0
 * \date 2018-10-13
 * \copyright GNU Public License
 * \mainpage algorithm - most data structures and it's related algorithms in the project.
 *
*/

#include <iostream>
#include <fstream>
#include <unordered_map>
//#include <cctype>
#include <algorithm>
using namespace std;

#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <syslog.h>

#include "CBst.h"
#include "AVL.h"
#include "CAlgo.h"
#include "CSortAlgo.h"
#include "CSearch.h"
#include "CRecursion.h"
#include "CTree.h"
#include <bits/stdc++.h>
#include "CArray.h"
#include "CLinkList.h"
#include "CIpc.h"
#include "CTrie.h"
#include "CExpression.h"
#include "json.h"
struct pair_hash
{
	size_t operator()(const pair<int,int>& p) const
	{
		return (p.first<<16) + p.second;
	}
};

void reverse(string& s, int l, int r)
{
	for(int i=l; i<=(l+r)/2; i++)
		swap(s[i],s[r-(i-l)]);
}

string reverseStr(string str, int k)
{
	string s = str;
	int num = s.length()/(k*2);
	int left = s.length()%(k*2);
	int i=0;
	for(; i<num; i++)
		reverse(s,i*2*k, i*2*k+k-1);
	reverse(s,i*2*k, i*2*k+min(k,left)-1);
	return s;
}

string reverseVowels(string s)
{
	string vows("aeiou");
	int l = 0;
	int r = s.length() - 1;
	while(true)
	{
		while(l<r && vows.find(s[l]) == string::npos)
			l++;
		while(r>l && vows.find(s[r]) == string::npos)
			r--;
		if(l>=r)
			break;
		swap(s[l],s[r]);
		l++;
		r--;
	}
	return s;
}

void test(int i)
{
	if(i==0)
		return ;
	test(--i);
}
int nmain();

void initDaemon()
{
	//for off the parent process. the program like a install program at the same time.
	pid_t pid = fork();
	if( pid < 0)
	{
		exit(EXIT_FAILURE);
	}
	else if( pid > 0)
	{//I am parent.
		cout << "in parent process" << endl;
		exit(EXIT_SUCCESS);
	}
	//0, this is the fresh forked process.
	cout << "in child process" << endl;
	umask(0);

	//create logs.

	//create a new SID for the child process.
	pid_t sid = setsid(); //make myself, the child process as the group lead. as the original group exits
	if( sid < 0 )
	{
		exit(EXIT_FAILURE);
	}
	//change current working directory
	if(chdir("/") < 0)
	{
		exit(EXIT_FAILURE);
	}
	//close 0,1,2 fd
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

bool running = true;
void sig_handler(int sig)
{
	static int c = 0;
	switch(sig)
	{
	case SIGUSR1: cout << "SIGUSR1 rcvd." << endl;break;
	case SIGUSR2: cout << "SIGUSR2 rcvd." << endl;break;
	case SIGINT:
	{
		cout << "SIGINT rcvd." << endl;
		running = false;
		break;
	}
	default: cout << "signal " << sig << "rcvd." << endl;break;
	}
}

vector<string> findItinerary( vector<pair<string,string>> tickets)
{
	vector<string> itinerary;
	if( tickets.empty())
		return itinerary;
	unordered_map<string, multiset<string>> graph;
	for(auto t:tickets)
		graph[t.first].insert(t.second);

	stack<string> dfs;
	dfs.push("JFK");
	while(!dfs.empty())
	{
		string topAirport = dfs.top();
		if(graph[topAirport].empty())
		{
			itinerary.push_back(topAirport);
			dfs.pop();
		}
		else
		{
			dfs.push(*(graph[topAirport].begin()));
			graph[topAirport].erase(graph[topAirport].begin());
		}
	}
	reverse(itinerary.begin(), itinerary.end());
	return itinerary;

}

void* operator new(size_t t)
	{
	void* v = malloc(t);
//	printf("new %x %d\n", v, t);
	return v;
	}

int main() {
	TestJson();
	
	int * p = (int*)malloc(128);
	int * q = new int;

	int a = (-2%2+2)%2;
	string s = "Hello World!";
	char c = toupper('c');
	transform(s.begin(), s.end(), s.begin(),::toupper);
	cout << s << endl;

	vector<pair<string,string>> tickets({{"JFK","SFO"},{"JFK", "ATL"}, {"ATL", "JFK"}, {"ATL", "SFO"}});
	vector<string> it = findItinerary(tickets);
//	daemon(0,0);
//	initDaemon();
//	umask(0);
	//save pid to file.
//	ofstream pidF;
//	pidF.open("/var/run/algo.d/algo.pid", ios::out|ios::trunc);
//	pidF << getpid() << endl;
//	pidF.close();

	if( signal(SIGUSR1, sig_handler) == SIG_ERR)
	{
		cout << "can't ache SIGUSR1!" << endl;
	}
	if( signal(SIGUSR2, sig_handler) == SIG_ERR)
	{
		cout << "can't ache SIGUSR2!" << endl;
	}
	if( signal(SIGINT, sig_handler) == SIG_ERR)
	{
		cout << "can't ache SIGINT!" << endl;
	}

	int i=0;
//	while(running)
	{
		syslog(LOG_INFO, "algo daemon running... %d .", i++);
//		sleep(30);//30seconds
	}

//	CIpc::main();
//	nmain();
//	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
//	string s = reverseVowels("hello");
//	cout << s << endl;
//	auto myHash = [](const pair<int,int>& p){return (p.first<<16) + p.second;};
//	unordered_map<pair<int,int>,int, pair_hash> m();

//	m[make_pair(1,1)] = 1;

//	CTrie::main();
//	CArray::main();
//	CLinkList::main();
//	CTree::main();
//	CRecursion::main();

//	CSearch::main();
//	AVL::Main();
//	CAvlBst::Main();
//	CAsio::Main();
//	CWorkerThread::Main();
//	CAlgo::main();
//	CSortAlgo::main();
	CExpression::main();
	return 0;
}



