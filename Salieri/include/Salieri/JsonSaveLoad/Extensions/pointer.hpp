#pragma once 

#include <Salieri/JsonSaveLoad/context.hpp>
#include <Salieri/JsonSaveLoad/generic.hpp>

#include <string>
#include <set>
#include <map>

namespace slr
{
namespace json
{

class SaveContextPointerExtension : public slr::JsonSaveContext::DefaultExtension
{
private:
    std::map<std::string /* name */, slr::JsonSaveContext* /* not null */> alreadySavedObjects;

public:

    virtual void saveObject(slr::JsonSaveContext& context, slr::Object<slr::JsonSaveContext>& obj) = 0;
    void saveFileObject(slr::JsonSaveContext& context, slr::Object<slr::JsonSaveContext>& obj, const std::string& name);
};

class LoadContextPointerExtension : public slr::JsonLoadContext::DefaultExtension
{
public:
    using ObjectType = slr::Object<slr::JsonLoadContext>;

private:
    // Key : name of the object
    // Value : pointer to an ObjectPtr
    std::map<std::string, std::shared_ptr<ObjectType>> objectsMap;

public:   
    std::function<void(slr::Object<slr::JsonLoadContext>*)> loadedObjectInstanceDeleter = std::default_delete<slr::Object<slr::JsonLoadContext>>();

    // std::string loadFolder = sSaveFolder;

    /**
     * @brief 
     * 
     * @return ObjectType* Non null object 
     */
    virtual ObjectType* loadObject(slr::JsonLoadContext& context) = 0;
    ObjectType* loadFileObject(const std::string& objectName);
    std::shared_ptr<ObjectType> loadSharedFileObject(slr::JsonLoadContext& context, const std::string& objectName);
    std::weak_ptr<ObjectType> loadWeakFileObject(slr::JsonLoadContext& context, const std::string& objectName);
};

}

template<typename T>                                          
struct JsonFileAccess<std::shared_ptr<T>>                             
{
    static_assert(std::is_base_of<slr::Object<slr::JsonSaveLoadContext>, T>::value);

    void save(std::shared_ptr<T>& data, JsonSaveContext& context, const JsonSaveInfo& info)
    {
        // TODO : multithread
        T* ptr =  data.get();
        bool doesPtrExist = ptr != nullptr;
        slr::serialize(doesPtrExist, context, slr::JsonSaveInfo("isNull"));
        if (doesPtrExist)
        {
            std::string name = ptr->getName();
            slr::serialize(name, context, slr::JsonSaveInfo(info.getName()));

            // Dynamic cast is more secure but less performant than static_cast
            // However, it doesn't have a lot of cost even if called 1 000 000 times.
            json::SaveContextPointerExtension& extension = dynamic_cast<json::SaveContextPointerExtension&>(context.shared); 
            // SaveContextPointerExtension& extension = static_cast<SaveContextPointerExtension&>(context.shared);
            extension.saveFileObject(context, *data.get(), name);
        }
    }
    void load(std::shared_ptr<T>& data, JsonLoadContext& context, const JsonLoadInfo& info)
    {
        // TODO : multithread
        bool doesPtrExist;
        slr::serialize(doesPtrExist, context, slr::JsonLoadInfo("isNull"));

        if (doesPtrExist)
        {
            std::string name;
            slr::serialize(name, context, slr::JsonLoadInfo(info.getName()));
            json::LoadContextPointerExtension& extension = dynamic_cast<json::LoadContextPointerExtension&>(context.shared); 
            data = std::static_pointer_cast<T>(extension.loadSharedFileObject(context, name));
        }
    }
};

template<typename T>                                          
struct JsonFileAccess<std::weak_ptr<T>>                             
{
    static_assert(std::is_base_of<slr::Object<slr::JsonSaveLoadContext>, T>::value);

    void save(std::weak_ptr<T>& data, JsonSaveContext& context, const JsonSaveInfo& info)
    {
        // TODO : multithread
        std::shared_ptr<T> ptr = data.lock();
        bool doesPtrExist = ptr.get() != nullptr;
        slr::serialize(doesPtrExist, context, slr::JsonSaveInfo());
        if (doesPtrExist)
        {
            std::string name = ptr->getName();
            slr::serialize(name, context, slr::JsonSaveInfo());

            // Dynamic cast is more secure but less performant than static_cast
            // However, it doesn't have a lot of cost even if called 1 000 000 times.
            json::SaveContextPointerExtension& extension = dynamic_cast<json::SaveContextPointerExtension&>(context.shared); 
            // SaveContextPointerExtension& extension = static_cast<SaveContextPointerExtension&>(context.shared);
            extension.saveFileObject(context, *ptr, name);
        }
    }
    void load(std::weak_ptr<T>& data, JsonLoadContext& context, const JsonLoadInfo& info)
    {
        // TODO : multithread
        bool doesPtrExist;
        slr::serialize(doesPtrExist, context, slr::JsonLoadInfo());

        if (doesPtrExist)
        {
            std::string name;
            slr::serialize(name, context, slr::JsonLoadInfo());
            json::LoadContextPointerExtension& extension = dynamic_cast<json::LoadContextPointerExtension&>(context.shared); 
            data = std::static_pointer_cast<T>(extension.loadSharedFileObject(context, name));
        }
    }
};

template<typename T>                                          
struct JsonFileAccess<std::unique_ptr<T>>                         
{
    void save(std::unique_ptr<T>& data, JsonSaveContext& context, const JsonSaveInfo& info)
    {
        T* ptr =  data.get();
        bool doesPtrExist = ptr != nullptr;
        slr::serialize(doesPtrExist, context, slr::JsonSaveInfo());
        if (doesPtrExist)
        {
            json::SaveContextPointerExtension& extension = dynamic_cast<json::SaveContextPointerExtension&>(context.shared); 
            extension.saveObject(context, *ptr);
        }
    }
    void load(std::unique_ptr<T>& data, JsonLoadContext& context, const JsonLoadInfo& info)
    {
        bool doesPtrExist;
        slr::serialize(doesPtrExist, context, slr::JsonLoadInfo());

        if (doesPtrExist)
        {
            json::LoadContextPointerExtension& extension = dynamic_cast<json::LoadContextPointerExtension&>(context.shared); 
            data = std::unique_ptr<T>(static_cast<T*>(extension.loadObject(context)));
        }
    }
};

}