#include <Salieri/JsonSaveLoad/context.hpp>
#include <Salieri/exceptions.hpp>
#include <Salieri/JsonSaveLoad/generic.hpp>
#include <Salieri/JsonSaveLoad/primitives.hpp>

#include <cstdio>
#include <cassert>
#include <iostream>

#include <Salieri/file.hpp>

namespace slr
{

JsonSaveContext::JsonSaveContext(DefaultSaveExtension& sharedContext)
    : shared(sharedContext)
{

}

JsonLoadContext::JsonLoadContext(DefaultLoadExtension& sharedContext)
    : shared(sharedContext)
{

}

void DefaultSaveExtension::toDisk(const std::string& directory, const std::string& globalContextName)
{
    slr::WriteFile file = directory + globalContextName;
    file << globalContext.getJson();
}

}