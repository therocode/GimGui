#pragma once
#include <list>
#include <cstddef>

namespace gim
{
    class Element;

    class AllPropagator
    {
        public:
            AllPropagator(gim::Element& first);
            gim::Element* next();
        private:
            std::list<gim::Element*> mElements;
            std::list<gim::Element*>::iterator mCurrentElement;
    };
}
