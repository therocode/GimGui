#pragma once
#include <gimgui/data/element.hpp>

namespace gim
{
    template<typename AttributeType>
    class AbsoluteMap
    {
        public:
            AbsoluteMap(const std::string& attributeName);
            const AttributeType& getAbsoluteOf(const Element& element);
        private:
            std::string mAttributeName;
            std::unordered_map<const gim::Element*, AttributeType> mAbsoluteAttributes;
    };
#include <gimgui/logic/absolutemap.inl>
}
