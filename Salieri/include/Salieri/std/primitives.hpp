#pragma once

#include <salieri.hpp>

namespace slr
{
template<class CONTEXT, class DATA_INFO>
struct Serialize<float, CONTEXT, DATA_INFO>
{
    void operator()(float& data, CONTEXT& context, const DATA_INFO& info)
    {
        std::cout << "Float serialized : " << data << std::endl;
    }
};
}