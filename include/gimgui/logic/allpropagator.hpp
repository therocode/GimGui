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
            size_t size() const;
        private:
            std::list<gim::Element*> mElements;
            std::list<gim::Element*>::iterator mCurrentElement;
    };

    class AllConstPropagator
    {
        public:
            AllConstPropagator(const gim::Element& first);
            const gim::Element* next();
            size_t size() const;
        private:
            std::list<const gim::Element*> mElements;
            std::list<const gim::Element*>::iterator mCurrentElement;
    };
}
