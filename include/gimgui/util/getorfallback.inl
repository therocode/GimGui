#pragma once

template <typename AttributeType>
AttributeType getOrFallback(const Element& element, const std::string& name, const AttributeType& fallback)
{
    const AttributeType* attributePtr = element.findAttribute<AttributeType>(name);
    return attributePtr ? *attributePtr : fallback;
}

template <typename AttributeType>
AttributeType getOrFallback(const std::unordered_map<std::string, Variant>& map, const std::string& name, const AttributeType& fallback)
{
    const auto iterator = map.find(name);

    if(iterator != map.end())
    {
        const Variant& variant = iterator->second;

        if(variant.isOfType<AttributeType>())
        {
            return variant.get<AttributeType>();
        }
    }

    return fallback;
}
