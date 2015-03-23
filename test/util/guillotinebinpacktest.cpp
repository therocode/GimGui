#include <catch.hpp>
#include <random>
#include <gimgui/util/guillotinebinpack.hpp>

const int32_t minSize = 4;
const int32_t maxSize = 10;

bool overlaps(const gim::GuillotineBinPack::Rect& a, const gim::GuillotineBinPack::Rect& b)
{
    return (a.x < b.x + b.width && b.x < a.x + a.width) &&
           (a.y < b.y + b.height && b.y < a.y + a.height);
}

SCENARIO("GuillotineBinPack can be used to pack rectangles in a non-intersecting way", "[util]")
{
    GIVEN("a GuillotineBinPack of any size, and a random generator")
    {
        gim::GuillotineBinPack packer(128, 128);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(minSize, maxSize);

        WHEN("the packer is filled until it is full with random sized rectangles")
        {
            std::vector<gim::GuillotineBinPack::Rect> packedRectangles;

            bool full = false;
            while(!full)
            {
                gim::GuillotineBinPack::Rect packed = packer.Insert(dis(gen), dis(gen), true, gim::GuillotineBinPack::RectBestAreaFit, gim::GuillotineBinPack::SplitShorterLeftoverAxis);
                if(packed.width == 0)
                    full = true;
                else
                    packedRectangles.push_back(packed);
            }

            THEN("the packed rectangles do not overlap")
            {
                bool noneOverlapped = true;

                for(int32_t i = 0; i < packedRectangles.size(); ++i)
                {
                    for(int32_t j = i + 1; j < packedRectangles.size(); ++j)
                    {
                        noneOverlapped = noneOverlapped && !overlaps(packedRectangles[i], packedRectangles[j]);
                    }
                }

                CHECK(noneOverlapped);
            }
        }
    }
}
