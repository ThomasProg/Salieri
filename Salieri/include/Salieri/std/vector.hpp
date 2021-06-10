#pragma once

#include <Salieri/salieri.hpp>
#include <vector>

namespace slr
{
template<class T, class CONTEXT, class DATA_INFO>
struct Serialize<std::vector<T>, CONTEXT, DATA_INFO>
{
    void operator()(std::vector<T>& container, CONTEXT& context, const DATA_INFO& info)
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