#include <catch.hpp>
#include <gimgui/data/rectangle.hpp>
#include <gimgui/logic/rectanglepacker.hpp>
#include <helpers/vec2.hpp>

const int32_t minSize = 4;
const int32_t maxSize = 10;

bool overlaps(const gim::internal::Rectangle& a, const gim::internal::Rectangle& b)
{
    return (a.start.x < b.start.x + b.size.x && b.start.x < a.start.x + a.size.x) &&
           (a.start.y < b.start.y + b.size.y && b.start.y < a.start.y + a.size.y);
}

SCENARIO("RectanglePacker can be used to pack rectangles on an ever increasing surface", "[util]")
{
    GIVEN("a RectanglePacker of a specific size")
    {
        gim::internal::RectanglePacker packer(16);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(minSize, maxSize);

        WHEN("more rectangles are added to the packer than it would fit")
        {
            std::vector<gim::internal::Rectangle> packedRectangles;

            for(int32_t i = 0; i < 1000; i++)
            {
                auto packed = packer.insert(gim::internal::IVec2({dis(gen), dis(gen)}));
                packedRectangles.push_back(packed);
            }

            THEN("they are still added validly")
            {
                bool noneOverlapped = true;
                int32_t zeroHeightCount = 0;

                for(int32_t i = 0; i < packedRectangles.size(); ++i)
                {
                    zeroHeightCount += packedRectangles[i].size.y == 0 ? 1 : 0;

                    for(int32_t j = i + 1; j < packedRectangles.size(); ++j)
                    {
                        noneOverlapped = noneOverlapped && !overlaps(packedRectangles[i], packedRectangles[j]);
                    }
                }

                INFO("packed " << packedRectangles.size() - zeroHeightCount << " rectangles");

                CHECK(noneOverlapped);
                CHECK(zeroHeightCount == 0);
            }
        }
    }
}
