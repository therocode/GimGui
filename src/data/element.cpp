#include <gimgui/data/element.hpp>
#include <gimgui/assert.hpp>
#include <algorithm>
#include <iostream>

namespace gim
{
    Element::Element():
        mParent(nullptr)
    {
    }

    Element::Element(const TagSet& tags, std::deque<Element>&& children) :
        mTags(tags),
        mParent(nullptr)
    {
        for(int32_t i = 0; i < children.size(); i++)
        {
            append(std::move(children[i]));
        }
    }

    Element::Element(Element&& other)
    {
        std::swap(mParent, other.mParent);
        std::swap(mTags, other.mTags);
        std::swap(mAttributes, other.mAttributes);
        std::swap(mChildren, other.mChildren);

        for(auto& child : mChildren)
        {
            child->mParent = this;
        }
    }

    Element& Element::operator=(Element&& other)
    {
        std::swap(mParent, other.mParent);
        std::swap(mTags, other.mTags);
        std::swap(mAttributes, other.mAttributes);
        std::swap(mChildren, other.mChildren);

        for(auto& child : mChildren)
        {
            child->mParent = this;
        }

        return *this;
    }

    Element::Element(const Element& other) :
        mParent(nullptr),
        mTags(other.mTags),
        mAttributes(other.mAttributes)
    {
        for(const auto& child : other.mChildren)
        {
            append(gim::Element(*child));
        }
    }

    Element& Element::operator=(const Element& other)
    {
        mParent = nullptr;
        mTags = other.mTags;
        mAttributes = other.mAttributes;

        for(const auto& child : other.mChildren)
        {
            append(gim::Element(*child));
        }

        return *this;
    }

    Element& Element::append(Element&& child)
    {
        mChildren.push_back(std::unique_ptr<Element>(new Element(std::move(child))));
        Element& newChild = *mChildren.back();
        newChild.mParent = this;
        return newChild;
    }

    Element& Element::prepend(Element&& child)
    {
        child.mParent = this;
        mChildren.push_front(std::unique_ptr<Element>(new Element(std::move(child))));
        Element& newChild = *mChildren.front();
        newChild.mParent = this;
        return newChild;
    }

    Element& Element::insert(size_t index, Element&& child)
    {
        GIM_ASSERT(index <= mChildren.size(), "Index out of bounds. Trying to insert element at " + std::to_string(index) + " but this element only has " + std::to_string(mChildren.size()) + " children");
        child.mParent = this;
        auto iterator = mChildren.insert(mChildren.begin() + index, std::unique_ptr<Element>(new Element(std::move(child))));
        Element& newChild = **iterator;
        newChild.mParent = this;
        return newChild;
    }

    Element Element::detachChild(size_t index)
    {
        GIM_ASSERT(index <= mChildren.size(), "Index out of bounds. Trying to detach child index " + std::to_string(index) + " but this element only has " + std::to_string(mChildren.size()) + " children");

        gim::Element detached = std::move(*mChildren[index]);
        detached.mParent = nullptr;
        mChildren.erase(mChildren.begin() + index);

        return detached;
    }

    ElementList& Element::children()
    {
        return mChildren;
    }

    const ElementList& Element::children() const
    {
        return mChildren;
    }
            
    const TagSet& Element::tags() const
    {
        return mTags;
    }

    Element* Element::parent()
    {
        return mParent;
    }

    const Element* Element::parent() const
    {
        return mParent;
    }

    ElementPtrList Element::findChildren(const TagSet& tags)
    {
        GIM_ASSERT(!tags.empty(), "No tags given");
        return findChildrenHelper(tags);
    }

    ElementConstPtrList Element::findChildren(const TagSet& tags) const
    {
        GIM_ASSERT(!tags.empty(), "No tags given");
        ElementPtrList elements = findChildrenHelper(tags);

        return ElementConstPtrList(elements.begin(), elements.end());
    }

    ElementPtrList Element::recursiveFindChildren(const TagSet& tags)
    {
        GIM_ASSERT(!tags.empty(), "No tags given");
        return recursiveFindChildrenHelper(tags);
    }

    ElementConstPtrList Element::recursiveFindChildren(const TagSet& tags) const
    {
        GIM_ASSERT(!tags.empty(), "No tags given");
        ElementPtrList elements = recursiveFindChildrenHelper(tags);

        return ElementConstPtrList(elements.begin(), elements.end());
    }

    ElementPtrList Element::findChildrenHelper(const TagSet& tags) const
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
    
    ElementPtrList Element::recursiveFindChildrenHelper(const TagSet& tags) const
    {
        ElementPtrList result;

        if(std::includes(mTags.begin(), mTags.end(), tags.begin(), tags.end()))
        {
            result.push_back(&*const_cast<Element*>(this));
        }

        for(auto& child : mChildren)
        {
            ElementPtrList childResults = child->recursiveFindChildrenHelper(tags);
            result.insert(result.end(), childResults.begin(), childResults.end());
        }

        return result;
    }
    
    bool Element::deleteAttribute(const std::string& name)
    {
        return mAttributes.erase(name) > 0;
    }

    bool Element::hasAttribute(const std::string& name)
    {
        return mAttributes.count(name) != 0;
    }
}
