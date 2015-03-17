#pragma once

template <typename AttributeType>
AttributeType getOrFallback(const Element& element, const std::string& name, const AttributeType& fallback)
{
    const AttributeType* attributePtr = element.findAttribute<AttributeType>(name);
    return attributePtr ? *attributePtr : fallback;
}
