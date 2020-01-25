type Point : record{
    x : int;
    y : int;
};

type MyEnum : enum {A1,A2,A3,};
redef enum MyEnum +={B1,B2};
type YourEnum : enum{A1=11, A2=22};

type MyVec : vector of int;
type MyMap : table[int] of string;
type MySet : set[int];
type MyStr : string;

redef record Point+= {
    z : int &optional;
};

function full(i : int &default=0) : string &priority=10
{
    print i;
    return fmt("%d", i);
}

function hello(s:string)
{
    print "hello " + s;
}
global _hello = hello;
global Hello :function(s : string);
global HELLO = function(s:string) {print "HELLO " + s;};

global f1i : string = "abc";
module m;
global f1i : count = 1;

function f1(f1i:count)
{
    print "f1()", ++f1i;
#    if (f1i<10)
#        f1(1);
}

event bro_init() &priority=1
{
    print @DIR,@FILENAME;
    print fmt("bro_init() event handler. priority=1");
}

const maxInt = 2222;
event bro_init() &priority=2
	{ 
    print "another bro_init() event handler. priority=2";    

    const maxInt2 = 1111;
	local a: MyEnum =A1;
	local b: YourEnum = A1;
	local c = A1;
	print a,b,c;
	
	local p: Point = record($y = +2, $x = +1);
	local q = Point($x=+1,$y=-1);
	local r: Point = [$x=+10, $y=+10];
	local rr: Point;
	rr = [$x=+10, $y=+10];
	print p$x, p$y, p,q,r;
	


	#vector
	local w : vector of int = [1,2,3];
	local v = vector(1,2,3);
	v[3] = 3;
	++v[1];
	--v[2];

    print v[0], |v|, v;
    
    for(item in v)
	    print item;
	    
	#set
	local s : set[count] = [1,2,3];
	local ss : set[count,string] = {[1,"1"],[2,"2"],[3,"3"]};
	local t = set(1,2,3);
	local u = [1,2,3];
	add s[0];
	add ss[1,"1"];
	delete t[1];
	print ss,s,t,u;
	
	for([it1,it2] in ss)
	    print it1,it2;
	    
	#map
	local m :table[string,string,string] of int = { ["1","2","3"] = 1, ["2","4","6"] = 2};
	local n = table(["3"]=3, ["4"] = 4);

	m["100", "200", "300"] = 100;
	delete n["3"];
	print m["1", "2","3"], m,n;
	
	print "items in map n:";
	for([k] in n)
	    print k, "->", n[k];
    print "=======================";
    
    print "items in map m:";
	for([x1,x2,x3] in m)
	    print x1,x2,x3, "->", m[x1,x2,x3];
    print "=======================";
    
    
	local i = 1;
	++i;
	print i;
	
	local str = "abcde";
	print str[1:4], str[1:], str[:3], str[:];

    const pi = 3.14;

    local j : counter = 9;
    print "j=", j;
    
    
    local ii=0;
    while(ii<10)
    {
        print ii;
        ++ii;
    }
	}

type MyFunc : function(s:string);

global f0 : function(i: int &default=2222, j:int &default=3333);
function f0(i:int, j:int &default=1)
{
    print fmt("f0.2(%d,%d)", i,j);
}

event test(i:int, j:int)
{#no return value.
    print fmt("event test handler1(%d,%d)",i,j);
    
    if( /abc/|/def/ in "abc")
    {
        print "Cool!";
    }
}

event test(i:int, j:int)
{#no return value.
    print fmt("event test handler2(%d,%d)",i,j);
}

hook myHook(s:string) &priority=6
{#no return value.
    print "priority 6 hook ", s;
    s = "bye";
}

hook myHook(s:string)
{
    print "break out of my hook handling ", s;
    break;
}

hook myHook(str:string) &priority=-5
{
    print "not going to happen ", str;
}

hook myHook(str:string) &priority=-5
{
    print "not going to happen ", str;
}

type yourHook : hook(s:string);

event bro_done() 
	{
	print "=================================================";
    print "event bro_done()";
	print "=================================================";
	f0();
	f1i = 666;    
	print f1i;
    f1(1);
    print f1i;
	_hello("_world!");
    hello("world!");
    Hello = hello;
    Hello("World!");
    HELLO("WORLD!");
    
    event test(1,2);
#    schedule 600 secs {test(2,3)};
    
    hook myHook("hello hook!");
#    hook yourHook("yours");    
	local x = 1;
	when(0 ==x%2) #same as if if no async function is evaluated.
	{
	    print x;
	}
	timeout 5sec
	{
	    print "timeout";
	}   
	while(x<10)
	    ++x;
	    
	local i = 5;
	local j = copy(i);
	print i,j;
	switch(i)
	{
	case 1:
	    print "1";
	    break;
	case 2: fallthrough;
	case 3:
	    print "23";
        break;
        
	case 4,5,6: 
	    print "456";
	    break;
	    
	default:
	    print "default";
	    break;
	}
	
	print "Well done!"; 
	}
