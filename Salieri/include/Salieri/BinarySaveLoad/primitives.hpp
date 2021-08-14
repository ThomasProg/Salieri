#pragma once

#include <Salieri/salieri.hpp>
#include <Salieri/BinarySaveLoad/context.hpp>
#include <Salieri/BinarySaveLoad/generic.hpp>

namespace slr
{

#define SAVE_LOAD_PRIMITIVE_AS_RAW(DATA_TYPE)    \
template<>                                          \
struct BinaryFileAccess<DATA_TYPE>                             \
{\
    void save(const DATA_TYPE& data, BinarySaveContext& context, const BinarySaveInfo& info)\
    {\
        context.saveRaw((void*)&data, sizeof(data));\
    }\
    void load(DATA_TYPE& data, BinaryLoadContext& context, const BinaryLoadInfo& info)\
    {\
        context.loadRaw((void*)&data, sizeof(data));\
    }\
};

SAVE_LOAD_PRIMITIVE_AS_RAW(bool)
SAVE_LOAD_PRIMITIVE_AS_RAW(char)
SAVE_LOAD_PRIMITIVE_AS_RAW(signed char)
SAVE_LOAD_PRIMITIVE_AS_RAW(unsigned char)
SAVE_LOAD_PRIMITIVE_AS_RAW(int)
SAVE_LOAD_PRIMITIVE_AS_RAW(unsigned int)
SAVE_LOAD_PRIMITIVE_AS_RAW(size_t)
SAVE_LOAD_PRIMITIVE_AS_RAW(float)
SAVE_LOAD_PRIMITIVE_AS_RAW(double)
// SAVE_LOAD_PRIMITIVE_AS_RAW(const char*)

#undef SAVE_LOAD_PRIMITIVE_AS_RAW


template<>                                          
struct BinaryFileAccess<std::string>                             
{
    void save(std::string& data, BinarySaveContext& context, const BinarySaveInfo& info)
    {
        size_t length = data.length();
        slr::serialize(length, context, slr::BinarySaveInfo());
        context.saveRaw(data.c_str(), sizeof(char) * length);
    }
    void load(std::string& data, BinaryLoadContext& context, const BinaryLoadInfo& info)
    {
        size_t length;
        slr::serialize(length, context, slr::BinaryLoadInfo());
        data.resize(length);
        context.loadRaw(&data[0], sizeof(char) * length);
    }
};





}