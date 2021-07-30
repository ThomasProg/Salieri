#include <iostream>
#include <vector>

#include <Salieri/salieri.hpp>
#include <Salieri/Inspector/std/vector.hpp>
// #include <Salieri/Displayer/primitives.hpp>
#include <Salieri/FileContext/saveLoadFile.hpp>
#include <Salieri/FileContext/primitives.hpp>
#include <Salieri/FileContext/generic.hpp>

// template< class T, class CONTEXT, class DATA_INFO>
// struct Serialize<std::vector<T>, CONTEXT, DATA_INFO>
// {
// public:
//     void operator()(std::vector<T>& container, CONTEXT&& context, const DATA_INFO& info)
//     {
//         for (T& elem : container)
//         {
//             Serialize<T, CONTEXT, DATA_INFO>{}(elem, context, info/*DATA_INFO(info)*/);
//         }
//     }
// };

// template<class DATA, class CONTEXT = DefaultContext, class DATA_INFO = DefaultDataInfo>
// void serialize(DATA&& data, CONTEXT&& context = DefaultContext(), const DATA_INFO& info = DefaultDataInfo())
// {
//     // Even though we take by rvalue reference, we DON'T want to std::forward, because :
//     // - It would mean having rvalue specialised virtual functions, so this would be a burden
//     // - We don't want to "consume" the value or anything like that, so this is useless
//     // We also remove references for class specialization because T& should have the same specialization as T&&.
//     Serialize<std::remove_reference_t<DATA>, std::remove_reference_t<CONTEXT>, DATA_INFO>{}(data, context, info);
// }

class Bar : public slr::Object<slr::BinarySaveLoadContext>
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

    std::string& getName()
    {
        return name;
    }

    virtual std::size_t getTypeID() override 
    {
        return 101;
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
    std::shared_ptr<Bar> bar2;

    void serialize(slr::BinarySaveContext& context)
    {
        slr::serialize(a, context, slr::BinarySaveInfo());
        slr::serialize(b, context, slr::BinarySaveInfo());
        slr::serialize(c, context, slr::BinarySaveInfo());
        slr::serialize(bar, context, slr::BinarySaveInfo());
        slr::serialize(bar2, context, slr::BinarySaveInfo());
    }

    void serialize(slr::BinaryLoadContext& context)
    {
        slr::serialize(a, context, slr::BinaryLoadInfo());
        slr::serialize(b, context, slr::BinaryLoadInfo());
        slr::serialize(c, context, slr::BinaryLoadInfo());
        slr::serialize(bar, context, slr::BinaryLoadInfo());
        slr::serialize(bar2, context, slr::BinaryLoadInfo());
    }

    virtual std::size_t getTypeID() override 
    {
        return 100;
    }
};

// TODO
slr::Object<slr::BinaryLoadContext>* constructInstance(size_t)
{
    // class Test : public slr::Object<slr::BinarySaveLoadContext>
    // {
    //     virtual void serialize(slr::BinarySaveContext&) override
    //     {

    //     }

    //     virtual void serialize(slr::BinaryLoadContext&) override
    //     {

    //     }

    //     virtual std::size_t getTypeID() override 
    //     {
    //         return 1;
    //     }
    // };
    return new Bar();
}

void destroyInstance(slr::Object<slr::BinaryLoadContext>* p)
{
    delete p;
}

void saveTest()
{
    Foo f;
    f.bar2 = f.bar = std::make_shared<Bar>();
    f.bar->h = 9;
    slr::BinarySaveContext::SharedContext shared;
    shared.saveObjectTypeInfo = [](slr::Object<slr::BinarySaveContext>& obj, slr::BinarySaveContext& context)
        {
            size_t typeID = obj.getTypeID();
            slr::serialize(typeID, context, slr::BinarySaveInfo());
        };
    slr::BinarySaveContext context(f.name.c_str(), shared);
    for (int i = 0; i < 1000 * 1000; i++)
    {
    // for (int i = 0; i < 1000 * 1000; i++)
    slr::serialize(f, context, slr::BinarySaveInfo());
    }
}

void loadTest()
{
    Foo f;
    f.a = 4;
    f.c = "yo";
    slr::BinaryLoadContext::SharedContext shared;
    shared.loadObjectInstance = [](slr::BinaryLoadContext& context)
        {
            size_t typeID;
            slr::serialize(typeID, context, slr::BinaryLoadInfo());
            return constructInstance(typeID);
            // return nullptr;
        };
    shared.loadedObjectInstanceDeleter = [](slr::Object<slr::BinaryLoadContext>* obj)
    {
        delete obj;
    };
    slr::BinaryLoadContext context(f.name.c_str(), shared);
    for (int i = 0; i < 1000 * 1000; i++)
    {
    // for (int i = 0; i < 1000 * 1000; i++)
    slr::serialize(f, context, slr::BinaryLoadInfo());
    }

    std::cout << f.a << " / " << f.b << " / " << f.c << " / " << f.bar.get() << " / " << f.bar2.get() << std::endl;
    std::cout << "Bar : "  << f.bar->h << std::endl;
}

int main()
{
    // std::shared_ptr<void> a = std::make_unique<int>(4);
    // std::shared_ptr<int> b = std::static_pointer_cast<int>(a);

    try 
    {
        saveTest();
        loadTest();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }


    return 0;

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