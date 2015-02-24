#include <gimgui/data/element.hpp>
#include <gimgui/assert.hpp>
#include <algorithm>

namespace gim
{
    Element::Element(const TagSet& tags) :
        mTags(tags)
    {
    }

    Element* Element::append(Element&& child)
    {
        mChildren.push_back(std::unique_ptr<Element>(new Element(std::move(child))));
        return &**mChildren.end();
    }

    ElementPtrList Element::find(const TagSet& tags)
    {
        GIM_ASSERT(!tags.empty(), "No tags given");
        return findHelper(tags);
    }

    ElementConstPtrList Element::find(const TagSet& tags) const
    {
        GIM_ASSERT(!tags.empty(), "No tags given");
        ElementPtrList elements = findHelper(tags);

        return ElementConstPtrList(elements.begin(), elements.end());
    }

    ElementPtrList Element::findHelper(const TagSet& tags) const
    {
        ElementPtrList result;

        for(auto& child : mChildren)
        {
            if(std::includes(child->mTags.begin(), child->mTags.end(), tags.begin(), tags.end()))
            {
                result.push_back(&*child);
            }
        }

        return result;
    }
}
