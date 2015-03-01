#include "../catch.hpp"

#include <gimgui/data/element.hpp>
#include <gimgui/gui/gui.hpp>

SCENARIO("Gui instances can be created and given a root element", "[gui]")
{
    GIVEN("A an element")
    {
        gim::Element rootElement({"root"});

        WHEN("a gui instance is created with the root element given to it")
        {
            //gim::Gui gui(rootElement);

        }
    }
}
