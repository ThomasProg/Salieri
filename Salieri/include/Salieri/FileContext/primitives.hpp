#pragma once

#include <Salieri/salieri.hpp>
#include <Salieri/FileContext/saveLoadFile.hpp>
#include <Salieri/FileContext/binaryFileAccess.hpp>

namespace slr
{

#define SAVE_LOAD_PRIMITIVE_AS_RAW(DATA_TYPE)    \
template<>                                          \
struct BinaryFileAccess<DATA_TYPE>                             \
{\
    void save(DATA_TYPE& data, BinarySaveContext& context, const BinarySaveInfo& info)\
    {\
        context.saveRaw((char*)&data, sizeof(data));\
    }\
    void load(DATA_TYPE& data, BinaryLoadContext& context, const BinaryLoadInfo& info)\
    {\
        context.loadRaw((char*)&data, sizeof(data));\
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
SAVE_LOAD_PRIMITIVE_AS_RAW(const char*)

#undef SAVE_LOAD_PRIMITIVE_AS_RAW



}