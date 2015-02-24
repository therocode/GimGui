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

    Element* Element::prepend(Element&& child)
    {
        mChildren.push_front(std::unique_ptr<Element>(new Element(std::move(child))));
        return &**mChildren.end();
    }

    Element* Element::insert(size_t index, Element&& child)
    {
        GIM_ASSERT(index < mChildren.size(), "Index out of bounds");
        mChildren.insert(mChildren.begin() + index, std::unique_ptr<Element>(new Element(std::move(child))));
        return &**mChildren.end();
    }

    ElementList& Element::getChildren()
    {
        return mChildren;
    }

    const ElementList& Element::getChildren() const
    {
        return mChildren;
    }
            
    const TagSet& Element::getTags() const
    {
        return mTags;
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
