#pragma once

#include <Salieri/salieri.hpp>
#include <Salieri/JsonSaveLoad/context.hpp>
#include <Salieri/JsonSaveLoad/generic.hpp>

namespace slr
{

#define SAVE_LOAD_PRIMITIVE(DATA_TYPE)    \
template<>                                          \
struct JsonFileAccess<DATA_TYPE>                             \
{\
    void save(const DATA_TYPE& data, json::JsonSaveContext& context, const json::JsonSaveInfo& info)\
    {\
        context.save(info.getName(), data);\
    }\
    void load(DATA_TYPE& data, json::JsonLoadContext& context, const json::JsonLoadInfo& info)\
    {\
        context.load(info.getName(), data);\
    }\
};

SAVE_LOAD_PRIMITIVE(bool)
SAVE_LOAD_PRIMITIVE(char)
SAVE_LOAD_PRIMITIVE(signed char)
SAVE_LOAD_PRIMITIVE(unsigned char)
SAVE_LOAD_PRIMITIVE(int)
SAVE_LOAD_PRIMITIVE(unsigned int)
SAVE_LOAD_PRIMITIVE(size_t)
SAVE_LOAD_PRIMITIVE(float)
SAVE_LOAD_PRIMITIVE(double)
// SAVE_LOAD_PRIMITIVE(const char*)
SAVE_LOAD_PRIMITIVE(std::string)
SAVE_LOAD_PRIMITIVE(std::nullptr_t)

#undef SAVE_LOAD_PRIMITIVE

}