#include <unordered_map>
#include <vector>
#include <string>
using namespace std;

class JsonValue{
public:
    enum {Object = 'O', Array= 'A', String = 'S', Number='#', True='T', False='F', Null};
    char type;
    //parse true, false, null.
    JsonValue(int t=0) : type(t){}
    //generic parser wrapper.
    static JsonValue* ParseValue(const char* begin, const char*& end);
    virtual const char* Parse(const char* begin) = 0; //return the end of parsing, trimming following space. NULL if error occurs.
    virtual string ToString() = 0;
    virtual ~JsonValue(){};
};

class JsonObject : public JsonValue{
public:
    unordered_map<string,JsonValue*>  m;

    JsonObject() : JsonValue(Object){}
    virtual const char* Parse(const char* begin) override; //return the end of parsing. NULL if error occurs.
    virtual string ToString() override;
    virtual ~JsonObject();
};

class JsonArray: public JsonValue{
public:
    vector<JsonValue*> a;
    JsonArray() : JsonValue(Array){}
    virtual const char* Parse(const char* begin) override; //return the end of parsing. NULL if error occurs.
    virtual string ToString() override;
    virtual ~JsonArray();
};

class JsonString : public JsonValue{
public:
    string s;

    JsonString() : JsonValue(String){}
    JsonString(string s) : JsonValue(String), s(s){}
    virtual const char* Parse(const char* begin) override; //return the end of parsing. NULL if error occurs.
    virtual string ToString() override;
    virtual ~JsonString(){}
};

class JsonNumber : public JsonValue{
protected:
    bool integer;
    union {__int64_t i; double d;};    
public:
    JsonNumber() : JsonValue(Number){}
    JsonNumber(__int64_t i) : JsonValue(Number), integer(true), i(i){}
    JsonNumber(double d) : JsonValue(Number), integer(false), d(d){}
    virtual const char* Parse(const char* begin) override; //return the end of parsing. NULL if error occurs.
    virtual string ToString() override;
    virtual ~JsonNumber(){}
};

class JsonTrueFalseNull : public JsonValue{ 
public:
    JsonTrueFalseNull(){}
    JsonTrueFalseNull(int type) : JsonValue(type){}
    virtual const char* Parse(const char* begin) override; //return the end of parsing. NULL if error occurs.
    virtual ~JsonTrueFalseNull(){}
    virtual string ToString() override;
};

JsonObject* LoadJson(const char* content);
void TestJson();