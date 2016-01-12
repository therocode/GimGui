#include <catch.hpp>
#include <helpers/vector2.hpp>
#include <helpers/rectangle.hpp>

SCENARIO("Rectangles can be constructed with a value", "[data]")
{
    GIVEN("a rectangle filled with data")
    {
        Rectangle rectangle({{0, 0}, {10, 10}});

        WHEN("the data of the rectangle is accessed")
        {
            THEN("it is correct")
            {
                CHECK(rectangle.start == Vector2({0, 0}));
                CHECK(rectangle.size == Vector2({10, 10}));
            }
        }
    }
}
