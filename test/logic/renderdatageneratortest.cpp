#include <catch.hpp>
#include <gimgui/data/element.hpp>
#include <gimgui/logic/renderdatagenerator.hpp>
#include <helpers/vec2.hpp>
#include <helpers/color.hpp>
#include <helpers/closeenough.hpp>

void checkQuadPositions(const float* firstPosition, float xMin, float yMin, float xMax, float yMax)
{
    CHECK(closeEnough(*(firstPosition + 0) ,  xMin));//x
    CHECK(closeEnough(*(firstPosition + 1) ,  yMin));//y
    CHECK(closeEnough(*(firstPosition + 2) ,  0.0f));
    CHECK(closeEnough(*(firstPosition + 3) ,  xMin));//x
    CHECK(closeEnough(*(firstPosition + 4) ,  yMax));//y
    CHECK(closeEnough(*(firstPosition + 5) ,  0.0f));
    CHECK(closeEnough(*(firstPosition + 6) ,  xMax));//x
    CHECK(closeEnough(*(firstPosition + 7) ,  yMax));//y
    CHECK(closeEnough(*(firstPosition + 8) ,  0.0f));
    CHECK(closeEnough(*(firstPosition + 9) ,  xMax));//x
    CHECK(closeEnough(*(firstPosition + 10),  yMax));//y
    CHECK(closeEnough(*(firstPosition + 11),  0.0f));
    CHECK(closeEnough(*(firstPosition + 12),  xMax));//x
    CHECK(closeEnough(*(firstPosition + 13),  yMin));//y
    CHECK(closeEnough(*(firstPosition + 14),  0.0f));
    CHECK(closeEnough(*(firstPosition + 15),  xMin));//x
    CHECK(closeEnough(*(firstPosition + 16),  yMin));//y
    CHECK(closeEnough(*(firstPosition + 17),  0.0f));
}

void checkQuadColors(const float* firstColor, float r, float g, float b)
{
    CHECK(closeEnough(*(firstColor + 0) , r));//r
    CHECK(closeEnough(*(firstColor + 1) , g));//g
    CHECK(closeEnough(*(firstColor + 2) , b));//b
    CHECK(closeEnough(*(firstColor + 3) , r));//r
    CHECK(closeEnough(*(firstColor + 4) , g));//g
    CHECK(closeEnough(*(firstColor + 5) , b));//b
    CHECK(closeEnough(*(firstColor + 6) , r));//r
    CHECK(closeEnough(*(firstColor + 7) , g));//g
    CHECK(closeEnough(*(firstColor + 8) , b));//b
    CHECK(closeEnough(*(firstColor + 9) , r));//r
    CHECK(closeEnough(*(firstColor + 10), g));//g
    CHECK(closeEnough(*(firstColor + 11), b));//b
    CHECK(closeEnough(*(firstColor + 12), r));//r
    CHECK(closeEnough(*(firstColor + 13), g));//g
    CHECK(closeEnough(*(firstColor + 14), b));//b
    CHECK(closeEnough(*(firstColor + 15), r));//r
    CHECK(closeEnough(*(firstColor + 16), g));//g
    CHECK(closeEnough(*(firstColor + 17), b));//b
}

void checkQuadTexCoords(const float* firstTexCoord, float xMin, float yMin, float xMax, float yMax)
{
    CHECK(closeEnough(*(firstTexCoord + 0) ,  xMin));//u
    CHECK(closeEnough(*(firstTexCoord + 1) ,  yMin));//v
    CHECK(closeEnough(*(firstTexCoord + 2) ,  xMin));//u
    CHECK(closeEnough(*(firstTexCoord + 3) ,  yMax));//v
    CHECK(closeEnough(*(firstTexCoord + 4) ,  xMax));//u
    CHECK(closeEnough(*(firstTexCoord + 5) ,  yMax));//v
    CHECK(closeEnough(*(firstTexCoord + 6) ,  xMax));//u
    CHECK(closeEnough(*(firstTexCoord + 7) ,  yMax));//v
    CHECK(closeEnough(*(firstTexCoord + 8) ,  xMax));//u
    CHECK(closeEnough(*(firstTexCoord + 9) ,  yMin));//v
    CHECK(closeEnough(*(firstTexCoord + 10),  xMin));//u
    CHECK(closeEnough(*(firstTexCoord + 11),  yMin));//v
}

SCENARIO("RenderDataGenerator can be used to turn a gui tree into triangle buffers and such data for rendering", "[logic]")
{
    GIVEN("a gui tree with elements with the attributes width and height")
    {
        gim::Element root({"container"},
        {
            {"color",    Color(255, 0, 0)},
            {"position", Vec2({5, 5})},
            {"size",     Vec2({40, 30})},
            {"image_id", 1},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))}
        },
        {
            gim::Element({"child"},
            {
                {"color",    Color(0, 0, 255)},
                {"position", Vec2({10, 10})},
                {"size",     Vec2({10, 10})}
            })
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the tree")
        {
            gim::RenderDataGenerator<Vec2, Color> generator;
            generator.registerImageInfo(1, Vec2({64, 64}));

            std::vector<gim::RenderData> data = generator.generate(root);

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
                CHECK(data[0].element == &root);
                CHECK(data[1].element == root.children()[0].get());

                checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 45.0f, 35.0f);
                checkQuadPositions(&data[1].positions[0], 15.0f, 15.0f, 25.0f, 25.0f);

                checkQuadColors(&data[0].colors[0], 1.0f, 0.0f, 0.0f);
                checkQuadColors(&data[1].colors[0], 0.0f, 0.0f, 1.0f);

                checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f);
            }
        }
    }
}

//write test for tiles
