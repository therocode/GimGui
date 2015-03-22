#pragma once
#include <map>
#include <gimgui/assert.hpp>

namespace gim
{
    template<typename ToType, typename FromType>
    ToType resolve(const FromType& from, const std::map<FromType, ToType>& dictionary, const ToType& defaultResult);
    template<typename ToType, typename FromType>
    ToType resolve(const FromType& from, const std::map<FromType, ToType>& dictionary);

#include <gimgui/util/resolve.inl>
}
