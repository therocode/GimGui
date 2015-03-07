#include <catch.hpp>
#include <gimgui/data/element.hpp>
#include <helpers/vec2.hpp>
#include <helpers/color.hpp>

SCENARIO("RenderDataGenerator can be used to turn a gui tree into triangle buffers and such data for rendering", "[logic]")
{
    GIVEN("a gui tree with elements with the attributes width and height")
    {
        gim::Element root({"container"},
        {
            {"color",    Color({140, 35, 24})},
            {"position", Vec2({200, 150})},
            {"size",     Vec2({400, 300})}
        },
        {
            gim::Element({"child"},
            {
                {"color",    Color({94, 140, 106})},
                {"position", Vec2({20, 20})},
                {"size",     Vec2({50, 50})}
            })
        });
    }
}
