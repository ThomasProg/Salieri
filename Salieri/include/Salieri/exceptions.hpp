#pragma once

#include <exception>

namespace slr
{
    struct Exception : public std::exception
    {
        const char* desc;

        Exception(const char* description) : desc(description)
        {

        }

        const char* what() const throw() final
        {
            return desc;
        }
    };
}