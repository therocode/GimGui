#include <catch.hpp>
#include <gimgui/data/rectangle.hpp>
#include <helpers/vec2.hpp>

SCENARIO("Rectangles can be constructed with a value", "[data]")
{
    GIVEN("a rectangle filled with data")
    {
        gim::Rectangle<Vec2> rectangle(Vec2({0, 0}), Vec2({10, 10}));

        WHEN("the data of the rectangle is accessed")
        {
            THEN("it is correct")
            {
                CHECK(rectangle.start == Vec2({0, 0}));
                CHECK(rectangle.size == Vec2({10, 10}));
            }
        }
    }
}
