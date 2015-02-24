#pragma once
#include <gimgui/data/types.hpp>

namespace gim
{
    class Element
    {
        public:
            Element(const TagSet& tags);
            Element* append(Element&& child);
            ElementPtrList find(const TagSet& tags);
            ElementConstPtrList find(const TagSet& tags) const;
        private:
            ElementPtrList findHelper(const TagSet& tags) const;
            TagSet mTags;
            ElementList mChildren;
    };
}
