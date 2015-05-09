#pragma once
#include <gimgui/data/element.hpp>
#include <string>
#include <unordered_map>

namespace gim
{
    template <typename AttributeType>
    AttributeType getOrFallback(const Element& element, const std::string& name, AttributeType fallback);
    template <typename AttributeType>
    AttributeType getOrFallback(const std::unordered_map<std::string, Variant>& map, const std::string& name, AttributeType fallback);

#include <gimgui/util/getorfallback.inl>
}
