#pragma once
#include <gimgui/data/element.hpp>
#include <string>

namespace gim
{
    template <typename AttributeType>
    AttributeType getOrFallback(const Element& element, const std::string& name, const AttributeType& fallback);

#include <gimgui/util/getorfallback.inl>
}
