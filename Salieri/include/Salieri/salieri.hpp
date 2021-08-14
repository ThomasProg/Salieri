#pragma once

#include <type_traits>
#include <Salieri/additionnalDebugIncludes.hpp>
// #include <unordered_map>
// #include <map>

namespace slr
{

// ===============================================================================//
// ======================== SET DEFAULT CONTEXT AND INFO ======================== // 
// ===============================================================================//

// template<class KeyType>
// class SharedPtrSupport
// {
// private:
//     // std::unordered_map<KeyType, void*> sharedPtrs;

// protected:
//     void* retrieve()
//     {
//         return nullptr;
//     }
// };

class BaseContext
{
public:
    // std::map<void*, LoadedPtr> alreadyLoadedPtrs;
};

class SafeDataInfo
{

};

using DefaultContext  = BaseContext;
using DefaultDataInfo = SafeDataInfo; 

// ===============================================================================//
// ======================== SET BASE SERIALIZE FUNCTIONS ======================== // 
// ===============================================================================//


template<class DATA, class CONTEXT, class DATA_INFO>
struct Serialize
{
    void operator()(DATA& data, CONTEXT& context, const DATA_INFO& info)
    {
        data.serialize(context);
    }
};

// ===============================================================================//
// ============================== UTILITY FUNCTION ============================== // 
// ===============================================================================//

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