#pragma once

#include <Salieri/salieri.hpp>
#include <Salieri/object.hpp>
#include <iosfwd>

#include <future>
#include <map>
#include <set>
#include <memory>
#include <shared_mutex>

namespace slr
{
class BinarySaveContext;
class BinaryLoadContext;

class BinarySaveLoadContext
{

};

template<>
class Object<BinarySaveContext> 
{
public:
    virtual void serialize(BinarySaveContext&) = 0;
    virtual std::size_t getTypeID() = 0;
    virtual ~Object() = default;
};

template<>
class Object<BinarySaveLoadContext> : public Object<BinarySaveContext>, public Object<BinaryLoadContext>
{

};

class BinarySaveInfo
{

};

// Must be thread safe
class BinarySaveContext 
{
    using ObjectType = slr::Object<BinarySaveContext>;

    // Should be considered non null after constructor call
    FILE* file = nullptr;

public:
    class SharedContext
    {
        using ObjectType = BinarySaveContext::ObjectType;
    
    public:
        std::set<std::string> alreadySavedObjects;
        std::function<void(slr::Object<BinarySaveContext>&, BinarySaveContext&)> saveObjectTypeInfo;

    }& shared;

    void saveRaw(const void* bytes, size_t nbBytes);
    void savePolymorphicObject(slr::Object<BinarySaveContext>& obj);
    void saveFileObject(slr::Object<BinarySaveContext>& obj, const std::string& name);

    BinarySaveContext(const char* filename, SharedContext& sharedContext);
    ~BinarySaveContext();
};

class BinaryLoadInfo
{

};

class BinaryLoadContext
{
    // Should be considered non null after constructor call
    FILE* file = nullptr;

    using ObjectType = slr::Object<BinaryLoadContext>;

public:
    class SharedContext
    {
        using ObjectType = BinaryLoadContext::ObjectType;
    
    public:

        // Key : name of the object
        // Value : pointer to an ObjectPtr
        // std::map<std::string, ControlBlock*> objectsMap;
        // std::map<std::string, std::future<slr::Object*>> objectsMapFuture;

        // Key : name of the object
        // Value : pointer to an ObjectPtr
        std::map<std::string, std::shared_ptr<ObjectType>> objectsMap;
        
        std::function<slr::Object<BinaryLoadContext>*(BinaryLoadContext&)> loadObjectInstance;
        std::function<void(slr::Object<BinaryLoadContext>*)> loadedObjectInstanceDeleter;

        // std::pair<std::shared_ptr<ObjectType>, bool> loadObjectFromExistingObjects(const std::string& name)
        // {
        //     auto it = objectsMap.find(name);
        //     if (it != objectsMap.end())
        //     {            
        //         return std::pair<std::shared_ptr<ObjectType>, bool>(it->second, true);
        //     }
        //     else 
        //     {
        //         auto newIt = objectsMap.emplace_hint(it, name, std::shared_ptr<ObjectType>());
        //         return std::pair<std::shared_ptr<ObjectType>, bool>(newIt->second, false);
        //     }
        // }
    }& shared;

public:
    void loadRaw(void* bytes, size_t nbBytes);
    /**
     * @brief 
     * 
     * @return ObjectType* Non null object 
     */
    ObjectType* loadPolymorphicObject();
    std::shared_ptr<ObjectType> loadFileObject(const std::string& objectName);

    // void loadObjectAsync(const std::string& objectName)
    // {
    //     // shared.objectsMapFuture.emplace(objectName, std::async(&BinaryLoadContext::loadObject, this, objectName));
    // }

    BinaryLoadContext(const char* filename, SharedContext& sharedContext);
    ~BinaryLoadContext();
};

}
