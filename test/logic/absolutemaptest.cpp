#include <catch.hpp>
#include <gimgui/data/element.hpp>
#include <gimgui/logic/absolutemap.hpp>

SCENARIO("Absolute map can be used to keep track of absolute values across a tree", "[data]")
{
    GIVEN("An element tree with an attribute stored as relative values")
    {
        gim::Element root({}, {{"size", 10}},
        {
            gim::Element({}, {{"size", -5}},
            {
                gim::Element({}, {{"size", 2}})
            }),
            gim::Element({}, {{"size", 0}},
            {
                gim::Element({}, {{"size", 1}}),
                gim::Element({},  {{"size", -4}},
                {
                    gim::Element({}, {{"size", -1}})
                }),
                gim::Element({}, {{"size", 80}})
            })
        });

        WHEN("the tree is traversed")
        {
            THEN("an absolute map can be used to deduce the absolute value of the given attribute")
            {
                gim::AbsoluteMap<int32_t>map("size");

                CHECK(map.getAbsoluteOf(root) == 10);
                CHECK(map.getAbsoluteOf(*root.children()[0]) == 5);
                CHECK(map.getAbsoluteOf(*root.children()[1]) == 10);
                CHECK(map.getAbsoluteOf(*root.children()[0]->children()[0]) == 7);
                CHECK(map.getAbsoluteOf(*root.children()[1]->children()[0]) == 11);
                CHECK(map.getAbsoluteOf(*root.children()[1]->children()[1]) == 6);
                CHECK(map.getAbsoluteOf(*root.children()[1]->children()[2]) == 90);
                CHECK(map.getAbsoluteOf(*root.children()[1]->children()[1]->children()[0]) == 5);
            }
        }
    }
}
