#pragma once

#include <cstdio>
#include <string>
#include <Salieri/exceptions.hpp>

namespace slr
{
class ReadFile
{
    FILE* file;

public:
    ReadFile(const char* filename)
    {
        file = fopen(filename, "rb");
        if (file == nullptr)
        {
            throw slr::Exception("File couldn't be opened for reading");
        }
    }

    ReadFile(const std::string& s) : ReadFile(s.c_str())
    {

    }

    std::string toStr() const
    {
        fseek(file, 0, SEEK_END);
        size_t length = ftell(file);
        fseek(file, 0, SEEK_SET);
        std::string s;
        s.resize(length);
        fread(&s[0], 1, length, file);
        return s;
    }

    ~ReadFile()
    {
        fclose(file);
    }

    ReadFile& operator>>(std::string& s)
    {
        s = toStr();
        return *this;
    }
};

class WriteFile
{
    FILE* file;

public:
    WriteFile(const char* filename)
    {
        file = fopen(filename, "wb");
        if (file == nullptr)
        {
            throw slr::Exception("File couldn't be opened for writing");
        }
    }

    WriteFile(const std::string& s) : WriteFile(s.c_str())
    {

    }

    void setTo(const std::string& s)
    {
        fwrite(s.c_str(), sizeof(char), s.size(), file);
    }

    ~WriteFile()
    {
        fclose(file);
    }

    WriteFile& operator<<(const std::string& s)
    {
        setTo(s);
        return *this;
    }
};
}