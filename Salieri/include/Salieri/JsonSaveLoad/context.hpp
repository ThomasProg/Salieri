#pragma once

#include <Salieri/salieri.hpp>
#include <Salieri/object.hpp>
#include <Salieri/JsonSaveLoad/fwd.hpp>
#include <Salieri/JsonSaveLoad/json.hpp>

#include <stack>

namespace slr
{
class BaseJsonContext
{
protected:
    nlohmann::json mainJson;
    std::stack<nlohmann::json*> jsons; // does not contain null values

public:
    nlohmann::json& getLastJsonObject()
    {
        return *jsons.top();
    }

    std::string getJson()
    {
        return mainJson.dump(3);
    }

    void pushCategory(const std::string& name)
    {
        nlohmann::json& newJson = getLastJsonObject()[name];
        if (newJson.is_discarded())
        {
            newJson = nlohmann::json::object();
        }
        jsons.emplace(&newJson);
    }

    void popCategory()
    {
        jsons.pop();
    }

    BaseJsonContext()
    {
        jsons.emplace(&mainJson);
    }
};

class JsonSaveInfo
{
    std::string name;

public:
    JsonSaveInfo(const std::string& name) : name(name)
    {

    }

    std::string getName() const
    {
        return name;
    }
};

// Must be thread safe
class JsonSaveContext : public BaseJsonContext
{
public:
    class DefaultSaveExtension& shared;

public:

    // void save(const void* bytes, size_t nbBytes);
    // void saveString(const std::string& str);
    // void savePair(const std::string& key, const std::string& value);



    template<typename T>
    void save(const std::string& name, const T& value)
    {
        getLastJsonObject()[name] = value;
    }

    JsonSaveContext(DefaultSaveExtension& sharedContext);
};

class DefaultSaveExtension
{  
public:
    JsonSaveContext globalContext = JsonSaveContext(*this);

    virtual ~DefaultSaveExtension() = default;
};

template<typename DATA_TYPE>
struct Serialize<DATA_TYPE, DefaultSaveExtension, JsonSaveInfo>
{
    void operator()(DATA_TYPE& data, DefaultSaveExtension& context, const JsonSaveInfo& info)
    {
        slr::serialize(data, context.globalContext, info);
    }
};  

class JsonLoadInfo
{
    std::string name;

public:
    JsonLoadInfo(const std::string& name) : name(name)
    {

    }

    std::string getName() const
    {
        return name;
    }
};

class JsonLoadContext : public BaseJsonContext
{
public:
    class DefaultLoadExtension& shared;

public:
    template<typename T>
    void load(const std::string& name, T& value)
    {
        getLastJsonObject()[name].get_to(value);
    }

    bool isNull(const std::string& name)
    {
        return getLastJsonObject()[name].is_null();
    }

    void parse(const std::string& jsonStr)
    {
        mainJson = nlohmann::json::parse(jsonStr);
    }

    JsonLoadContext(DefaultLoadExtension& sharedContext);
};

class DefaultLoadExtension
{

public:
    JsonLoadContext globalContext = JsonLoadContext(*this);
    virtual ~DefaultLoadExtension() = default;

};

template<typename DATA_TYPE>
struct Serialize<DATA_TYPE, DefaultLoadExtension, JsonLoadInfo>
{
    void operator()(DATA_TYPE& data, DefaultLoadExtension& context, const JsonLoadInfo& info)
    {
        slr::serialize(data, context.globalContext, info);
    }
};  

class JsonSaveLoadContext : public JsonSaveContext, public JsonLoadContext
{
public:
    JsonSaveLoadContext(DefaultSaveExtension& saveSharedContext, DefaultLoadExtension& loadSharedContext)
        : JsonSaveContext(saveSharedContext), JsonLoadContext(loadSharedContext)
    {

    }
};

template<>
class Object<JsonSaveLoadContext> : public Object<JsonSaveContext>, public Object<JsonLoadContext>
{

};


}