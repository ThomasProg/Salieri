#include <Salieri/BinarySaveLoad/Extensions/pointer.hpp>
#include <Salieri/BinarySaveLoad/primitives.hpp>

namespace slr
{

void SaveContextPointerExtension::saveFileObject(slr::BinarySaveContext& context, slr::Object<slr::BinarySaveContext>& obj, const std::string& name)
{
    auto v = alreadySavedObjects.find(name);
    if (v == alreadySavedObjects.end())
    {
        alreadySavedObjects.emplace(name); // TODO : can be optimized
        slr::BinarySaveContext newContext = slr::BinarySaveContext((saveFolder + name).c_str(), *this);
        saveObject(newContext, obj);
    }
}




LoadContextPointerExtension::ObjectType* LoadContextPointerExtension::loadFileObject(const std::string& objectName)
{
    slr::BinaryLoadContext newContext = slr::BinaryLoadContext((loadFolder + objectName).c_str(), *this);
    return loadObject(newContext);
}

std::shared_ptr<LoadContextPointerExtension::ObjectType> LoadContextPointerExtension::loadSharedFileObject(slr::BinaryLoadContext& context, const std::string& objectName)
{
    auto it = objectsMap.find(objectName);
    if (it == objectsMap.end())
    {
        it = objectsMap.emplace_hint(it, objectName, std::shared_ptr<ObjectType>());

        it->second = std::shared_ptr<LoadContextPointerExtension::ObjectType>(loadFileObject(objectName), loadedObjectInstanceDeleter);
    }
    return it->second;
}

std::weak_ptr<LoadContextPointerExtension::ObjectType> LoadContextPointerExtension::loadWeakFileObject(slr::BinaryLoadContext& context, const std::string& objectName)
{
    return loadSharedFileObject(context, objectName);
}

}