#include <Salieri/JsonSaveLoad/context.hpp>
#include <Salieri/exceptions.hpp>
#include <Salieri/JsonSaveLoad/generic.hpp>
#include <Salieri/JsonSaveLoad/primitives.hpp>

#include <cstdio>
#include <cassert>
#include <iostream>

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

}