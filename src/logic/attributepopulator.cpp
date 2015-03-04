#include <gimgui/logic/attributepopulator.hpp>
#include <gimgui/logic/allpropagator.hpp>
#include <list>

namespace gim
{
    void AttributePopulator::populate(gim::Element& target) const
    {
        AllPropagator propagator(target);

        gim::Element* currentElement;

        while((currentElement = propagator.next()))
        {
            for(auto attributeCreator : mGlobalAttributeCreators)
            {
                attributeCreator.second(*currentElement);
            }

            for(const auto& tag : currentElement->tags())
            {
                auto specificCreatorsIterator = mSpecificAttributeCreators.find(tag);

                if(specificCreatorsIterator != mSpecificAttributeCreators.end())
                {
                    for(auto attributeCreator : specificCreatorsIterator->second)
                    {
                        attributeCreator.second(*currentElement);
                    }
                }
            }
        }
    }
}
