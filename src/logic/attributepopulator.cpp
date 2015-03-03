#include <gimgui/logic/attributepopulator.hpp>
#include <list>

namespace gim
{
    void AttributePopulator::populate(gim::Element& target) const
    {
        std::list<gim::Element*> elementList({&target});

        for(auto elementIterator = elementList.begin(); elementIterator != elementList.end(); elementIterator++)
        {
            for(auto attributeCreator : mGlobalAttributeCreators)
            {
                attributeCreator.second(**elementIterator);
            }

            for(const auto& tag : (*elementIterator)->tags())
            {
                auto specificCreatorsIterator = mSpecificAttributeCreators.find(tag);

                if(specificCreatorsIterator != mSpecificAttributeCreators.end())
                {
                    for(auto attributeCreator : specificCreatorsIterator->second)
                    {
                        attributeCreator.second(**elementIterator);
                    }
                }
            }

            for(auto& child : (*elementIterator)->children())
            {
                elementList.push_back(child.get());
            }
        }
    }
}
