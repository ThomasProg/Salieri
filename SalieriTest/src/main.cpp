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

class Bar : public slr::Object<slr::BinarySaveLoadContext>, public slr::Object<slr::json::JsonSaveLoadContext>
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

    void serialize(slr::json::JsonSaveContext& context)
    {
        slr::serialize(h, context, slr::json::JsonSaveInfo("h"));
    }

    void serialize(slr::json::JsonLoadContext& context)
    {
        slr::serialize(h, context, slr::json::JsonLoadInfo("h"));
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
    Bar bar4;

    void serialize(slr::json::JsonSaveContext& context)
    {
        slr::serialize(a, context, slr::json::JsonSaveInfo("a"));
        slr::serialize(b, context, slr::json::JsonSaveInfo("b"));
        slr::serialize(c, context, slr::json::JsonSaveInfo("c"));
        slr::serialize(bar, context, slr::json::JsonSaveInfo("bar"));
        // slr::serialize(bar2, context, slr::json::JsonSaveInfo("bar2"));
        // slr::serialize(bar3, context, slr::json::JsonSaveInfo("bar3"));
        slr::serialize(bar4, context, slr::json::JsonSaveInfo("bar4"));
    }

    void serialize(slr::json::JsonLoadContext& context)
    {
        slr::serialize(a, context, slr::json::JsonLoadInfo("a"));
        slr::serialize(b, context, slr::json::JsonLoadInfo("b"));
        slr::serialize(c, context, slr::json::JsonLoadInfo("c"));
        slr::serialize(bar, context, slr::json::JsonLoadInfo("bar"));
        // slr::serialize(bar2, context, slr::json::JsonLoadInfo("bar2"));
        // slr::serialize(bar3, context, slr::json::JsonLoadInfo("bar3"));
        slr::serialize(bar4, context, slr::json::JsonLoadInfo("bar4"));
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


class JsonSaveContextExtension : public slr::json::ptr::SaveContextPointerExtension
{
    virtual void saveObject(slr::json::JsonSaveContext& context, slr::Object<slr::json::JsonSaveContext>& obj) override;
};

void JsonSaveContextExtension::saveObject(slr::json::JsonSaveContext& context, slr::Object<slr::json::JsonSaveContext>& obj)
{
    // Save Object type info
    // size_t typeID = obj.getTypeID();
    // slr::serialize(typeID, context, slr::JsonSaveInfo());

    // Save Object insides
    // slr::serialize(obj, context, slr::JsonSaveInfo("obj"));
    obj.serialize(context);
}

class JsonLoadContextExtension : public slr::json::ptr::LoadContextPointerExtension
{
    virtual ObjectType* loadObject(slr::json::JsonLoadContext& context) override;
    virtual ObjectType* loadFileObject(const std::string& objectName) override; // Loads an object from another context
};

slr::json::ptr::LoadContextPointerExtension::ObjectType* JsonLoadContextExtension::loadObject(slr::json::JsonLoadContext& context)
{
    // size_t typeID;
    // slr::serialize(typeID, context, slr::JsonLoadInfo());
    LoadContextPointerExtension::ObjectType* ptr = new Bar();
    // slr::serialize(*ptr, context, slr::JsonLoadInfo("obj"));
    ptr->serialize(context);
    return ptr;
}

#include <Salieri/file.hpp>

JsonLoadContextExtension::ObjectType* JsonLoadContextExtension::loadFileObject(const std::string& objectName)
{
    auto it = alreadySavedObjects.find(objectName);
    slr::json::JsonLoadContext* newContext; 
    if (it == alreadySavedObjects.end()) 
    {
        // Load new file
        newContext = new slr::json::JsonLoadContext(*this);
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
void test<slr::json::JsonSaveLoadContext>()
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

// bool isDifferent(nlohmann::json& doc1, nlohmann::json& doc2)
// {
//     if (doc1.is_object())
//     {
//         bool isDifferent = false;
//         for (auto& doc : doc1)
//         {
            
//         }
//     }
//     else 
//     {
//         doc1.dif
//     }
// }


static nlohmann::json diff(const nlohmann::json& source, const nlohmann::json& target,
                        const std::string& path = "")
{
    // the patch
    nlohmann::json result({});

    // if the values are the same, return empty patch
    if (source == target)
    {
        return result;
    }

    if (source.type() != target.type())
    {
        // different types: replace value
        result = target;
        return result;
    }

    switch (source.type())
    {
        case nlohmann::json::value_t::array:
        {
            // // first pass: traverse common elements
            // std::size_t i = 0;
            // while (i < source.size() && i < target.size())
            // {
            //     // recursive call to compare array values at index i
            //     auto temp_diff = diff(source[i], target[i], path + "/" + std::to_string(i));
            //     result.insert(result.end(), temp_diff.begin(), temp_diff.end());
            //     ++i;
            // }

            // // i now reached the end of at least one array
            // // in a second pass, traverse the remaining elements

            // // remove my remaining elements
            // const auto end_index = static_cast<nlohmann::json::difference_type>(result.size());
            // while (i < source.size())
            // {
            //     // add operations in reverse order to avoid invalid
            //     // indices
            //     result.insert(result.begin() + end_index, object(
            //     {
            //         {"op", "remove"},
            //         {"path", path + "/" + std::to_string(i)}
            //     }));
            //     ++i;
            // }

            // // add other remaining elements
            // while (i < target.size())
            // {
            //     result.push_back(
            //     {
            //         {"op", "add"},
            //         {"path", path + "/-"},
            //         {"value", target[i]}
            //     });
            //     ++i;
            // }

            break;
        }

        case nlohmann::json::value_t::object:
        {
            // first pass: traverse this object's elements
            for (auto it = source.cbegin(); it != source.cend(); ++it)
            {
                // escape the key name to be used in a JSON patch
                const auto path_key = path + it.key();

                if (target.find(it.key()) != target.end())
                {
                    // recursive call to compare object values at key it
                    auto temp_diff = diff(it.value(), target[it.key()], it.key()/*path_key*/);

                    for (auto it2 = temp_diff.cbegin(); it2 != temp_diff.cend(); ++it2)
                    {
                        if (it->is_object())
                        {
                            result[it.key()][it2.key()] = it2.value();
                        }
                        else 
                        {
                            result[it.key()] = it2.value();
                        }
                    }
                }
                else
                {
                    // found a key that is not in o -> remove it
                    result[it.key()] = nullptr;
                }
            }

            // second pass: traverse other object's elements
            for (auto it = target.cbegin(); it != target.cend(); ++it)
            {
                if (source.find(it.key()) == source.end())
                {
                    // found a key that is not in this -> add it
                    result[it.key()] = it.value();
                }
            }

            break;
        }

        default:
        {
            result = target;
            break;
        }
    }

    return result;
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

    // slr::json::Doc doc;
    slr::json::ptr::Doc doc;
    {
        {
        int i = 5;
        doc.slr::json::Doc::save<JsonSaveContextExtension, decltype(i)>(i, slr::json::JsonSaveInfo{"i"});
        }
        {
        int i = 2;
        doc.slr::json::Doc::load<JsonLoadContextExtension, decltype(i)>(i, slr::json::JsonLoadInfo{"i"});
        std::cout << i << std::endl;
        }
    }

    {
        JsonSaveContextExtension shared1;
        auto& context = shared1.globalContext;
        // slr::JsonSaveContext context(shared1);
        float f = 0.1f;
        // slr::JsonSaveContext::DefaultExtension shared;
        // slr::JsonSaveContext context(shared);
        slr::serialize(f, context, slr::json::JsonSaveInfo("f"));
        int u = 889;
        slr::serialize(u, context, slr::json::JsonSaveInfo("u"));

        std::string str = "Hello world!";
        slr::serialize(str, context, slr::json::JsonSaveInfo("str"));

        str = "Hello world!!!!!!!!!";
        slr::serialize(str, context, slr::json::JsonSaveInfo("str"));


        Foo foo;
        foo.a = 9;
        foo.bar = std::make_shared<Bar>();
        // foo.bar->h = 3;
        // slr::serialize(foo, context, slr::json::JsonSaveInfo("foo"));
        foo.bar->h = 100;
        slr::serialize(foo, context, slr::json::JsonSaveInfo("foo"));
        foo.bar4.h = 3;
        nlohmann::json doc1 = shared1.globalContext.mainJson;
        slr::serialize(foo, context, slr::json::JsonSaveInfo("foo"));


        shared1.toDisk(dirName);

        nlohmann::json doc2 = diff(doc1, shared1.globalContext.mainJson);

        std::cout << "=======\n";
        std::cout << diff(doc1, shared1.globalContext.mainJson).dump(3) << std::endl;
        // std::cout << shared1.globalContext.getJson() << std::endl;
        std::cout << "=======\n";
    }

    {
        JsonLoadContextExtension shared1;
        auto& context = shared1.globalContext;

        shared1.fromDisk(dirName);

        float f;
        slr::serialize(f, context, slr::json::JsonLoadInfo("f"));
        int u;
        slr::serialize(u, context, slr::json::JsonLoadInfo("u"));
        std::string str;
        slr::serialize(str, context, slr::json::JsonLoadInfo("str"));
        Foo foo;
        slr::serialize(foo, context, slr::json::JsonLoadInfo("foo"));

        std::cout << f << " / " << u << " / " << str << std::endl;
        std::cout << foo.a << std::endl;
        std::cout << foo.bar->h << std::endl;
    }
}






