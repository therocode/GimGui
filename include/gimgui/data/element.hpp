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
            Element(const TagSet& tags, const std::deque<std::pair<std::string, Variant>>& attributes = std::deque<std::pair<std::string, Variant>>(), std::deque<Element>&& children = std::deque<Element>());
            Element(Element&& other);
            Element& operator=(Element&& other);
            Element(const Element& other);
            Element& operator=(const Element& other);
            //tags
            const TagSet& tags() const;
            //hierarchy
            Element* parent();
            const Element* parent() const;
            ElementPtrList findParents(const TagSet& tags);
            ElementConstPtrList findParents(const TagSet& tags) const;
            Element& append(Element&& child);
            Element& prepend(Element&& child);
            Element& insert(size_t index, Element&& child);
            Element detachChild(size_t index);
            ElementList& children();
            const ElementList& children() const;
            ElementPtrList findChildren(const TagSet& tags);
            ElementConstPtrList findChildren(const TagSet& tags) const;
            ElementPtrList recursiveFindChildren(const TagSet& tags);
            ElementConstPtrList recursiveFindChildren(const TagSet& tags) const;
            //attributes
            template <typename AttributeType>
            void createAttribute(const std::string& name, const AttributeType& value);
            bool deleteAttribute(const std::string& name);
            bool hasAttribute(const std::string& name) const;
            template <typename AttributeType>
            bool hasAttribute(const std::string& name) const;
            template <typename AttributeType>
            AttributeType* findAttribute(const std::string& name);
            template <typename AttributeType>
            const AttributeType* findAttribute(const std::string& name) const;
            template <typename AttributeType>
            const AttributeType& getAttribute(const std::string& name) const;
            template <typename AttributeType>
            void setAttribute(const std::string& name, const AttributeType& value);
        private:
            ElementPtrList findChildrenHelper(const TagSet& tags) const;
            ElementPtrList recursiveFindChildrenHelper(const TagSet& tags) const;

            TagSet mTags;
            Element* mParent;
            ElementList mChildren;
            std::unordered_map<std::string, Variant> mAttributes;
    };

#include <gimgui/data/element.inl>
}
