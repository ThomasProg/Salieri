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
namespace ptr
{

class SaveContextPointerExtension : public slr::json::DefaultSaveExtension
{
public:
    std::map<std::string /* name */, slr::json::JsonSaveContext* /* not null */> alreadySavedObjects;
    // void (*objectSaver)(slr::json::JsonSaveContext& context, slr::Object<slr::json::JsonSaveContext>& obj) = nullptr;

public:
    virtual void toDisk(const std::string& directory = "SavedJSON/", const std::string& globalContextName = "__global") override;
    virtual void saveObject(json::JsonSaveContext& context, slr::Object<json::JsonSaveContext>& obj) = 0;
    void saveFileObject(json::JsonSaveContext& context, slr::Object<json::JsonSaveContext>& obj, const std::string& name);

    ~SaveContextPointerExtension()
    {
        for (auto&& [key,value] : alreadySavedObjects)
        {
            delete value;
        }
    }
};

class LoadContextPointerExtension : public slr::json::DefaultLoadExtension
{
public:
    using ObjectType = slr::Object<json::JsonLoadContext>;

public:
    // Key : name of the object
    // Value : pointer to an ObjectPtr
    std::map<std::string, std::shared_ptr<ObjectType>> objectsMap;
    // slr::Object<slr::json::JsonLoadContext>* (*objectLoader)(slr::json::JsonLoadContext& context) = nullptr;

public:   
    std::map<std::string /* name */, json::JsonLoadContext* /* not null */> alreadySavedObjects;

    std::function<void(slr::Object<json::JsonLoadContext>*)> loadedObjectInstanceDeleter = std::default_delete<slr::Object<json::JsonLoadContext>>();

    // std::string loadFolder = sSaveFolder;

    /**
     * @brief 
     * 
     * @return ObjectType* Non null object 
     */
    virtual ObjectType* loadObject(json::JsonLoadContext& context) = 0; // Loads an object from the current context
    virtual ObjectType* loadFileObject(const std::string& objectName); // Loads an object from another context
    std::shared_ptr<ObjectType> loadSharedFileObject(json::JsonLoadContext& context, const std::string& objectName);
    std::weak_ptr<ObjectType> loadWeakFileObject(json::JsonLoadContext& context, const std::string& objectName);

    ~LoadContextPointerExtension()
    {
        for (auto&& [key,value] : alreadySavedObjects)
        {
            delete value;
        }
    }
};

class Doc : public json::Doc
{
    std::map<std::string /* name */, nlohmann::json> alreadySavedObjects;

    using Super = json::Doc;

public:
    void (*objectSaver)(slr::json::JsonSaveContext& context, slr::Object<slr::json::JsonSaveContext>& obj) = nullptr;
    slr::Object<slr::json::JsonLoadContext>* (*objectLoader)(slr::json::JsonLoadContext& context) = nullptr;

    Doc& operator=(SaveContextPointerExtension&& rhs)
    {
        json::Doc::operator=(std::move(rhs));
        alreadySavedObjects.clear();
        for (auto&& a : rhs.alreadySavedObjects)
        {
            alreadySavedObjects.emplace(a.first, std::move(a.second->mainJson));
        }
        return *this;
    }

    Doc& operator=(LoadContextPointerExtension&& rhs)
    {
        json::Doc::operator=(std::move(rhs));
        alreadySavedObjects.clear();
        for (auto&& a : rhs.alreadySavedObjects)
        {
            alreadySavedObjects.emplace(a.first, std::move(a.second->mainJson));
        }
        return *this;
    }

    template<typename T>
    void save(const T& data, const JsonSaveInfo& info)
    {
        Super::save<SaveContextPointerExtension, T>(data, info);
    }

    template<typename T>
    void load(T& data, const JsonLoadInfo& info)
    {
        Super::load<LoadContextPointerExtension, T>(data, info);
    }
};

}
}

template<typename T>                                          
struct JsonFileAccess<std::shared_ptr<T>>                             
{
    static_assert(std::is_base_of<slr::Object<json::JsonSaveLoadContext>, T>::value);

