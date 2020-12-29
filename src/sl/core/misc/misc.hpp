#pragma once

#include <typeindex>
#include <typeinfo>

#define TYPE_INDEX(T) std::type_index(typeid(T))
