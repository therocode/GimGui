#pragma once
#include <list>
#include <cstddef>
#include <gimgui/logic/absolutemap.hpp>
#include <gimgui/data/element.hpp>
#include <deque>
#include <algorithm>

namespace gim
{
    class Element;

    template <typename Vec2>
    class BoundaryPropagator
    {
        public:
            BoundaryPropagator(Element& first, const std::deque<Vec2>& positions);
            Element* next();
            size_t size() const;
            void reverse();
        private:
            bool anyPointWithinElement(const std::deque<Vec2>& points, const Element& element);
            std::list<Element*> mElements;
            std::list<Element*>::iterator mCurrentElement;
            AbsoluteMap<Vec2> mAbsoluteMap;
    };
#include <gimgui/logic/boundarypropagator.inl>
}
