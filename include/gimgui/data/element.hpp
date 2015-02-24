#pragma once
#include <gimgui/data/types.hpp>

namespace gim
{
    class Element
    {
        public:
            Element(const TagSet& tags);
            const TagSet& getTags() const;
            Element* append(Element&& child);
            Element* prepend(Element&& child);
            Element* insert(size_t index, Element&& child);
            ElementList& getChildren();
            const ElementList& getChildren() const;
            ElementPtrList find(const TagSet& tags);
            ElementConstPtrList find(const TagSet& tags) const;
            ElementPtrList recursiveFind(const TagSet& tags);
            ElementConstPtrList recursiveFind(const TagSet& tags) const;
        private:
            ElementPtrList findHelper(const TagSet& tags) const;
            ElementPtrList recursiveFindHelper(const TagSet& tags) const;
            TagSet mTags;
            ElementList mChildren;
    };
}
