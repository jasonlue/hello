#include "json.h"
#include <sstream>
#include <string.h>
#include <assert.h>
#include <iostream>

const char* SkipSpace(const char* p)
    {
    while( isspace(*p) )
        p++;
    
    return p;
    }

JsonObject* LoadJson(const char* content)
    {
    JsonObject* obj = new JsonObject;
    const char* q = obj->Parse(SkipSpace(content));
    if( q && *q == 0)
        return obj;

    //extra at the end.
    delete obj;
    return NULL;
    }

JsonObject::~JsonObject()
    {//delete all values.
        for( auto kv: m)
            delete kv.second;
    }

string JsonObject::ToString()
    {
        stringstream ss;
        ss << "{";
        bool first= true;
        for( auto kv: m)
            {
            ss << (first?"":",") << '\"' << kv.first << "\":" << kv.second->ToString();
            first = false;
            }
        ss << "}";
        return ss.str();
    }

const char* JsonObject::Parse(const char* p)
    {
    //must start with {
    if( *p != '{')
        return NULL;
    //expect { <string> : <value>, <string> : <value>, ... }
    p = SkipSpace(p+1);

    while(true)
        {
        if( *p == '}')
            return SkipSpace(p+1);
        if( *p == ',' )
            {//, must follow one item, and must be followed by one item.
            if( m.empty() )
                return NULL;
            p = SkipSpace(p+1);
            }
        else
        {//have preceding item, but no , still bad.
            if( !m.empty())
                return NULL;
        }
        
        JsonString* k = new JsonString;
        p = k->Parse(p);
        if (!p || *p != ':')
            {
            delete k;
            return NULL;
            }
        p = SkipSpace(p+1);
        const char* end = p;
        JsonValue* v = JsonValue::ParseValue(p, end);
        if( !v )
            return NULL;
        m[k->s] = v;
        p = SkipSpace(end);
        }
    }

JsonArray::~JsonArray()
    {
        for( auto v : a)
            delete v;
    }
    
string JsonArray::ToString()
    {
    stringstream ss;
    ss << "[";
    bool first = true;
    for(auto v: a)
        {
        ss << (first ? "" : ",") << v->ToString();
        first = false;
        }
    ss << "]";
    return ss.str(); 
    }

const char* JsonArray::Parse(const char* p)
    { //expect [ <value>, <value>, ...]
    if( *p != '[')
        return NULL;
    p = SkipSpace(p+1);
    while(true)
        {
        if( *p == ']')
            return SkipSpace(p+1);
        if( *p == ',' )
            {//, must follow one item, and must be followed by one item.
            if( a.empty() )
                return NULL;
            p = SkipSpace(p+1);
            }
        else
        {//have preceding item, but no , still bad.
            if( !a.empty())
                return NULL;
        }
        const char* end = p;
        JsonValue* v = JsonValue::ParseValue(p, end);
        if( !v )
            return NULL;
        a.push_back(v);
        p = SkipSpace(end);
        }
    }

int Hex2Int(const char* p, const char* end)
    {
    int i = 0;
    while(p != end)
        {
        if( *p >= '0' && *p <= '9')
            i += *p - '0';
        else if( *p >= 'A' && *p <= 'F')
            i += *p - 'A' + 10;
        else if( *p >= 'a' && *p <= 'f') 
            i += *p - 'a' + 10;
        p++;
        i <<= 4;
        }
    return i;
    }

string JsonString::ToString()
    {
    stringstream ss;
    ss << '\"' << s << '\"';
    return ss.str();
    }
const char* JsonString::Parse(const char* p)
    {
    if( *p != '\"')
        return NULL;
    p++;
    stringstream ss;
    while(true)
        {
        if( *p != '\\')
            {
            if( *p == '\"')
                {
                p++;
                break; //end loop. the only exit.
                }
            ss << *p++;
            }
        else//escape seq.
            {
            p++;
            switch(*p)
            {
            case '\"': 
            case '\\':
            case '/': ss << *p++; break;
            case 'b': ss << '\b'; p++; break;
            case 'f': ss << '\f'; p++; break;
            case 'n': ss << '\n'; p++; break;
            case 'r': ss << '\r'; p++; break;
            case 't': ss << '\t'; p++; break;
            case 'u': //4 hex digits.
                p++;
                if( isxdigit(*p) && isxdigit(*(p+1)) && isxdigit(*(p+2)) && isxdigit(*(p+3)) )
                        ss << char(Hex2Int(p, p+4));
                else
                    return NULL;
                p += 4;
                break;
            default: 
                return NULL;
            }
            }
        }
    s = ss.str();
    return SkipSpace(p);
    }

