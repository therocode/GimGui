#pragma once

template <typename AttributeType>
void AttributePopulator::addConfiguration(const std::string& name, const AttributeType& value, const TagSet& tags)
{
    GIM_ASSERT(std::find_if(mGlobalAttributeCreators.begin(), mGlobalAttributeCreators.end(), [&] (const NameCreatorPair& e) {return e.first == name;}) == mGlobalAttributeCreators.end(), "Adding configuration for attribute '" + name + "' when such a configuration already exists");

    GIM_ASSERT(std::find_if(mSpecificAttributeCreators.begin(), mSpecificAttributeCreators.end(), [&] (const std::pair<std::string, NameCreatorPairList>& e)
    {
        return std::find_if(e.second.begin(), e.second.end(), [&] (const NameCreatorPair& e2)
        {
            return e2.first == name;
        }) != e.second.end();
    }) == mSpecificAttributeCreators.end(), "Adding configuration for attribute '" + name + "' when such a configuration already exists");

    if(tags.empty())
    {
        mGlobalAttributeCreators.push_back({name, [=] (Element& element)
        {
            if(element.hasAttribute(name))
            {
                GIM_ASSERT(element.hasAttribute<AttributeType>(name), "Encountered already existing attribute '" + name + "' created with a different type than '" + std::string(typeid(AttributeType).name()) + "' when populating element");

                element.setAttribute(name, value);
            }
            else
            {
                element.createAttribute(name, value);
            }
        }});
    }
    else
    {
        for(const auto& tag : tags)
        {
            mSpecificAttributeCreators[tag].push_back({name, [=] (Element& element)
            {
                if(element.hasAttribute(name))
                {
                    GIM_ASSERT(element.hasAttribute<AttributeType>(name), "Encountered already existing attribute '" + name + "' created with a different type than '" + std::string(typeid(AttributeType).name()) + "' when populating element");

                    element.setAttribute(name, value);
                }
                else
                {
                    element.createAttribute(name, value);
                }
            }});
        }
    }
}
