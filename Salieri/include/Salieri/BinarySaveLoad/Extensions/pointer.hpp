#pragma once 

#include <Salieri/BinarySaveLoad/context.hpp>
#include <Salieri/BinarySaveLoad/generic.hpp>

#include <string>
#include <set>
#include <map>

static std::string sSaveFolder = "Saved/";

namespace slr
{

class SaveContextPointerExtension : public slr::BinarySaveContext::DefaultExtension
{
private:
    std::set<std::string> alreadySavedObjects;
    std::string saveFolder = sSaveFolder;

public:

    virtual void saveObject(slr::BinarySaveContext& context, slr::Object<slr::BinarySaveContext>& obj) = 0;
    void saveFileObject(slr::BinarySaveContext& context, slr::Object<slr::BinarySaveContext>& obj, const std::string& name);
};

class LoadContextPointerExtension : public slr::BinaryLoadContext::DefaultExtension
{
public:
    using ObjectType = slr::Object<slr::BinaryLoadContext>;

private:
    // Key : name of the object
    // Value : pointer to an ObjectPtr
    std::map<std::string, std::shared_ptr<ObjectType>> objectsMap;

public:   
    std::function<void(slr::Object<slr::BinaryLoadContext>*)> loadedObjectInstanceDeleter = std::default_delete<slr::Object<slr::BinaryLoadContext>>();

    std::string loadFolder = sSaveFolder;

    /**
     * @brief 
     * 
     * @return ObjectType* Non null object 
     */
    virtual ObjectType* loadObject(slr::BinaryLoadContext& context) = 0;
    ObjectType* loadFileObject(const std::string& objectName);
    std::shared_ptr<ObjectType> loadSharedFileObject(slr::BinaryLoadContext& context, const std::string& objectName);
    std::weak_ptr<ObjectType> loadWeakFileObject(slr::BinaryLoadContext& context, const std::string& objectName);
};

template<typename T>                                          
struct BinaryFileAccess<std::shared_ptr<T>>                             
{
    static_assert(std::is_base_of<slr::Object<slr::BinarySaveLoadContext>, T>::value);

    void save(std::shared_ptr<T>& data, BinarySaveContext& context, const BinarySaveInfo& info)
    {
        // TODO : multithread
        T* ptr =  data.get();
        bool doesPtrExist = ptr != nullptr;
        slr::serialize(doesPtrExist, context, slr::BinarySaveInfo());
        if (doesPtrExist)
        {
            std::string name = ptr->getName();
            slr::serialize(name, context, slr::BinarySaveInfo());

            // Dynamic cast is more secure but less performant than static_cast
            // However, it doesn't have a lot of cost even if called 1 000 000 times.
            SaveContextPointerExtension& extension = dynamic_cast<SaveContextPointerExtension&>(context.shared); 
            // SaveContextPointerExtension& extension = static_cast<SaveContextPointerExtension&>(context.shared);
            extension.saveFileObject(context, *data.get(), name);
        }
    }
    void load(std::shared_ptr<T>& data, BinaryLoadContext& context, const BinaryLoadInfo& info)
    {
        // TODO : multithread
        bool doesPtrExist;
        slr::serialize(doesPtrExist, context, slr::BinaryLoadInfo());

        if (doesPtrExist)
        {
            std::string name;
            slr::serialize(name, context, slr::BinaryLoadInfo());
            LoadContextPointerExtension& extension = dynamic_cast<LoadContextPointerExtension&>(context.shared); 
            data = std::static_pointer_cast<T>(extension.loadSharedFileObject(context, name));
        }
    }
};

template<typename T>                                          
struct BinaryFileAccess<std::weak_ptr<T>>                             
{
    static_assert(std::is_base_of<slr::Object<slr::BinarySaveLoadContext>, T>::value);

    void save(std::weak_ptr<T>& data, BinarySaveContext& context, const BinarySaveInfo& info)
    {
        // TODO : multithread
        std::shared_ptr<T> ptr = data.lock();
        bool doesPtrExist = ptr.get() != nullptr;
        slr::serialize(doesPtrExist, context, slr::BinarySaveInfo());
        if (doesPtrExist)
        {
            std::string name = ptr->getName();
            slr::serialize(name, context, slr::BinarySaveInfo());

            // Dynamic cast is more secure but less performant than static_cast
            // However, it doesn't have a lot of cost even if called 1 000 000 times.
            SaveContextPointerExtension& extension = dynamic_cast<SaveContextPointerExtension&>(context.shared); 
            // SaveContextPointerExtension& extension = static_cast<SaveContextPointerExtension&>(context.shared);
            extension.saveFileObject(context, *ptr, name);
        }
    }
    void load(std::weak_ptr<T>& data, BinaryLoadContext& context, const BinaryLoadInfo& info)
    {
        // TODO : multithread
        bool doesPtrExist;
        slr::serialize(doesPtrExist, context, slr::BinaryLoadInfo());

        if (doesPtrExist)
        {
            std::string name;
            slr::serialize(name, context, slr::BinaryLoadInfo());
            LoadContextPointerExtension& extension = dynamic_cast<LoadContextPointerExtension&>(context.shared); 
            data = std::static_pointer_cast<T>(extension.loadSharedFileObject(context, name));
        }
    }
};

template<typename T>                                          
struct BinaryFileAccess<std::unique_ptr<T>>                         
{
    void save(std::unique_ptr<T>& data, BinarySaveContext& context, const BinarySaveInfo& info)
    {
        T* ptr =  data.get();
        bool doesPtrExist = ptr != nullptr;
        slr::serialize(doesPtrExist, context, slr::BinarySaveInfo());
        if (doesPtrExist)
        {
            SaveContextPointerExtension& extension = dynamic_cast<SaveContextPointerExtension&>(context.shared); 
            extension.saveObject(context, *ptr);
        }
    }
    void load(std::unique_ptr<T>& data, BinaryLoadContext& context, const BinaryLoadInfo& info)
    {
        bool doesPtrExist;
        slr::serialize(doesPtrExist, context, slr::BinaryLoadInfo());

        if (doesPtrExist)
        {
            LoadContextPointerExtension& extension = dynamic_cast<LoadContextPointerExtension&>(context.shared); 
            data = std::unique_ptr<T>(static_cast<T*>(extension.loadObject(context)));
        }
    }
};

}