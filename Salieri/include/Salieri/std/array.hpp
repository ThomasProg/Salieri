#pragma once

#include <salieri.hpp>
#include <array>

namespace slr
{
template<class T, size_t SIZE, class CONTEXT_TYPE, class DATA_INFO>
struct Serialize<std::array<T, SIZE>, CONTEXT_TYPE, DATA_INFO>
{
    void operator()(std::vector<T>& container, CONTEXT_TYPE& context, const DATA_INFO& info)
    {
        OnSerializationStart<std::vector<T>, CONTEXT, DATA_INFO>{}(container, context, info);

        for (auto&& elem : container)
        {
            serialize(elem, context, /*info.makeNewInfo()*/info);
        }

        OnSerializationStart<std::vector<T>, CONTEXT, DATA_INFO>{}(container, context, info);
    }
};
}