#pragma once
#include <gimgui/data/rectangle.hpp>
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
            void registerImageInfo(int32_t imageId, const Vec2& imageSize);
        private:
            std::unordered_map<int32_t, Vec2> mImageSizes;
    };

#include <gimgui/logic/renderdatagenerator.inl>
}
