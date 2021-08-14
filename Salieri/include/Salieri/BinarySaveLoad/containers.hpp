#pragma once

#include <Salieri/salieri.hpp>
#include <Salieri/BinarySaveLoad/context.hpp>
#include <Salieri/BinarySaveLoad/generic.hpp>
#include <Salieri/BinarySaveLoad/primitives.hpp>

#include <vector>
#include <array>
#include <list>
#include <deque>
#include <forward_list>
#include <map>
#include <set>

namespace slr
{

template<typename T>                                          
struct BinaryFileAccess<std::basic_string<T>>                             
{
    // TODO : Do specialization with non trivially copyable types
    static_assert(std::is_trivially_copyable<T>::value);

    void save(std::basic_string<T>& data, BinarySaveContext& context, const BinarySaveInfo& info)
    {
        size_t length = data.length();
        slr::serialize(length, context, slr::BinarySaveInfo());
        context.saveRaw(data.c_str(), sizeof(T) * length);
    }
    void load(std::basic_string<T>& data, BinaryLoadContext& context, const BinaryLoadInfo& info)
    {
        size_t length;
        slr::serialize(length, context, slr::BinaryLoadInfo());
        data.resize(length);
        context.loadRaw(&data[0], sizeof(T) * length);
    }
};

// =========================================== //
// =========== SEQUENCE CONTAINERS =========== //
// =========================================== //

template<typename T>                                          
struct BinaryFileAccess<std::vector<T>>                             
{
    void save(std::vector<T>& container, BinarySaveContext& context, const BinarySaveInfo& info)
    {
        size_t size = container.size();
        slr::serialize(size, context, slr::BinarySaveInfo());

        for (T& elem : container)
        {
            slr::serialize(elem, context, slr::BinarySaveInfo());
        }
    }
    void load(std::vector<T>& container, BinaryLoadContext& context, const BinaryLoadInfo& info)
    {
        size_t size;
        slr::serialize(size, context, slr::BinaryLoadInfo());
        container.resize(size);
        for (T& elem : container)
        {
            slr::serialize(elem, context, slr::BinaryLoadInfo());
        }
    }
};

template<typename T, size_t SIZE>                                          
struct BinaryFileAccess<std::array<T, SIZE>>                             
{
    void save(std::array<T, SIZE>& container, BinarySaveContext& context, const BinarySaveInfo& info)
    {
        for (T& elem : container)
        {
            slr::serialize(elem, context, slr::BinarySaveInfo());
        }
    }
    void load(std::array<T, SIZE>& container, BinaryLoadContext& context, const BinaryLoadInfo& info)
    {
        for (T& elem : container)
        {
            slr::serialize(elem, context, slr::BinaryLoadInfo());
        }
    }
};

template<typename T>                                          
struct BinaryFileAccess<std::deque<T>>                             
{
    void save(std::deque<T>& container, BinarySaveContext& context, const BinarySaveInfo& info)
    {
        size_t size = container.size();
        slr::serialize(size, context, slr::BinarySaveInfo());

        for (T& elem : container)
        {
            slr::serialize(elem, context, slr::BinarySaveInfo());
        }
    }
    void load(std::deque<T>& container, BinaryLoadContext& context, const BinaryLoadInfo& info)
    {
        size_t size;
        slr::serialize(size, context, slr::BinaryLoadInfo());
        container.resize(size);
        for (T& elem : container)
        {
            slr::serialize(elem, context, slr::BinaryLoadInfo());
        }
    }
};

template<typename T>                                          
struct BinaryFileAccess<std::forward_list<T>>                             
{
    void save(std::forward_list<T>& container, BinarySaveContext& context, const BinarySaveInfo& info)
    {
        size_t size = std::distance(container.begin(), container.end()); // TODO : Set size after loop ?
        slr::serialize(size, context, slr::BinarySaveInfo());

        for (T& elem : container)
        {
            slr::serialize(elem, context, slr::BinarySaveInfo());
        }
    }
    void load(std::forward_list<T>& container, BinaryLoadContext& context, const BinaryLoadInfo& info)
    {
        size_t size;
        slr::serialize(size, context, slr::BinaryLoadInfo());

        for (size_t i = 0; i < size; i++)
        {
            container.emplace_front();
            slr::serialize(container.front(), context, slr::BinaryLoadInfo());
        }

        container.reverse(); // TODO : Can be optimized (by removing the reverse)
    }
};

template<typename T>                                          
struct BinaryFileAccess<std::list<T>>                             
{
    void save(std::list<T>& container, BinarySaveContext& context, const BinarySaveInfo& info)
    {
        size_t size = std::distance(container.begin(), container.end()); // TODO : Set size after loop ?
        slr::serialize(size, context, slr::BinarySaveInfo());

        for (T& elem : container)
        {
            slr::serialize(elem, context, slr::BinarySaveInfo());
        }
    }
    void load(std::list<T>& container, BinaryLoadContext& context, const BinaryLoadInfo& info)
    {
        size_t size;
        slr::serialize(size, context, slr::BinaryLoadInfo());

        for (size_t i = 0; i < size; i++)
        {
            container.emplace_back();
            slr::serialize(container.back(), context, slr::BinaryLoadInfo());
        }
    }
};

// ============================================== //
// =========== ASSOCIATIVE CONTAINERS =========== //
// ============================================== //

template<typename T>                                          
struct BinaryFileAccess<std::set<T>>                             
{
    void save(std::set<T>& container, BinarySaveContext& context, const BinarySaveInfo& info)
    {
        size_t size = container.size();
        slr::serialize(size, context, slr::BinarySaveInfo());

        for (auto& elem : container)
        {
            slr::serialize(elem, context, slr::BinarySaveInfo());
        }
    }
    void load(std::set<T>& container, BinaryLoadContext& context, const BinaryLoadInfo& info)
    {
        size_t size;
        slr::serialize(size, context, slr::BinaryLoadInfo());

        for (size_t i = 0; i < size; i++)
        {
            T elem;
            slr::serialize(elem, context, slr::BinaryLoadInfo());
            if constexpr (std::is_trivially_move_constructible<T>::value)
            {
                container.emplace(std::move(elem));
            }
            else 
            {
                container.emplace(elem);
            }
        }
    }
};

template<typename KEY, typename VALUE>                                          
struct BinaryFileAccess<std::pair<KEY, VALUE>>                             
{
    void save(const std::pair<KEY, VALUE>& data, BinarySaveContext& context, const BinarySaveInfo& info)
    {
        slr::serialize(data.first, context, slr::BinarySaveInfo());
        slr::serialize(data.second, context, slr::BinarySaveInfo());
    }
    void load(std::pair<KEY, VALUE>& data, BinaryLoadContext& context, const BinaryLoadInfo& info)
    {
        slr::serialize(data.first, context, slr::BinarySaveInfo());
        slr::serialize(data.second, context, slr::BinarySaveInfo());
    }
};

template<typename KEY, typename VALUE>                                          
struct BinaryFileAccess<std::map<KEY, VALUE>>                             
{
    void save(const std::map<KEY, VALUE>& container, BinarySaveContext& context, const BinarySaveInfo& info)
    {
        size_t size = container.size();
        slr::serialize(size, context, slr::BinarySaveInfo());

        for (const std::pair<KEY, VALUE>& elem : container)
        {
            slr::serialize(elem, context, slr::BinarySaveInfo());
        }
    }
    void load(std::map<KEY, VALUE>& container, BinaryLoadContext& context, const BinaryLoadInfo& info)
    {
        size_t size;
        slr::serialize(size, context, slr::BinaryLoadInfo());

        for (size_t i = 0; i < size; i++)
        {
            KEY key;
            slr::serialize(key, context, slr::BinaryLoadInfo());
            std::pair<typename std::map<KEY, VALUE>::iterator, bool> it;
            if constexpr (std::is_trivially_move_constructible<KEY>::value)
            {
                it = container.emplace(std::move(key), KEY{});
            }
            else 
            {
                it = container.emplace(key, KEY{});
            }
            slr::serialize(it.first->second, context, slr::BinaryLoadInfo());
        }
    }
};

}