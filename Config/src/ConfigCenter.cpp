#include "ConfigCenter.h"
#include "Log.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/error/en.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <cstdio>

using namespace nd;
using namespace rapidjson;
using namespace std;

//-----------------------------------------------------------------------------

ConfigCenter::ConfigCenter()
    : documentM(NULL)
{
    load("config.json");
}

//-----------------------------------------------------------------------------

ConfigCenter::~ConfigCenter()
{
    clear();
}

//-----------------------------------------------------------------------------

void ConfigCenter::clear()
{
    unique_lock<shared_mutex> lock(mapMutexM);
    paramMapM.clear();
    if (documentM != NULL){
        delete documentM; 
        documentM = NULL;
    }
}

//-----------------------------------------------------------------------------

void ConfigCenter::set(Document* doc, ParamMap& map)
{
    clear();
    unique_lock<shared_mutex> lock(mapMutexM);
    paramMapM.clear();
    documentM = doc;
    paramMapM = map;
}


//-----------------------------------------------------------------------------

int ConfigCenter::load(const std::string& thePath)
{
    FILE* fp = fopen(thePath.c_str(), "r");
    if (fp == NULL){
        CFG_ERROR( "failed to open file:" << thePath << ", errno:" << errno);
        return -1;
    }

    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    auto doc = new Document();
    ParseResult ok = doc->ParseStream(is);
    if (!ok){
        fclose(fp);
        CFG_ERROR( "JSON parse error: " << GetParseError_En(ok.Code()) << ", offset: " << ok.Offset());
        return -1;
    }
    fclose(fp);

    ParamMap paramMap;
    StringBuffer sb;
    Writer<StringBuffer> writer(sb);
    for (auto it = doc->MemberBegin(); it != doc->MemberEnd(); ++it) {
        string cfgId = it->name.GetString();
        Value& cfgDef = it->value;
        Value* value = &cfgDef["val"];
        value->Accept(writer);    // Accept() traverses the DOM and generates Handler events.
        paramMap[cfgId] = value;
        CFG_DEBUG("get config: " << cfgId << "=[" << sb.GetString() << "]");
        writer.Reset(sb);
        sb.Clear();
    }

    set(doc, paramMap);
    return 0;

}

//-----------------------------------------------------------------------------

int ConfigCenter::get(const std::string& theKey, const int theDefault)
{
    shared_lock<shared_mutex> lock(mapMutexM);
    ParamMap::iterator it = paramMapM.find(theKey);
    if (it != paramMapM.end())
    {
        return it->second->GetInt();
    }
    else
    {
        return theDefault;
    }
}

//-----------------------------------------------------------------------------

const std::string ConfigCenter::get(const std::string& theKey, const std::string& theDefault)
{
    shared_lock<shared_mutex> lock(mapMutexM);
    ParamMap::iterator it = paramMapM.find(theKey);
    if (it != paramMapM.end())
    {
        return it->second->GetString();
    }
    else
    {
        return theDefault;
    }
}

//-----------------------------------------------------------------------------

StrVector ConfigCenter::getStrVector(const std::string& theKey)
{
    shared_lock<shared_mutex> lock(mapMutexM);
    ParamMap::iterator it = paramMapM.find(theKey);
    StrVector v;
    if (it == paramMapM.end()) {
        return v;
    }

    Value& val = *it->second;
    for (SizeType i = 0; i < val.Size(); i++){
        v.push_back(val[i].GetString());
    }
    return v;
}

//-----------------------------------------------------------------------------

IntVector ConfigCenter::getIntVector(const std::string& theKey)
{
    shared_lock<shared_mutex> lock(mapMutexM);
    ParamMap::iterator it = paramMapM.find(theKey);
    IntVector v;
    if (it == paramMapM.end()) {
        return v;
    }

    Value& val = *it->second;
    for (SizeType i = 0; i < val.Size(); i++){
        v.push_back(val[i].GetInt());
    }
    return v;
}

