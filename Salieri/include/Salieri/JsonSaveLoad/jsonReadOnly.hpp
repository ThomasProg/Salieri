// #pragma once 

// #include <Salieri/JsonSaveLoad/json.hpp>

// namespace slr
// {
// class JsonReadOnly
// {
//     nlohmann::json mainJson;

//     void pushCategory(const std::string& name)
//     {
//         nlohmann::json& newJson = getLastJsonObject()[name];
//         if (newJson.is_discarded())
//         {
//             newJson = nlohmann::json::object();
//         }
//         jsons.emplace(&newJson);
//     }

//     void popCategory()
//     {
//         jsons.pop();
//     }

//     template<typename T>
//     void save(const std::string& name, const T& value)
//     {
//         getLastJsonObject()[name] = value;
//     }
// };

// }