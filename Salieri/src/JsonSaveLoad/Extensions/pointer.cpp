#include <Salieri/JsonSaveLoad/Extensions/pointer.hpp>
#include <Salieri/JsonSaveLoad/primitives.hpp>
#include <Salieri/file.hpp>

namespace slr
{
namespace json
{
namespace ptr
{

void SaveContextPointerExtension::saveFileObject(slr::json::JsonSaveContext& context, slr::Object<slr::json::JsonSaveContext>& obj, const std::string& name)
{
    auto v = alreadySavedObjects.find(name);
    if (v == alreadySavedObjects.end())
    {
        slr::json::JsonSaveContext* newContext = new slr::json::JsonSaveContext(*this);
        alreadySavedObjects.emplace(name, newContext); // TODO : can be optimized
        saveObject(*newContext, obj);
    }
}

void SaveContextPointerExtension::toDisk(const std::string& directory, const std::string& globalContextName) 
{
    DefaultSaveExtension::toDisk(directory, globalContextName);

    for (const auto& [key, v] : alreadySavedObjects)
    {
        slr::WriteFile file = directory + key;
        file << v->getJson();

        std::cout << v->getJson() << std::endl;
    }
}


LoadContextPointerExtension::ObjectType* LoadContextPointerExtension::loadFileObject(const std::string& objectName)
{
    // slr::JsonLoadContext newContext = slr::JsonLoadContext(*this);
    auto it = alreadySavedObjects.find(objectName);
    if (it == alreadySavedObjects.end())
    {
        throw slr::Exception("Object not loaded.");
    }
    assert(it->second != nullptr);
    return loadObject(*it->second);
}

std::shared_ptr<LoadContextPointerExtension::ObjectType> LoadContextPointerExtension::loadSharedFileObject(slr::json::JsonLoadContext& context, const std::string& objectName)
{
    auto it = objectsMap.find(objectName);
    if (it == objectsMap.end())
    {
        it = objectsMap.emplace_hint(it, objectName, std::shared_ptr<ObjectType>());

        it->second = std::shared_ptr<LoadContextPointerExtension::ObjectType>(loadFileObject(objectName), loadedObjectInstanceDeleter);
    }
    return it->second;
}

std::weak_ptr<LoadContextPointerExtension::ObjectType> LoadContextPointerExtension::loadWeakFileObject(slr::json::JsonLoadContext& context, const std::string& objectName)
{
    return loadSharedFileObject(context, objectName);
}

}
}
}