#pragma once

template <typename AttributeType>
void AttributePopulator::addConfiguration(const std::string& name, const AttributeType& value)
{
    GIM_ASSERT(mAttributeCreators.count(name) == 0, "Adding configuration for attribute '" + name + "' when such a configuration already exists");

    mAttributeCreators.emplace(name, [=] (Element& element)
    {
        if(element.hasAttribute(name))
        {
            GIM_ASSERT(element.hasAttribute<AttributeType>(name), "Encountered already existing attribute '" + name + "' created with type '" + std::string(typeid(AttributeType).name()) + "' instead of '" + std::string(typeid(AttributeType).name()) + "' when populating element");

            element.setAttribute(name, value);
        }
        else
        {
            element.createAttribute(name, value);
        }
    });
}
