#pragma once
#include <gimgui/data/types.hpp>

namespace gim
{
    class Element
    {
        public:
            //creation
            Element();
            Element(const TagSet& tags);
            //tags
            const TagSet& getTags() const;
            //children
            Element* append(Element&& child);
            Element* prepend(Element&& child);
            Element* insert(size_t index, Element&& child);
            ElementList& getChildren();
            const ElementList& getChildren() const;
            ElementPtrList find(const TagSet& tags);
            ElementConstPtrList find(const TagSet& tags) const;
            ElementPtrList recursiveFind(const TagSet& tags);
            ElementConstPtrList recursiveFind(const TagSet& tags) const;
            //attributes
            template <typename AttributeType>
            void createAttribute(const std::string& name);
            template <typename AttributeType>
            void createAttribute(const std::string& name, const AttributeType& value);
            bool hasAttribute(const std::string& name);
            template <typename AttributeType>
            bool hasAttribute(const std::string& name);
            template <typename AttributeType>
            AttributeType* findAttribute(const std::string& name);
            template <typename AttributeType>
            const AttributeType* findAttribute(const std::string& name) const;
            template <typename AttributeType>
            void setAttribute(const std::string& name, const AttributeType& value);
            template <typename AttributeType>
            const AttributeType& getAttribute(const std::string& name) const;
        private:
            ElementPtrList findHelper(const TagSet& tags) const;
            ElementPtrList recursiveFindHelper(const TagSet& tags) const;
            TagSet mTags;
            ElementList mChildren;
    };
}
