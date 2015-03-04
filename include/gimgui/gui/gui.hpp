#pragma once
#include <gimgui/data/element.hpp>

namespace gim
{
    class Gui
    {
        public:
            Gui(gim::Element&& element);       
            Element& root();
            const Element& root() const;
        private:
            gim::Element mRoot;
    };
}
