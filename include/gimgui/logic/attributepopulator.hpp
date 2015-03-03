#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <algorithm>
#include <gimgui/assert.hpp>
#include <gimgui/data/element.hpp>

namespace gim
{
    class Element;

    class AttributePopulator
    {
        public:
            template <typename AttributeType>
            void addConfiguration(const std::string& name, const AttributeType& value, const TagSet& tags = TagSet());
            void populate(gim::Element& target) const;
        private:
            using Creator = std::function<void(Element&)>;
            using NameCreatorPair = std::pair<std::string, Creator>;
            using NameCreatorPairList = std::vector<NameCreatorPair>;
            std::unordered_map<std::string, NameCreatorPairList> mSpecificAttributeCreators;
            NameCreatorPairList mGlobalAttributeCreators;
    };

#include <gimgui/logic/attributepopulator.inl>
}
