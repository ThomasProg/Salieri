#pragma once

#include <Salieri/salieri.hpp>
#include <Salieri/FileContext/saveLoadFile.hpp>
#include <Salieri/FileContext/binaryFileAccess.hpp>

namespace slr
{

#define SAVE_LOAD_PRIMITIVE_AS_RAW(DATA_TYPE)    \
template<>                                          \
struct BinaryFileAccess<DATA_TYPE>                             \
{\
    void save(DATA_TYPE& data, BinarySaveContext& context, const BinarySaveInfo& info)\
    {\
        context.saveRaw((void*)&data, sizeof(data));\
    }\
    void load(DATA_TYPE& data, BinaryLoadContext& context, const BinaryLoadInfo& info)\
    {\
        context.loadRaw((void*)&data, sizeof(data));\
    }\
};

SAVE_LOAD_PRIMITIVE_AS_RAW(bool)
SAVE_LOAD_PRIMITIVE_AS_RAW(char)
SAVE_LOAD_PRIMITIVE_AS_RAW(signed char)
SAVE_LOAD_PRIMITIVE_AS_RAW(unsigned char)
SAVE_LOAD_PRIMITIVE_AS_RAW(int)
SAVE_LOAD_PRIMITIVE_AS_RAW(unsigned int)
SAVE_LOAD_PRIMITIVE_AS_RAW(size_t)
SAVE_LOAD_PRIMITIVE_AS_RAW(float)
SAVE_LOAD_PRIMITIVE_AS_RAW(double)
SAVE_LOAD_PRIMITIVE_AS_RAW(const char*)

#undef SAVE_LOAD_PRIMITIVE_AS_RAW


template<>                                          
struct BinaryFileAccess<std::string>                             
{
    void save(std::string& data, BinarySaveContext& context, const BinarySaveInfo& info)
    {
        size_t length = data.length();
        slr::serialize(length, context, slr::BinarySaveInfo());
        context.saveRaw(data.c_str(), sizeof(char) * length);
    }
    void load(std::string& data, BinaryLoadContext& context, const BinaryLoadInfo& info)
    {
        size_t length;
        slr::serialize(length, context, slr::BinaryLoadInfo());
        data.resize(length);
        context.loadRaw(&data[0], sizeof(char) * length);
    }
};

// template<typename T>                                          
// struct BinaryFileAccess<slr::ControlBlockPtr<T>>                             
// {
//     void save(slr::ControlBlockPtr<T>& data, BinarySaveContext& context, const BinarySaveInfo& info)
//     {
//         // TODO
//         T* ptr =  data.get();
//         if (ptr != nullptr)
//         {
//             std::string name = ptr->getName();
//             slr::serialize(name, context, slr::BinarySaveInfo());
//             // context.saveObjectAsync(name, data);
//         }
//     }
//     void load(slr::ControlBlockPtr<T>& data, BinaryLoadContext& context, const BinaryLoadInfo& info)
//     {
//         // TODO
//         std::string name;
//         slr::serialize(name, context, slr::BinaryLoadInfo());
//         // context.loadObjectAsync(name, data);
//     }
// };


template<typename T>                                          
struct BinaryFileAccess<std::unique_ptr<T>>                         
{
    void save(std::unique_ptr<slr::Object<BinarySaveLoadContext>>& data, BinarySaveContext& context, const BinarySaveInfo& info)
    {
        T* ptr =  data.get();
        bool doesPtrExist = ptr != nullptr;
        slr::serialize(doesPtrExist, context, slr::BinarySaveInfo());
        if (doesPtrExist)
        {
            // std::string name = ptr->getName();
            // slr::serialize(name, context, slr::BinarySaveInfo());
            // context.saveObject(*data.get(), name);
        }
    }
    void load(std::unique_ptr<slr::Object<BinarySaveLoadContext>>& data, BinaryLoadContext& context, const BinaryLoadInfo& info)
    {
        bool doesPtrExist;
        slr::serialize(doesPtrExist, context, slr::BinaryLoadInfo());

        if (doesPtrExist)
        {
            // std::string name;
            // slr::serialize(name, context, slr::BinaryLoadInfo());
            // data = std::static_pointer_cast<T>(context.loadObject(name));
        }
    }
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
            context.saveFileObject(*data.get(), name);
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
            data = std::static_pointer_cast<T>(context.loadFileObject(name));
        }
    }
};



}