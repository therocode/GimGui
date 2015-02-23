#include <gimgui/data/element.hpp>
#include <algorithm>

namespace gim
{
    Element::Element(const TagSet& tags) :
        mTags(tags)
    {
    }

    Element* Element::append(Element&& child)
    {
        mChildren.push_back(std::move(child));
        return &*mChildren.end();
    }

    ElementPtrList Element::find(const TagSet& tags)
    {
        ElementPtrList result;

        if(!tags.empty())
        {
            for(auto& child : mChildren)
            {
                if(std::includes(child.mTags.begin(), child.mTags.end(), tags.begin(), tags.end()))
                {
                    result.push_back(&child);
                }
            }
        }

        return result;
    }
}
