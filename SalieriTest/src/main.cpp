#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>

#include <Salieri/salieri.hpp>

#include <Salieri/Inspector/std/vector.hpp>
// #include <Salieri/Displayer/primitives.hpp>
#include <Salieri/BinarySaveLoad/context.hpp>
#include <Salieri/BinarySaveLoad/primitives.hpp>
#include <Salieri/BinarySaveLoad/generic.hpp>
#include <Salieri/BinarySaveLoad/Extensions/pointer.hpp>
#include <Salieri/BinarySaveLoad/containers.hpp>

#include <Salieri/JsonSaveLoad/context.hpp>
#include <Salieri/JsonSaveLoad/primitives.hpp>
#include <Salieri/JsonSaveLoad/Extensions/pointer.hpp>

#include <Salieri/exceptions.hpp>

class Bar : public slr::Object<slr::BinarySaveLoadContext>, public slr::Object<slr::JsonSaveLoadContext>
{
public:
    std::string name = "Bar2";
    int h = 4;

    void serialize(slr::BinarySaveContext& context)
    {
        slr::serialize(h, context, slr::BinarySaveInfo());
    }

    void serialize(slr::BinaryLoadContext& context)
    {
        slr::serialize(h, context, slr::BinaryLoadInfo());
    }

    void serialize(slr::JsonSaveContext& context)
    {
        slr::serialize(h, context, slr::JsonSaveInfo("h"));
    }

    void serialize(slr::JsonLoadContext& context)
    {
        slr::serialize(h, context, slr::JsonLoadInfo("h"));
    }

    std::string& getName()
    {
        return name;
    }
};

class Foo : public slr::Object<slr::BinarySaveLoadContext>
{
public:
    std::string name = "Foo1";
    int a = 3;
    char b = 'a';
    std::string c = "hello";
    std::shared_ptr<Bar> bar;
    std::weak_ptr<Bar> bar2;
    std::unique_ptr<Bar> bar3;

    void serialize(slr::JsonSaveContext& context)
    {
        slr::serialize(a, context, slr::JsonSaveInfo("a"));
        slr::serialize(b, context, slr::JsonSaveInfo("b"));
        slr::serialize(c, context, slr::JsonSaveInfo("c"));
        slr::serialize(bar, context, slr::JsonSaveInfo("bar"));
        // slr::serialize(bar2, context, slr::JsonSaveInfo("bar2"));
        // slr::serialize(bar3, context, slr::JsonSaveInfo("bar3"));
    }

    void serialize(slr::JsonLoadContext& context)
    {
        slr::serialize(a, context, slr::JsonLoadInfo("a"));
        slr::serialize(b, context, slr::JsonLoadInfo("b"));
        slr::serialize(c, context, slr::JsonLoadInfo("c"));
        slr::serialize(bar, context, slr::JsonLoadInfo("bar"));
        // slr::serialize(bar2, context, slr::JsonSaveInfo("bar2"));
        // slr::serialize(bar3, context, slr::JsonSaveInfo("bar3"));
    }

    void serialize(slr::BinarySaveContext& context)
    {
        slr::serialize(a, context, slr::BinarySaveInfo());
        slr::serialize(b, context, slr::BinarySaveInfo());
        slr::serialize(c, context, slr::BinarySaveInfo());
        slr::serialize(bar, context, slr::BinarySaveInfo());
        slr::serialize(bar2, context, slr::BinarySaveInfo());
        slr::serialize(bar3, context, slr::BinarySaveInfo());
    }

    void serialize(slr::BinaryLoadContext& context)
    {
        slr::serialize(a, context, slr::BinaryLoadInfo());
        slr::serialize(b, context, slr::BinaryLoadInfo());
        slr::serialize(c, context, slr::BinaryLoadInfo());
        slr::serialize(bar, context, slr::BinaryLoadInfo());
        slr::serialize(bar2, context, slr::BinaryLoadInfo());
        slr::serialize(bar3, context, slr::BinaryLoadInfo());
    }
};

