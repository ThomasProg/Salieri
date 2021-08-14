#include <Salieri/BinarySaveLoad/context.hpp>
#include <Salieri/exceptions.hpp>
#include <Salieri/BinarySaveLoad/generic.hpp>
#include <Salieri/BinarySaveLoad/primitives.hpp>

#include <cstdio>
#include <cassert>
#include <iostream>

namespace slr
{

BinarySaveContext::BinarySaveContext(const char* filename, DefaultExtension& sharedContext)
    : shared(sharedContext)
{
    file = fopen(filename, "w");
    if (file == nullptr)
    {
        throw slr::Exception("File couldn't be opened for writing");
    }
}

BinarySaveContext::~BinarySaveContext()
{
    fclose(file);
}

BinaryLoadContext::BinaryLoadContext(const char* filename, DefaultExtension& sharedContext)
    : shared(sharedContext)
{
    file = fopen(filename, "r");
    if (file == nullptr)
    {
        throw slr::Exception("File couldn't be opened for reading");
    }
}

BinaryLoadContext::~BinaryLoadContext()
{
    fclose(file);
}

void BinarySaveContext::saveRaw(const void* bytes, size_t nbBytes)
{
    assert(bytes != nullptr); // bytes should be valid

    // If we can't write all of the bytes, then we don't write anything
    fwrite(bytes, nbBytes, 1, file);    
}

void BinaryLoadContext::loadRaw(void* dest, size_t nbBytes)
{
    assert(dest != nullptr); // destination should be valid

    // If we can't write all of the bytes, then we don't write anything
    if (fread(dest, nbBytes, 1, file) == 0)
    {
        throw slr::Exception("Couldn't load bytes from file.");
    }   
}

}