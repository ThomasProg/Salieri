#include <Salieri/FileContext/saveLoadFile.hpp>
#include <Salieri/exceptions.hpp>
#include <Salieri/FileContext/generic.hpp>
#include <Salieri/FileContext/primitives.hpp>

#include <cstdio>
#include <cassert>
#include <iostream>

namespace slr
{

BinarySaveContext::BinarySaveContext(const char* filename, SharedContext& sharedContext)
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

BinaryLoadContext::BinaryLoadContext(const char* filename, SharedContext& sharedContext)
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

void BinarySaveContext::savePolymorphicObject(slr::Object<BinarySaveContext>& obj)
{
    shared.saveObjectTypeInfo(obj, *this);
    slr::serialize(obj, *this, slr::BinaryLoadInfo());
}

BinaryLoadContext::ObjectType* BinaryLoadContext::loadPolymorphicObject()
{
    BinaryLoadContext::ObjectType* ptr = shared.loadObjectInstance(*this);
    slr::serialize(*ptr, *this, slr::BinaryLoadInfo());
    return ptr;
}

void BinarySaveContext::saveFileObject(slr::Object<BinarySaveContext>& obj, const std::string& name)
{
    auto v = shared.alreadySavedObjects.find(name);
    if (v == shared.alreadySavedObjects.end())
    {
        shared.alreadySavedObjects.emplace(name); // TODO : can be optimized
        BinarySaveContext newContext = BinarySaveContext(name.c_str(), shared);
        if (!shared.saveObjectTypeInfo)
        {
            throw slr::Exception("saveObjectTypeInfo must be valid");
        }
        newContext.savePolymorphicObject(obj);
    }
}

std::shared_ptr<BinaryLoadContext::ObjectType> BinaryLoadContext::loadFileObject(const std::string& objectName)
{
    auto it = shared.objectsMap.find(objectName);
    if (it == shared.objectsMap.end())
    {
        it = shared.objectsMap.emplace_hint(it, objectName, std::shared_ptr<ObjectType>());

        BinaryLoadContext newContext = BinaryLoadContext(objectName.c_str(), shared);
        if (!shared.loadObjectInstance)
        {
            throw slr::Exception("loadObjectInstance must be valid");
        }
        it->second = std::shared_ptr<BinaryLoadContext::ObjectType>(newContext.loadPolymorphicObject(), shared.loadedObjectInstanceDeleter);
    }
    return it->second;
}

}