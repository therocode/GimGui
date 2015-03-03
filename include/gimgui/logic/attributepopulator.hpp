#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <gimgui/assert.hpp>
#include <gimgui/data/element.hpp>

namespace gim
{
    class Element;

    class AttributePopulator
    {
        public:
            template <typename AttributeType>
            void addConfiguration(const std::string& name, const AttributeType& value);
            void populate(gim::Element& target) const;
        private:
            std::unordered_map<std::string, std::function<void(Element&)>> mAttributeCreators;
    };

#include <gimgui/logic/attributepopulator.inl>
}
