#include <catch.hpp>
#include <gimgui/data/element.hpp>
#include <gimgui/logic/renderdatagenerator.hpp>
#include <helpers/vec2.hpp>
#include <helpers/color.hpp>
#include <helpers/closeenough.hpp>

SCENARIO("RenderDataGenerator can be used to turn a gui tree into triangle buffers and such data for rendering", "[logic]")
{
    GIVEN("a gui tree with elements with the attributes width and height")
    {
        gim::Element root({"container"},
        {
            {"color",    Color({255, 0, 0})},
            {"position", Vec2({5, 5})},
            {"size",     Vec2({40, 30})},
            {"image_id", 1},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))}
        },
        {
            gim::Element({"child"},
            {
                {"color",    Color({0, 0, 255})},
                {"position", Vec2({10, 10})},
                {"size",     Vec2({10, 10})}
            })
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the tree")
        {
            gim::RenderDataGenerator<Vec2, Color> generator;
            generator.registerImageInfo(1, Vec2({64, 64}));

            std::vector<RenderData> data = generator.generate(root);

            THEN("the data is correct")
            {
                REQUIRE(data.size() == 2);
                REQUIRE(data[0].positions.size() == 18);
                REQUIRE(data[0].colors.size() == 18);
                REQUIRE(data[0].texCoords.size() == 12);
                REQUIRE(data[1].positions.size() == 18);
                REQUIRE(data[1].colors.size() == 18);
                REQUIRE(data[1].texCoords.size() == 0);

                CHECK(data[0].imageId == 1);

                CHECK(closeEnough(data[0].positions[0] ,  5.0f));//x
                CHECK(closeEnough(data[0].positions[1] ,  5.0f));//y
                CHECK(closeEnough(data[0].positions[2] ,  0.0f));
                CHECK(closeEnough(data[0].positions[3] ,  5.0f));//x
                CHECK(closeEnough(data[0].positions[4] , 35.0f));//y
                CHECK(closeEnough(data[0].positions[5] ,  0.0f));
                CHECK(closeEnough(data[0].positions[6] , 45.0f));//x
                CHECK(closeEnough(data[0].positions[7] , 35.0f));//y
                CHECK(closeEnough(data[0].positions[8] ,  0.0f));
                CHECK(closeEnough(data[0].positions[9] , 45.0f));//x
                CHECK(closeEnough(data[0].positions[10], 35.0f));//y
                CHECK(closeEnough(data[0].positions[11],  0.0f));
                CHECK(closeEnough(data[0].positions[12], 45.0f));//x
                CHECK(closeEnough(data[0].positions[13],  5.0f));//y
                CHECK(closeEnough(data[0].positions[14],  0.0f));
                CHECK(closeEnough(data[0].positions[15],  5.0f));//x
                CHECK(closeEnough(data[0].positions[16],  5.0f));//y
                CHECK(closeEnough(data[0].positions[17],  0.0f));

                CHECK(closeEnough(data[1].positions[0] , 15.0f));//x
                CHECK(closeEnough(data[1].positions[1] , 15.0f));//y
                CHECK(closeEnough(data[1].positions[2] ,  0.0f));
                CHECK(closeEnough(data[1].positions[3] , 15.0f));//x
                CHECK(closeEnough(data[1].positions[4] , 25.0f));//y
                CHECK(closeEnough(data[1].positions[5] ,  0.0f));
                CHECK(closeEnough(data[1].positions[6] , 25.0f));//x
                CHECK(closeEnough(data[1].positions[7] , 25.0f));//y
                CHECK(closeEnough(data[1].positions[8] ,  0.0f));
                CHECK(closeEnough(data[1].positions[9] , 25.0f));//x
                CHECK(closeEnough(data[1].positions[10], 25.0f));//y
                CHECK(closeEnough(data[1].positions[11],  0.0f));
                CHECK(closeEnough(data[1].positions[12], 25.0f));//x
                CHECK(closeEnough(data[1].positions[13], 15.0f));//y
                CHECK(closeEnough(data[1].positions[14],  0.0f));
                CHECK(closeEnough(data[1].positions[15], 15.0f));//x
                CHECK(closeEnough(data[1].positions[16], 15.0f));//y
                CHECK(closeEnough(data[1].positions[17],  0.0f));

                CHECK(closeEnough(data[0].colors[0] , 1.0f));//r
                CHECK(closeEnough(data[0].colors[1] , 0.0f));//g
                CHECK(closeEnough(data[0].colors[2] , 0.0f));//b
                CHECK(closeEnough(data[0].colors[3] , 1.0f));//r
                CHECK(closeEnough(data[0].colors[4] , 0.0f));//g
                CHECK(closeEnough(data[0].colors[5] , 0.0f));//b
                CHECK(closeEnough(data[0].colors[6] , 1.0f));//r
                CHECK(closeEnough(data[0].colors[7] , 0.0f));//g
                CHECK(closeEnough(data[0].colors[8] , 0.0f));//b
                CHECK(closeEnough(data[0].colors[9] , 1.0f));//r
                CHECK(closeEnough(data[0].colors[10], 0.0f));//g
                CHECK(closeEnough(data[0].colors[11], 0.0f));//b
                CHECK(closeEnough(data[0].colors[12], 1.0f));//r
                CHECK(closeEnough(data[0].colors[13], 0.0f));//g
                CHECK(closeEnough(data[0].colors[14], 0.0f));//b
                CHECK(closeEnough(data[0].colors[15], 1.0f));//r
                CHECK(closeEnough(data[0].colors[16], 0.0f));//g
                CHECK(closeEnough(data[0].colors[17], 0.0f));//b

                CHECK(closeEnough(data[1].colors[0] , 0.0f));//r
                CHECK(closeEnough(data[1].colors[1] , 0.0f));//g
                CHECK(closeEnough(data[1].colors[2] , 1.0f));//b
                CHECK(closeEnough(data[1].colors[3] , 0.0f));//r
                CHECK(closeEnough(data[1].colors[4] , 0.0f));//g
                CHECK(closeEnough(data[1].colors[5] , 1.0f));//b
                CHECK(closeEnough(data[1].colors[6] , 0.0f));//r
                CHECK(closeEnough(data[1].colors[7] , 0.0f));//g
                CHECK(closeEnough(data[1].colors[8] , 1.0f));//b
                CHECK(closeEnough(data[1].colors[9] , 0.0f));//r
                CHECK(closeEnough(data[1].colors[10], 0.0f));//g
                CHECK(closeEnough(data[1].colors[11], 1.0f));//b
                CHECK(closeEnough(data[1].colors[12], 0.0f));//r
                CHECK(closeEnough(data[1].colors[13], 0.0f));//g
                CHECK(closeEnough(data[1].colors[14], 1.0f));//b
                CHECK(closeEnough(data[1].colors[15], 0.0f));//r
                CHECK(closeEnough(data[1].colors[16], 0.0f));//g
                CHECK(closeEnough(data[1].colors[17], 1.0f));//b

                CHECK(closeEnough(data[0].texCoords[0] , 0.0f));//u
                CHECK(closeEnough(data[0].texCoords[1] , 0.0f));//v
                CHECK(closeEnough(data[0].texCoords[2] , 0.0f));//u
                CHECK(closeEnough(data[0].texCoords[3] , 0.5f));//v
                CHECK(closeEnough(data[0].texCoords[4] , 0.5f));//u
                CHECK(closeEnough(data[0].texCoords[5] , 0.5f));//v
                CHECK(closeEnough(data[0].texCoords[6] , 0.5f));//u
                CHECK(closeEnough(data[0].texCoords[7] , 0.5f));//v
                CHECK(closeEnough(data[0].texCoords[8] , 0.5f));//u
                CHECK(closeEnough(data[0].texCoords[9] , 0.0f));//v
                CHECK(closeEnough(data[0].texCoords[10], 0.0f));//u
                CHECK(closeEnough(data[0].texCoords[11], 0.0f));//v
            }
        }
    }
}
