#pragma once
#include <gimgui/data/renderdata.hpp>
#include <gimgui/logic/allpropagator.hpp>
#include <gimgui/logic/absolutemap.hpp>
#include <vector>

namespace gim
{
    template <typename Vec2, typename Color>
    class RenderDataGenerator
    {
        public:
            std::vector<RenderData> generate(const gim::Element& element);
    };

#include <gimgui/logic/renderdatagenerator.inl>
}
