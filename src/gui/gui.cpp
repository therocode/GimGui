#include <gimgui/gui/gui.hpp>

namespace gim
{
    Gui::Gui(gim::Element&& element):
        mRoot(std::move(element))
    {
    }

    Element& Gui::root()
    {
        return mRoot;
    }

    const Element& Gui::root() const
    {
        return mRoot;
    }
}