    void save(std::shared_ptr<T>& data, json::JsonSaveContext& context, const json::JsonSaveInfo& info)
    {
        // TODO : multithread
        T* ptr =  data.get();
        bool doesPtrExist = ptr != nullptr;
        if (doesPtrExist)
        {
            std::string name = ptr->getName();
            slr::serialize(name, context, json::JsonSaveInfo(info.getName()));

            // Dynamic cast is more secure but less performant than static_cast
            // However, it doesn't have a lot of cost even if called 1 000 000 times.
            json::ptr::SaveContextPointerExtension& extension = dynamic_cast<json::ptr::SaveContextPointerExtension&>(context.shared); 
            // SaveContextPointerExtension& extension = static_cast<SaveContextPointerExtension&>(context.shared);
            extension.saveFileObject(context, *data.get(), name);
        }
        else 
        {
            slr::serialize(nullptr, context, json::JsonSaveInfo(info.getName()));
        }
    }
    void load(std::shared_ptr<T>& data, json::JsonLoadContext& context, const json::JsonLoadInfo& info)
    {
        // TODO : multithread
        // bool doesPtrExist;
        // std::string s;
        // slr::serialize(s, context, slr::JsonLoadInfo(info.getName()));

        if (!context.isNull(info.getName()))
        {
            std::string name;
            slr::serialize(name, context, json::JsonLoadInfo(info.getName()));
            json::ptr::LoadContextPointerExtension& extension = dynamic_cast<json::ptr::LoadContextPointerExtension&>(context.shared); 
            data = std::static_pointer_cast<T>(extension.loadSharedFileObject(context, name));
        }
    }
};

template<typename T>                                          
struct JsonFileAccess<std::weak_ptr<T>>                             
{
    static_assert(std::is_base_of<slr::Object<json::JsonSaveLoadContext>, T>::value);

    void save(std::weak_ptr<T>& data, json::JsonSaveContext& context, const json::JsonSaveInfo& info)
    {
        // TODO : multithread
        std::shared_ptr<T> ptr = data.lock();
        bool doesPtrExist = ptr.get() != nullptr;
        slr::serialize(doesPtrExist, context, json::JsonSaveInfo());
        if (doesPtrExist)
        {
            std::string name = ptr->getName();
            slr::serialize(name, context, json::JsonSaveInfo());

            // Dynamic cast is more secure but less performant than static_cast
            // However, it doesn't have a lot of cost even if called 1 000 000 times.
            json::ptr::SaveContextPointerExtension& extension = dynamic_cast<json::ptr::SaveContextPointerExtension&>(context.shared); 
            // SaveContextPointerExtension& extension = static_cast<SaveContextPointerExtension&>(context.shared);
            extension.saveFileObject(context, *ptr, name);
        }
    }
    void load(std::weak_ptr<T>& data, json::JsonLoadContext& context, const json::JsonLoadInfo& info)
    {
        // TODO : multithread
        bool doesPtrExist;
        slr::serialize(doesPtrExist, context, json::JsonLoadInfo());

        if (doesPtrExist)
        {
            std::string name;
            slr::serialize(name, context, json::JsonLoadInfo());
            json::ptr::LoadContextPointerExtension& extension = dynamic_cast<json::ptr::LoadContextPointerExtension&>(context.shared); 
            data = std::static_pointer_cast<T>(extension.loadSharedFileObject(context, name));
        }
    }
};

template<typename T>                                          
struct JsonFileAccess<std::unique_ptr<T>>                         
{
    void save(std::unique_ptr<T>& data, json::JsonSaveContext& context, const json::JsonSaveInfo& info)
    {
        T* ptr =  data.get();
        bool doesPtrExist = ptr != nullptr;
        slr::serialize(doesPtrExist, context, json::JsonSaveInfo());
        if (doesPtrExist)
        {
            json::ptr::SaveContextPointerExtension& extension = dynamic_cast<json::ptr::SaveContextPointerExtension&>(context.shared); 
            extension.saveObject(context, *ptr);
        }
    }
    void load(std::unique_ptr<T>& data, json::JsonLoadContext& context, const json::JsonLoadInfo& info)
    {
        bool doesPtrExist;
        slr::serialize(doesPtrExist, context, json::JsonLoadInfo());

        if (doesPtrExist)
        {
            json::ptr::LoadContextPointerExtension& extension = dynamic_cast<json::ptr::LoadContextPointerExtension&>(context.shared); 
            data = std::unique_ptr<T>(static_cast<T*>(extension.loadObject(context)));
        }
    }
};

}