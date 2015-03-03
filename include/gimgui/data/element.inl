#pragma once

template <typename AttributeType>
void Element::createAttribute(const std::string& name, const AttributeType& value)
{
    GIM_ASSERT(mAttributes.count(name) == 0, "Trying to create attribute '" + name + "' when such an attribute is already present");

    mAttributes[name] = Variant(AttributeType(value));
}

template <typename AttributeType>
bool Element::hasAttribute(const std::string& name) const
{
    auto variantIterator = mAttributes.find(name);
    if(variantIterator != mAttributes.end())
    {
        return variantIterator->second.isOfType<AttributeType>();
    }
    else
    {
        return false;
    }
}

template <typename AttributeType>
AttributeType* Element::findAttribute(const std::string& name)
{
    const Element* constThis = this;
    const AttributeType* constResult = constThis->findAttribute<AttributeType>(name);

    if(constResult != nullptr)
    {
        return const_cast<AttributeType*>(constResult);
    }
    else
    {
        return nullptr;
    }
}

template <typename AttributeType>
const AttributeType* Element::findAttribute(const std::string& name) const
{
    auto variantIterator = mAttributes.find(name);
    if(variantIterator != mAttributes.end())
    {
        const Variant& variant = variantIterator->second;
        if(variant.isOfType<AttributeType>())
        {
            return &variant.get<AttributeType>();
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        return nullptr;
    }
}

template <typename AttributeType>
const AttributeType& Element::getAttribute(const std::string& name) const
{
    GIM_ASSERT(mAttributes.count(name) != 0, "Trying to get attribute '" + name + "' when such an attribute does not exist");

    const Variant& variant = mAttributes.at(name);

    return variant.get<AttributeType>();
}

template <typename AttributeType>
void Element::setAttribute(const std::string& name, const AttributeType& value)
{
    GIM_ASSERT(mAttributes.count(name) != 0, "Trying to set attribute '" + name + "' when such an attribute does not exist");

    Variant& variant = mAttributes.at(name);

    variant.set(value);
}
