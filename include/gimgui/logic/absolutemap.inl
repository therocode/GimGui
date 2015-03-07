#pragma once
template<typename AttributeType>
AbsoluteMap<AttributeType>::AbsoluteMap(const std::string& attributeName):
    mAttributeName(attributeName)
{
}

template<typename AttributeType>
const AttributeType& AbsoluteMap<AttributeType>::getAbsoluteOf(const Element& element)
{
    auto absoluteAttributeIterator = mAbsoluteAttributes.find(&element);

    if(absoluteAttributeIterator != mAbsoluteAttributes.end())
    {
        return absoluteAttributeIterator->second;
    }
    else
    {
        const gim::Element* parent = element.parent();

        if(parent != nullptr)
        {
            AttributeType parentAbsoluteValue = getAbsoluteOf(*parent);
            AttributeType absoluteValue = parentAbsoluteValue + element.getAttribute<AttributeType>(mAttributeName);
            return mAbsoluteAttributes.emplace(&element, absoluteValue).first->second;
        }
        else
        {
            AttributeType absoluteValue = element.getAttribute<AttributeType>(mAttributeName);
            return mAbsoluteAttributes.emplace(&element, absoluteValue).first->second;
        }
    }

}
