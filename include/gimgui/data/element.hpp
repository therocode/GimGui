#pragma once
#include <gimgui/data/types.hpp>
#include <gimgui/data/variant.hpp>
#include <unordered_map>

namespace gim
{
    class Element
    {
        public:
            //creation
            Element();
            Element(const TagSet& tags);
            //tags
            const TagSet& tags() const;
            //hierarchy
            Element* parent();
            const Element* parent() const;
            Element& append(Element&& child);
            Element& prepend(Element&& child);
            Element& insert(size_t index, Element&& child);
            ElementList& children();
            const ElementList& children() const;
            ElementPtrList find(const TagSet& tags);
            ElementConstPtrList find(const TagSet& tags) const;
            ElementPtrList recursiveFind(const TagSet& tags);
            ElementConstPtrList recursiveFind(const TagSet& tags) const;
            //attributes
            template <typename AttributeType>
            void createAttribute(const std::string& name);
            template <typename AttributeType>
            void createAttribute(const std::string& name, const AttributeType& value);
            bool deleteAttribute(const std::string& name);
            bool hasAttribute(const std::string& name);
            template <typename AttributeType>
            bool hasAttribute(const std::string& name);
            template <typename AttributeType>
            AttributeType* findAttribute(const std::string& name);
            template <typename AttributeType>
            const AttributeType* findAttribute(const std::string& name) const;
            template <typename AttributeType>
            const AttributeType& getAttribute(const std::string& name) const;
            template <typename AttributeType>
            void setAttribute(const std::string& name, const AttributeType& value);
        private:
            ElementPtrList findHelper(const TagSet& tags) const;
            ElementPtrList recursiveFindHelper(const TagSet& tags) const;
            TagSet mTags;
            Element* mParent;
            ElementList mChildren;
            std::unordered_map<std::string, Variant> mAttributes;
    };

#include <gimgui/data/element.inl>
}
