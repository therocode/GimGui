#include <gimgui/logic/attributepopulator.hpp>
#include <gimgui/logic/foreach.hpp>
#include <list>

namespace gim
{
    void AttributePopulator::populate(gim::Element& target) const
    {
        forEach(target, [this] (Element& element)
        {
            for(auto attributeCreator : mGlobalAttributeCreators)
            {
                attributeCreator.second(element);
            }

            for(const auto& tag : element.tags())
            {
                auto specificCreatorsIterator = mSpecificAttributeCreators.find(tag);

                if(specificCreatorsIterator != mSpecificAttributeCreators.end())
                {
                    for(auto attributeCreator : specificCreatorsIterator->second)
                    {
                        attributeCreator.second(element);
                    }
                }
            }

            return false;
        });
    }
}
