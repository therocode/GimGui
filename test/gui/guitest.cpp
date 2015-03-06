#include <catch.hpp>

#include <gimgui/data/element.hpp>
#include <gimgui/gui/gui.hpp>
#include <gimgui/logic/event.hpp>
#include <gimgui/logic/allpropagator.hpp>

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

SCENARIO("Events can be sent to all elements of a tree using the all propagator", "[gui]")
{
    GIVEN("A gui with elements")
    {
        gim::Gui gui(
        {
            gim::Element({"root"}, {},
            {
                gim::Element({"container"}, {},
                {
                    gim::Element({"child"})
                }),
                gim::Element({"container"})
            })
        });

        WHEN("an event to create a name attribute is sent to the gui using the all propagator")
        {
            std::function<gim::Event(const std::string&)> createNameEventFactory = [] (const std::string& name)
            {
                return gim::Event([=] (gim::Element& element)
                {
                    element.createAttribute("name", name);
                });
            };

            gui.sendEvent<gim::AllPropagator>(createNameEventFactory("kalle"));

            THEN("all elements have such an attribute with the right value")
            {
                REQUIRE(gui.root().hasAttribute<std::string>("name"));
                CHECK(gui.root().getAttribute<std::string>("name") == "kalle");
                REQUIRE(gui.root().children()[0]->hasAttribute<std::string>("name"));
                CHECK(gui.root().children()[0]->getAttribute<std::string>("name") == "kalle");
                REQUIRE(gui.root().children()[1]->hasAttribute<std::string>("name"));
                CHECK(gui.root().children()[1]->getAttribute<std::string>("name") == "kalle");
                REQUIRE(gui.root().children()[0]->children()[0]->hasAttribute<std::string>("name"));
                CHECK(gui.root().children()[0]->children()[0]->getAttribute<std::string>("name") == "kalle");
            }
        }
    }
}

            
            //using ClickEvent = gim::Event<BorderPropagator>;
            //std::function<ClickEvent(int32_t, int32_t)> clickEventFactory = [] (int32_t x, int32_t y)
            //{
            //    //the event class takes a lambda in the constructor which defines what it will do on the target elements
            //    return ClickEvent([=] (gim::Element& element)
            //    {
            //        using CallbackList = std::vector<std::function<void(int32_t, int32_t)>>;
            //        if(element.hasAttribute<CallbackList>("on_click"))
            //        {
            //            for(auto callback : element.getAttribute<CallbackList>("on_click"))
            //                callback(x, y);
            //        }
            //    }, x, y);
            //};

            //gui.sendEvent(clickEventFactory(34, 12)); //now any element within the GUI which has a function stored as an attribute "on_click" will get the event and the function will be executed


            ///The clickEventFactory can be factored out to this:
            
            //std::function<ClickEvent(int32_t, int32_t)> clickEventFactory = [] (int32_t x, int32_t y)
            //{
            //    //the CallbackHandler is a variadic template which can be used to call simple callbacks on gui elements
            //    return ClickEvent(CallbackExecutor("on_click", x, y));
            //};
