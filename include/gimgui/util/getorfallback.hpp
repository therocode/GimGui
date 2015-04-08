#pragma once
#include <gimgui/data/element.hpp>
#include <string>
#include <unordered_map>

namespace gim
{
    template <typename AttributeType>
    const AttributeType& getOrFallback(const Element& element, const std::string& name, const AttributeType& fallback);
    template <typename AttributeType>
    const AttributeType& getOrFallback(const std::unordered_map<std::string, Variant>& map, const std::string& name, const AttributeType& fallback);

#include <gimgui/util/getorfallback.inl>
}
