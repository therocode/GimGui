#pragma once
#include <list>
#include <cstddef>
#include <gimgui/logic/absolutemap.hpp>
#include <gimgui/data/element.hpp>

namespace gim
{
    class Element;

    template <typename Vec2>
    class BoundaryPropagator
    {
        public:
            BoundaryPropagator(Element& first, const Vec2& position);
            Element* next();
            size_t size() const;
        private:
            bool pointWithinElement(const Vec2& point, const Element& element);
            std::list<Element*> mElements;
            std::list<Element*>::iterator mCurrentElement;
            AbsoluteMap<Vec2> mAbsoluteMap;
    };
#include <gimgui/logic/boundarypropagator.inl>
}
