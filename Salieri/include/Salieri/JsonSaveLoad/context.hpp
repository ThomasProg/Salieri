#pragma once

#include <Salieri/salieri.hpp>
#include <Salieri/object.hpp>
#include <Salieri/JsonSaveLoad/fwd.hpp>
#include <Salieri/JsonSaveLoad/json.hpp>
#include <Salieri/exceptions.hpp>

#include <stack>

namespace slr
{
namespace json
{
class BaseJsonContext
{
public:
    nlohmann::json mainJson;
    std::stack<nlohmann::json*> jsons; // does not contain null values

public:
    BaseJsonContext()
    {
        jsons.emplace(&mainJson);
    }

    nlohmann::json& getLastJsonObject()
    {
        return *jsons.top();
    }

    std::string getJson() const
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

    virtual void toDisk(const std::string& directory = "SavedJSON/", const std::string& globalContextName = "__global");

    virtual ~DefaultSaveExtension() = default;
};

}

template<typename DATA_TYPE>
struct Serialize<DATA_TYPE, json::DefaultSaveExtension, json::JsonSaveInfo>
{
    void operator()(DATA_TYPE& data, json::DefaultSaveExtension& context, const json::JsonSaveInfo& info)
    {
        slr::serialize(data, context.globalContext, info);
    }
};  

namespace json
{
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
        if (isNull(name))
        {
            // throw slr::Exception(("Could not load value : " + name).c_str());    
            return;
        }
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

    virtual void fromDisk(const std::string& directory = "SavedJSON/", const std::string& globalContextName = "__global");
};
}

template<typename DATA_TYPE>
struct Serialize<DATA_TYPE, json::DefaultLoadExtension, json::JsonLoadInfo>
{
    void operator()(DATA_TYPE& data, json::DefaultLoadExtension& context, const json::JsonLoadInfo& info)
    {
        slr::serialize(data, context.globalContext, info);
    }
};  

namespace json
{
class JsonSaveLoadContext : public json::JsonSaveContext, public json::JsonLoadContext
{
public:
    JsonSaveLoadContext(DefaultSaveExtension& saveSharedContext, DefaultLoadExtension& loadSharedContext)
        : JsonSaveContext(saveSharedContext), JsonLoadContext(loadSharedContext)
    {

    }
};

class Doc
{
    nlohmann::json mainJson;

public:
    Doc& operator=(DefaultSaveExtension&& rhs)
    {
        mainJson = std::move(rhs.globalContext.mainJson);
        return *this;
    }

    Doc& operator=(DefaultLoadExtension&& rhs)
    {
        mainJson = std::move(rhs.globalContext.mainJson);
        return *this;
    }

    template<class EXTENSION, typename T>
    void save(const T& data, const JsonSaveInfo& info)
    {
        EXTENSION extension;
        JsonSaveContext& context = extension.globalContext;
        context.mainJson = std::move(mainJson);
        slr::serialize(data, context, info);
        *this = std::move(extension);
    }

    template<class EXTENSION, typename T>
    void load(T& data, const JsonLoadInfo& info)
    {
        EXTENSION extension;
        JsonLoadContext& context = extension.globalContext;
        context.mainJson = std::move(mainJson);
        slr::serialize(data, context, info);
        *this = std::move(extension);
    }

    template<typename T>
    void save(const T& data, const JsonSaveInfo& info)
    {
        save<DefaultSaveExtension, T>(data, info);
    }

    template<typename T>
    void load(T& data, const JsonLoadInfo& info)
    {
        load<DefaultLoadExtension, T>(data, info);
    }
};
}

template<>
class Object<json::JsonSaveLoadContext> : public Object<json::JsonSaveContext>, public Object<json::JsonLoadContext>
{

};

}