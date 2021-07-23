#pragma once

#include <Salieri/salieri.hpp>
#include <iosfwd>

namespace slr
{
class BinarySaveContext
{
    FILE* file = nullptr;

public:
    void saveRaw(const void* bytes, size_t nbBytes);

    BinarySaveContext(const char* filename);
    ~BinarySaveContext();
};

class BinarySaveInfo
{

};

class BinaryLoadContext
{
    FILE* file = nullptr;

public:
    void loadRaw(void* bytes, size_t nbBytes);

    BinaryLoadContext(const char* filename);
    ~BinaryLoadContext();
};

class BinaryLoadInfo
{

};

}

