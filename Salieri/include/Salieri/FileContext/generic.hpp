#pragma once

#include "binaryFileAccess.hpp"

namespace slr
{

// template<typename DATA_TYPE>
// struct ToStr;
// {
//     std::string operator()(const DATA_TYPE& dataType) const
//     {
//         return dataType;
//     }
// };

template<typename DATA_TYPE>
struct Serialize<DATA_TYPE, BinarySaveContext, BinarySaveInfo>
{
    void operator()(DATA_TYPE& data, BinarySaveContext& context, const BinarySaveInfo& info)
    {
        BinaryFileAccess<DATA_TYPE>{}.save(data, context, info);
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
