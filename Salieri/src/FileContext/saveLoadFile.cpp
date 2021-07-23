#include <Salieri/FileContext/saveLoadFile.hpp>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <Salieri/exceptions.hpp>

namespace slr
{

BinarySaveContext::BinarySaveContext(const char* filename)
{
    file = fopen(filename, "w");
}

BinarySaveContext::~BinarySaveContext()
{
    fclose(file);
}

void BinarySaveContext::saveRaw(const void* bytes, size_t nbBytes)
{
    assert(file != nullptr);  // a file should be opened
    assert(bytes != nullptr); // bytes should be valid

    // If we can't write all of the bytes, then we don't write anything
    fwrite(bytes, nbBytes, 1, file);    
}

BinaryLoadContext::BinaryLoadContext(const char* filename)
{
    file = fopen(filename, "r");
}

BinaryLoadContext::~BinaryLoadContext()
{
    fclose(file);
}

void BinaryLoadContext::loadRaw(void* dest, size_t nbBytes)
{
    assert(file != nullptr);  // a file should be opened
    assert(dest != nullptr); // destination should be valid

    // If we can't write all of the bytes, then we don't write anything
    if (fread(dest, nbBytes, 1, file) == 0)
    {
        throw slr::Exception("Couldn't write bytes to file : no space left on device ?");
    }   
}

}