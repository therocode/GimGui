#include <gimgui/logic/attributepopulator.hpp>
#include <list>

namespace gim
{
    void AttributePopulator::populate(gim::Element& target) const
    {
        std::list<gim::Element*> elementList({&target});

        for(auto elementIterator = elementList.begin(); elementIterator != elementList.end(); elementIterator++)
        {
            for(auto attributeCreator : mAttributeCreators)
            {
                attributeCreator.second(**elementIterator);
            }

            for(auto& child : (*elementIterator)->children())
            {
                elementList.push_back(child.get());
            }
        }
    }
}
