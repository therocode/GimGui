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
            gim::Gui gui(std::move(rootElement));
            const gim::Gui& constGui = gui;
        
            THEN("the element is accessible through the root function")
            {
                CHECK(gui.root().tags().count("root") == 1);
                CHECK(constGui.root().tags().count("root") == 1);
            }

            
        }
    }
}

