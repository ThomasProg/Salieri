#pragma once

namespace slr
{

class BinaryContext
{

};

class SafeDataInfo
{

};

using DefaultContext  = BinaryContext;
using DefaultDataInfo = SafeDataInfo; 

template<class DATA, class CONTEXT, class DATA_INFO>
struct Serialize
{
    void operator()(DATA& data, CONTEXT& context, const DATA_INFO& info)
    {
        data.serialize(context);
    }
};

template<class DATA, class CONTEXT, class DATA_INFO>
struct OnSerializationStart
{
    inline void operator()(DATA& data, CONTEXT& context, const DATA_INFO& info)
    {

    }
};

template<class DATA, class CONTEXT, class DATA_INFO>
struct OnSerializationEnd
{
    inline void operator()(DATA& data, CONTEXT& context, const DATA_INFO& info)
    {

    }
};

template<class DATA, class CONTEXT = DefaultContext, class DATA_INFO = DefaultDataInfo>
void serialize(DATA&& data, CONTEXT&& context = DefaultContext(), const DATA_INFO& info = DefaultDataInfo())
{
    // Even though we take by rvalue reference, we DON'T want to std::forward, because :
    // - It would mean having rvalue specialised virtual functions, so this would be a burden
    // - We don't want to "consume" the value or anything like that, so this is useless
    // We also remove references for class specialization because T& should have the same specialization as T&&.
    Serialize<std::remove_reference_t<DATA>, std::remove_reference_t<CONTEXT>, DATA_INFO>{}(data, context, info);
}

}