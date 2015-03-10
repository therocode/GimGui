#pragma once
#include <gimgui/data/element.hpp>
#include <gimgui/logic/event.hpp>

namespace gim
{
    class Gui
    {
        public:
            Gui(gim::Element&& element);       
            Element& root();
            const Element& root() const;
            template <typename Propagator>
            void sendEvent(const Event& event, Propagator& propagator);
        private:
            gim::Element mRoot;
    };

#include <gimgui/gui/gui.inl>
}
