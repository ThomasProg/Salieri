#include <iostream>
#include <vector>

#include <Salieri/salieri.hpp>
#include <Salieri/std/vector.hpp>





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








int main()
{
    std::cout << "Hello" << std::endl;

    float f = 1;

    std::vector<float> container;
    for (int i = 0; i < 10; i++)
    {
        container.emplace_back(i);
    }
    slr::DefaultContext context;
    slr::serialize(container, context);
}