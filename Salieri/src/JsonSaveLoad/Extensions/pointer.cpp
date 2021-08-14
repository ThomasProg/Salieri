#include <Salieri/JsonSaveLoad/Extensions/pointer.hpp>
#include <Salieri/JsonSaveLoad/primitives.hpp>

namespace slr
{
namespace json
{

void SaveContextPointerExtension::saveFileObject(slr::JsonSaveContext& context, slr::Object<slr::JsonSaveContext>& obj, const std::string& name)
{
    auto v = alreadySavedObjects.find(name);
    if (v == alreadySavedObjects.end())
    {
        alreadySavedObjects.emplace(name, this); // TODO : can be optimized
        slr::JsonSaveContext newContext = slr::JsonSaveContext(*this);
        saveObject(newContext, obj);
    }
}




LoadContextPointerExtension::ObjectType* LoadContextPointerExtension::loadFileObject(const std::string& objectName)
{
    slr::JsonLoadContext newContext = slr::JsonLoadContext(*this);
    return loadObject(newContext);
}

std::shared_ptr<LoadContextPointerExtension::ObjectType> LoadContextPointerExtension::loadSharedFileObject(slr::JsonLoadContext& context, const std::string& objectName)
{
    auto it = objectsMap.find(objectName);
    if (it == objectsMap.end())
    {
        it = objectsMap.emplace_hint(it, objectName, std::shared_ptr<ObjectType>());

        it->second = std::shared_ptr<LoadContextPointerExtension::ObjectType>(loadFileObject(objectName), loadedObjectInstanceDeleter);
    }
    return it->second;
}

std::weak_ptr<LoadContextPointerExtension::ObjectType> LoadContextPointerExtension::loadWeakFileObject(slr::JsonLoadContext& context, const std::string& objectName)
{
    return loadSharedFileObject(context, objectName);
}

}
}