class BinarySaveContextExtension : public slr::SaveContextPointerExtension
{
    virtual void saveObject(slr::BinarySaveContext& context, slr::Object<slr::BinarySaveContext>& obj) override;
};

void BinarySaveContextExtension::saveObject(slr::BinarySaveContext& context, slr::Object<slr::BinarySaveContext>& obj)
{
    // Save Object type info
    // size_t typeID = obj.getTypeID();
    // slr::serialize(typeID, context, slr::BinarySaveInfo());

    // Save Object insides
    slr::serialize(obj, context, slr::BinaryLoadInfo());
}

class BinaryLoadContextExtension : public slr::LoadContextPointerExtension
{
    virtual ObjectType* loadObject(slr::BinaryLoadContext& context) override;
};

slr::LoadContextPointerExtension::ObjectType* BinaryLoadContextExtension::loadObject(slr::BinaryLoadContext& context)
{
    // size_t typeID;
    // slr::serialize(typeID, context, slr::BinaryLoadInfo());
    LoadContextPointerExtension::ObjectType* ptr = new Bar();
    slr::serialize(*ptr, context, slr::BinaryLoadInfo());
    return ptr;
}


class JsonSaveContextExtension : public slr::json::SaveContextPointerExtension
{
    virtual void saveObject(slr::JsonSaveContext& context, slr::Object<slr::JsonSaveContext>& obj) override;
};

void JsonSaveContextExtension::saveObject(slr::JsonSaveContext& context, slr::Object<slr::JsonSaveContext>& obj)
{
    // Save Object type info
    // size_t typeID = obj.getTypeID();
    // slr::serialize(typeID, context, slr::JsonSaveInfo());

    // Save Object insides
    // slr::serialize(obj, context, slr::JsonSaveInfo("obj"));
    obj.serialize(context);
}

class JsonLoadContextExtension : public slr::json::LoadContextPointerExtension
{
    virtual ObjectType* loadObject(slr::JsonLoadContext& context) override;
    virtual ObjectType* loadFileObject(const std::string& objectName) override; // Loads an object from another context
};

slr::json::LoadContextPointerExtension::ObjectType* JsonLoadContextExtension::loadObject(slr::JsonLoadContext& context)
{
    // size_t typeID;
    // slr::serialize(typeID, context, slr::JsonLoadInfo());
    LoadContextPointerExtension::ObjectType* ptr = new Bar();
    // slr::serialize(*ptr, context, slr::JsonLoadInfo("obj"));
    ptr->serialize(context);
    return ptr;
}

#include <fstream>
#include <iostream>
#include <sstream> //std::stringstream

#include <Salieri/file.hpp>

JsonLoadContextExtension::ObjectType* JsonLoadContextExtension::loadFileObject(const std::string& objectName)
{
    auto it = alreadySavedObjects.find(objectName);
    slr::JsonLoadContext* newContext; 
    if (it == alreadySavedObjects.end()) 
    {
        // Load new file
        newContext = new slr::JsonLoadContext(*this);
        slr::ReadFile f = ("SavedJSON/" + objectName);
        newContext->parse(f.toStr());
        alreadySavedObjects.emplace(objectName, newContext);
    }
    else 
    {
        // Use already loaded file
        newContext = it->second;
    }
    return loadObject(*newContext);
}

