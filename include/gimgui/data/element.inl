#pragma once

template <typename AttributeType>
void Element::createAttribute(const std::string& name)
{
}

template <typename AttributeType>
void Element::createAttribute(const std::string& name, const AttributeType& value)
{
}

template <typename AttributeType>
bool Element::hasAttribute(const std::string& name)
{
    return false;
}

template <typename AttributeType>
AttributeType* Element::findAttribute(const std::string& name)
{
    return nullptr;
}

template <typename AttributeType>
const AttributeType* Element::findAttribute(const std::string& name) const
{
    return nullptr;
}

template <typename AttributeType>
const AttributeType& Element::getAttribute(const std::string& name) const
{
    return *new AttributeType();
}

template <typename AttributeType>
void Element::setAttribute(const std::string& name, const AttributeType& value)
{
}
