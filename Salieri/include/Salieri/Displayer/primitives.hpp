#pragma once

#include <salieri/salieri.hpp>
#include <salieri/Displayer/displayer.hpp>

namespace slr
{

#define MAKE_PRIMITIVE_SERIALIZED(type)                                             \
template<typename DATA_INFO>                                                        \
struct Serialize<type, DisplayContext, DATA_INFO>                                   \
{                                                                                   \
    void operator()(type& data, DisplayContext& context, const DATA_INFO& info)     \
    {                                                                               \
        std::cout << data << " (" << #type << ")" << std::endl;                     \
    }                                                                               \
};

MAKE_PRIMITIVE_SERIALIZED(bool)
MAKE_PRIMITIVE_SERIALIZED(char)
MAKE_PRIMITIVE_SERIALIZED(signed char)
MAKE_PRIMITIVE_SERIALIZED(unsigned char)
MAKE_PRIMITIVE_SERIALIZED(int)
MAKE_PRIMITIVE_SERIALIZED(unsigned int)
MAKE_PRIMITIVE_SERIALIZED(size_t)
MAKE_PRIMITIVE_SERIALIZED(float)
MAKE_PRIMITIVE_SERIALIZED(double)
MAKE_PRIMITIVE_SERIALIZED(const char*)
MAKE_PRIMITIVE_SERIALIZED(std::string)

#undef MAKE_PRIMITIVE_SERIALIZED

// Specialization for array of char
template<size_t SIZE, typename DATA_INFO>                                                        
struct Serialize<const char[SIZE], DisplayContext, DATA_INFO>                                   
{                                                                                   
    void operator()(const char (&data)[SIZE], DisplayContext& context, const DATA_INFO& info)     
    {                                                                               
        std::cout << data << " (const char[" << SIZE <<"])" << std::endl;                     
    }                                                                               
};


}