void saveTest()
{
    Foo f;
    f.bar2 = f.bar = std::make_shared<Bar>();
    f.bar->h = 9;
    f.bar3 = std::make_unique<Bar>();
    f.bar3->h = 15;
    BinarySaveContextExtension shared;
    slr::BinarySaveContext context((sSaveFolder + f.name).c_str(), shared);

    auto start = std::chrono::steady_clock::now();

    // for (int i = 0; i < 1000 * 1000; i++)
    {
        // for (int i = 0; i < 1000 * 1000; i++)
        slr::serialize(f, context, slr::BinarySaveInfo());
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
}

void loadTest()
{
    Foo f;
    f.a = 4;
    f.c = "yo";
    BinaryLoadContextExtension shared;
    slr::BinaryLoadContext context((sSaveFolder + f.name).c_str(), shared);
    // for (int i = 0; i < 1000 * 1000; i++)
    {
    // for (int i = 0; i < 1000 * 1000; i++)
    slr::serialize(f, context, slr::BinaryLoadInfo());
    }

    std::cout << f.a << " / " << f.b << " / " << f.c << " / " << f.bar.get() << " / " << f.bar2.lock().get() << std::endl;
    std::cout << "Bar : "  << f.bar->h << std::endl;
    std::cout << f.bar3->h << std::endl;
}

template<typename CONTEXT>
void test();

template<>
void test<slr::BinarySaveLoadContext>()
{
    std::cout << std::boolalpha << std::is_trivially_copy_assignable<BinarySaveContextExtension>::value << std::endl;

    // std::shared_ptr<void> a = std::make_unique<int>(4);
    // std::shared_ptr<int> b = std::static_pointer_cast<int>(a);

    try 
    {
        saveTest();
        loadTest();

    
        std::map<int, int> a;
        a.emplace(1, 2);

        {
            BinarySaveContextExtension shared;
            slr::BinarySaveContext context((sSaveFolder + "test").c_str(), shared);
            slr::serialize(a, context, slr::BinarySaveInfo());
        }

        a.clear();

        {
            BinaryLoadContextExtension shared;
            slr::BinaryLoadContext context((sSaveFolder + "test").c_str(), shared);
            slr::serialize(a, context, slr::BinaryLoadInfo());
        }
        
        for (auto& b : a)
        {
            std::cout << b.first << " / " << b.second << std::endl;
        }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    // std::cout << "Hello" << std::endl;

    // float f = 1;

    // std::vector<float> container;
    // for (int i = 0; i < 10; i++)
    // {
    //     container.emplace_back(float(i));
    // }

    // slr::serialize(container, slr::DisplayContext());

    // const char* s ="Hello world!";
    // slr::serialize(s, slr::DisplayContext());
    // slr::serialize("Test array !", slr::DisplayContext());
}

template<>
void test<slr::JsonSaveLoadContext>()
{
    try 
    {
        saveTest();
        loadTest();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

int main()
{
    // test<slr::JsonSaveLoadContext>();

    // nlohmann::json j;
    // auto& p = j["object"];
    // p = 3;
    // p = 4;

    // p = nlohmann::json::object();
    // p["o"] = "3";

    // std::string file;

    std::string dirName = "SavedJSON/";

    {
        JsonSaveContextExtension shared1;
        auto& context = shared1.globalContext;
        // slr::JsonSaveContext context(shared1);
        float f = 0.1f;
        // slr::JsonSaveContext::DefaultExtension shared;
        // slr::JsonSaveContext context(shared);
        slr::serialize(f, context, slr::JsonSaveInfo("f"));
        int u = 889;
        slr::serialize(u, context, slr::JsonSaveInfo("u"));
        std::string str = "Hello world!";
        slr::serialize(str, context, slr::JsonSaveInfo("str"));
        Foo foo;
        foo.a = 9;
        foo.bar = std::make_shared<Bar>();
        foo.bar->h = 100;
        slr::serialize(foo, context, slr::JsonSaveInfo("foo"));

        shared1.toDisk(dirName);
    }

    {
        JsonLoadContextExtension shared1;
        auto& context = shared1.globalContext;
        slr::ReadFile file = dirName + "__global";
        shared1.globalContext.parse(file.toStr());

        shared1.fromDisk(dirName);

        float f;
        slr::serialize(f, context, slr::JsonLoadInfo("f"));
        int u;
        slr::serialize(u, context, slr::JsonLoadInfo("u"));
        std::string str;
        slr::serialize(str, context, slr::JsonLoadInfo("str"));
        Foo foo;
        slr::serialize(foo, context, slr::JsonLoadInfo("foo"));

        std::cout << f << " / " << u << " / " << str << std::endl;
        std::cout << foo.a << std::endl;
        std::cout << foo.bar->h << std::endl;
    }
}






