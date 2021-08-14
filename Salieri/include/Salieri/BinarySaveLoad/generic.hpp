#pragma once

#include <Salieri/salieri.hpp>
#include <Salieri/BinarySaveLoad/fwd.hpp>

namespace slr
{
template<typename DATA_TYPE>
struct BinaryFileAccess
{
    void save(DATA_TYPE& data, BinarySaveContext& context, const BinarySaveInfo& info)
    {
        data.serialize(context);
    }
    void load(DATA_TYPE& data, BinaryLoadContext& context, const BinaryLoadInfo& info)
    {
        data.serialize(context);
    }
};

template<typename DATA_TYPE>
struct Serialize<DATA_TYPE, BinarySaveContext, BinarySaveInfo>
{
    void operator()(DATA_TYPE& data, BinarySaveContext& context, const BinarySaveInfo& info)
    {
        BinaryFileAccess<std::remove_const_t<DATA_TYPE>>{}.save(data, context, info);
    }
};  

template<typename DATA_TYPE>
struct Serialize<DATA_TYPE, BinaryLoadContext, BinaryLoadInfo>
{
    void operator()(DATA_TYPE& data, BinaryLoadContext& context, const BinaryLoadInfo& info)
    {
        BinaryFileAccess<DATA_TYPE>{}.load(data, context, info);
    }
};  

}
