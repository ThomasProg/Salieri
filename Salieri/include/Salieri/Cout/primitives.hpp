#pragma once

#include <Salieri/salieri.hpp>
#include <Salieri/Cout/coutContext.hpp>

namespace slr
{

#define MAKE_PRIMITIVE_SERIALIZED(type)                                             \
template<typename DATA_INFO>                                                        \
struct Serialize<type, CoutContext, DATA_INFO>                                   \
{                                                                                   \
    void operator()(type& data, CoutContext& context, const DATA_INFO& info)     \
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
template<std::size_t SIZE, typename DATA_INFO>                                                        
struct Serialize<const char[SIZE], CoutContext, DATA_INFO>                                   
{                                                                                   
    void operator()(const char (&data)[SIZE], CoutContext& context, const DATA_INFO& info)     
    {                                                                               
        std::cout << data << " (const char[" << SIZE <<"])" << std::endl;                     
    }                                                                               
};


}