string JsonNumber::ToString()
    {
    if(integer)
        return to_string(i);
    else
        return to_string(d);
    }

const char* JsonNumber::Parse(const char* p)
    {//expect [-]###.###[Ee]###
    const char* q = p;
    if( *q == '+' || *q == '-')
        q++;
    //1+ digit
    if( !isdigit(*q) )
        return NULL;
    q++;
    while(isdigit(*q))
        q++;

    bool exp = false;
    if(*q != '.' && *q != 'e' && *q != 'E')
        {//integer.
        integer = true;
        stringstream ss(string(p, q-p));
        ss >> i;
        return SkipSpace(q);
        }
    integer = false;
    if( *q == '.' )
        {
        q++;
        //0+ digits
        while(isdigit(*q))
            q++;
        }
    if( *q == 'e' || *q == 'E')
        {
        q++;
        //optiional - may exist.
        if( *q == '-' || *q == '+' )
            q++;
        //1+ digits.
        if( !isdigit(*q))
            return NULL;
        q++;
        while(isdigit(*q))
            q++;
        }
    stringstream ss(string(p, q-p));
    ss >> d;
    return SkipSpace(q);
    }

string JsonTrueFalseNull::ToString()
    {
    switch(type)
    {
    case True: return "true";
    case False: return "false";
    case Null: return "null";
    }
    return "";
    }

const char* JsonTrueFalseNull::Parse(const char* p)
    {//true,false,null
    const char* end = p; 
    while(isalpha(*end))
        end++;
    if( end - p == 5 && 0 == strncmp(p, "false", 5))
        type = JsonValue::False;
    else if( end - p == 4 && 0 == strncmp(p, "true", 4))
        type = JsonValue::True;
    else if( end - p == 4 && 0 == strncmp(p, "null", 4))
        type = JsonValue::Null;
    else
        return NULL;
    return SkipSpace(end);
    }

JsonValue* JsonValue::ParseValue(const char* p, const char*& end)
    {
    JsonValue* v = NULL;
    if( *p == '{')
        {
        v = new JsonObject;
        end = v->Parse(p);
        }
    else if( *p == '[')
        {
        v = new JsonArray;
        end = v->Parse(p);
        }
    else if( *p == '\"')
        {
        v = new JsonString;
        end = v->Parse(p);
        }
    else 
        {//have to try parsing
        v = new JsonTrueFalseNull;
        end = v->Parse(p);
        if( !end )
            {
            delete v;
            v = new JsonNumber;
            end = v->Parse(p);
            }
        }

    if( !end )
        {
        delete v;
        return NULL;
        }
    return v;
    }

void TestJson()
    {
        char* badJsons[] = {
            //common errors.
            R"()",
            R"({)",
            R"(})",
            R"({A:1})",
            R"({"A":"1")",
            //number errors
            R"({"A":.})",
            R"({"A":.3})",
            R"({"A":1.2.3})",
            R"({"A":e})",
            R"({"A":1.e})",
            R"({"A":0.e})",
            R"({"A":.+E2})",
            //object errors
            R"({,})",
            R"({,"A":1})",
            R"({"A":1,})",
            R"({"A":1 "B":2.0})",
            //array errors
            R"("A":[,])",
            R"("A":[,1])",
            R"("A":[1,])",
            R"("A":[1 1])",
        };
        for( auto j : badJsons)
            {
            cout << "json to be parsed: \n" << j << endl;
            JsonObject* o = LoadJson(j);
            assert(o == NULL);
            }
        char* goodJsons[] = {
            R"({})",
            R"({"A":-1})",
            R"({"A":+1, "B":"2", "C":true, "D":false, "E":null})", 
            //numbers
            R"({"A":0., "B":1., "C":0.1, "D":0.e1, "E":0e1, "F":1e2, "G": -0.1, "H": -1E-3, "I": +0.1E-2})",
            //strings
            R"({"A":"a", "B":"\\\/\t\r\n\b\f\"\u0044"})",
            R"({"A":1, "B":[]})",
            R"({"A":1, "B":[-2]})",
            R"({"A":1, "B":[-2,3]})",
            R"({"A":1, "B":{"C":3}})",
            R"({"A":1, "B":{"C":{"D":{"E":3}}}})",
            R"({"A":[{"B":2},{"C":3}]})",
            R"({"A":[[[[]]]]})",
            R"({"A":[[[[1]]]]})",
        };

        for(auto j: goodJsons)
            {
            cout << "json to be parsed: " << j << endl;
            JsonObject* o = LoadJson(j);
            assert(o != NULL);
            cout << "json dumped: " << o->ToString() <<endl; 
            }
    }