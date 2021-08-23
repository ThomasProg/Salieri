#pragma once

#include <exception>

namespace slr
{
    struct Exception : public std::exception
    {
        std::string desc;

        Exception(const char* description) : desc(description)
        {

        }

        Exception(const std::string& description) : desc(description)
        {

        }

        const char* what() const throw() final
        {
            return desc.c_str();
        }
    };
}