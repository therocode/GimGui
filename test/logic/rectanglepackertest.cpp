#include <catch.hpp>
#include <gimgui/data/rectangle.hpp>
#include <gimgui/logic/rectanglepacker.hpp>
#include <helpers/vec2.hpp>

const int32_t minSize = 4;
const int32_t maxSize = 10;

using Rect = gim::Rectangle<Vec2>;

bool overlaps(const Rect& a, const Rect& b)
{
    return (a.start.x < b.start.x + b.size.x && b.start.x < a.start.x + a.size.x) &&
           (a.start.y < b.start.y + b.size.y && b.start.y < a.start.y + a.size.y);
}

SCENARIO("RectanglePacker can be used to pack rectangles on an ever increasing surface", "[util]")
{
    GIVEN("a RectanglePacker of a specific size")
    {
        gim::RectanglePacker<Vec2> packer(Vec2({64, 64}));

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(minSize, maxSize);

        WHEN("more rectangles are added to the packer than it would fit")
        {
            std::vector<Rect> packedRectangles;

            Rect packed = packer.insert(Vec2({dis(gen), dis(gen)}));
            packedRectangles.push_back(packed);

            THEN("they are still added validly")
            {
            }
        }
    }
}
