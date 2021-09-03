#pragma once

#include <Salieri/salieri.hpp>
#include <Salieri/JsonSaveLoad/fwd.hpp>

namespace slr
{
template<typename DATA_TYPE>
struct JsonFileAccess
{
    void save(DATA_TYPE& data, json::JsonSaveContext& context, const json::JsonSaveInfo& info)
    {
        context.pushCategory(info.getName());
        data.serialize(context);
        context.popCategory();
    }
    void load(DATA_TYPE& data, json::JsonLoadContext& context, const json::JsonLoadInfo& info)
    {
        context.pushCategory(info.getName());
        data.serialize(context);
        context.popCategory();
    }
};

template<typename DATA_TYPE>
struct Serialize<DATA_TYPE, json::JsonSaveContext, json::JsonSaveInfo>
{
    void operator()(DATA_TYPE& data, json::JsonSaveContext& context, const json::JsonSaveInfo& info)
    {
        JsonFileAccess<std::remove_const_t<DATA_TYPE>>{}.save(data, context, info);
    }
};  

template<typename DATA_TYPE>
struct Serialize<DATA_TYPE, json::JsonLoadContext, json::JsonLoadInfo>
{
    void operator()(DATA_TYPE& data, json::JsonLoadContext& context, const json::JsonLoadInfo& info)
    {
        JsonFileAccess<DATA_TYPE>{}.load(data, context, info);
    }
};  

}