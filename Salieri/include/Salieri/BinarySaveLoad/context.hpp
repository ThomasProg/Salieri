#pragma once

#include <Salieri/salieri.hpp>
#include <Salieri/object.hpp>
#include <Salieri/BinarySaveLoad/fwd.hpp>

#include <iosfwd>

#include <future>
#include <map>
#include <set>
#include <memory>
#include <shared_mutex>

namespace slr
{
class BinarySaveInfo
{

};

// Must be thread safe
class BinarySaveContext 
{
    // Should be considered non null after constructor call
    FILE* file = nullptr;

public:
    class DefaultExtension
    {  
    public:

        // slr::BinarySaveContext globalContext = slr::BinarySaveContext("__global", *this);
        virtual ~DefaultExtension() = default;

    }& shared;

    void saveRaw(const void* bytes, size_t nbBytes);

    BinarySaveContext(const char* filename, DefaultExtension& sharedContext);
    ~BinarySaveContext();
};

class BinaryLoadInfo
{

};

class BinaryLoadContext
{
    // Should be considered non null after constructor call
    FILE* file = nullptr;

public:
    class DefaultExtension
    {
    
    public:

        virtual ~DefaultExtension() = default;

    }& shared;

public:
    void loadRaw(void* bytes, size_t nbBytes);

    BinaryLoadContext(const char* filename, DefaultExtension& sharedContext);
    ~BinaryLoadContext();
};

class BinarySaveLoadContext : public BinarySaveContext, public BinaryLoadContext
{

};

template<>
class Object<BinarySaveLoadContext> : public Object<BinarySaveContext>, public Object<BinaryLoadContext>
{

};

}
