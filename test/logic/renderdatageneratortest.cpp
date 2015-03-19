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
                checkQuadColors(&data[1].colors[0], 1.0f, 1.0f, 1.0f);

                checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f);
            }
        }
    }

    //tiles
    GIVEN("an element set to not be tiled")
    {
        gim::Element element({"non-tiled"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({40, 30})},
            {"stretch_mode", gim::StretchMode::STRETCHED},
            {"image_id", 1},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            gim::RenderDataGenerator<Vec2, Color> generator;
            generator.registerImageInfo(1, Vec2({64, 64}));

            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 45.0f, 35.0f);
                checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f);
            }
        }
    }

    GIVEN("an element bigger than one tile set to be fully tiled")
    {
        gim::Element element({"tiled"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({70, 37})},
            {"stretch_mode", gim::StretchMode::TILED},
            {"image_id", 1},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            gim::RenderDataGenerator<Vec2, Color> generator;
            generator.registerImageInfo(1, Vec2({64, 64}));

            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 37.0f, 37.0f);
                checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f);

                checkQuadPositions(&data[0].positions[18], 37.0f, 5.0f, 69.0f, 37.0f);
                checkQuadColors(&data[0].colors[18], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[12], 0.0f, 0.0f, 0.5f, 0.5f);

                checkQuadPositions(&data[0].positions[36], 69.0f, 5.0f, 75.0f, 37.0f);
                checkQuadColors(&data[0].colors[36], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[24], 0.0f, 0.0f, 0.1875f, 0.5f);


                checkQuadPositions(&data[0].positions[54], 5.0f, 37.0f, 37.0f, 42.0f);
                checkQuadColors(&data[0].colors[54], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[36], 0.0f, 0.0f, 0.5f, 0.15625f);

                checkQuadPositions(&data[0].positions[72], 37.0f, 37.0f, 69.0f, 42.0f);
                checkQuadColors(&data[0].colors[72], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[48], 0.0f, 0.0f, 0.5f, 0.15625f);

                checkQuadPositions(&data[0].positions[90], 69.0f, 37.0f, 75.0f, 42.0f);
                checkQuadColors(&data[0].colors[90], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[60], 0.0f, 0.0f, 0.1875f, 0.15625f);
            }
        }
    }

    GIVEN("an element smaller than one tile set to be fully tiled")
    {
        gim::Element element({"tiled"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({20, 10})},
            {"stretch_mode", gim::StretchMode::TILED},
            {"image_id", 1},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            gim::RenderDataGenerator<Vec2, Color> generator;
            generator.registerImageInfo(1, Vec2({64, 64}));

            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 25.0f, 15.0f);
                checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.625f, 0.3125f);
            }
        }
    }

    GIVEN("an element bigger than one tile set to be vertically tiled")
    {
        gim::Element element({"tiled"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({70, 37})},
            {"stretch_mode", gim::StretchMode::V_TILED},
            {"image_id", 1},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            gim::RenderDataGenerator<Vec2, Color> generator;
            generator.registerImageInfo(1, Vec2({64, 64}));

            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 75.0f, 37.0f);
                checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f);

                checkQuadPositions(&data[0].positions[18], 5.0f, 37.0f, 75.0f, 42.0f);
                checkQuadColors(&data[0].colors[18], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[12], 0.0f, 0.0f, 0.5f, 0.15625f);
            }
        }
    }

    GIVEN("an element smaller than one tile set to be vertically tiled")
    {
        gim::Element element({"tiled"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({20, 10})},
            {"stretch_mode", gim::StretchMode::V_TILED},
            {"image_id", 1},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            gim::RenderDataGenerator<Vec2, Color> generator;
            generator.registerImageInfo(1, Vec2({64, 64}));

            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 25.0f, 15.0f);
                checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.3125f);
            }
        }
    }

    GIVEN("an element bigger than one tile set to be horizontally tiled")
    {
        gim::Element element({"tiled"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({70, 37})},
            {"stretch_mode", gim::StretchMode::H_TILED},
            {"image_id", 1},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            gim::RenderDataGenerator<Vec2, Color> generator;
            generator.registerImageInfo(1, Vec2({64, 64}));

            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 37.0f, 42.0f);
                checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f);

                checkQuadPositions(&data[0].positions[18], 37.0f, 5.0f, 69.0f, 42.0f);
                checkQuadColors(&data[0].colors[18], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[12], 0.0f, 0.0f, 0.5f, 0.5f);

                checkQuadPositions(&data[0].positions[36], 69.0f, 5.0f, 75.0f, 42.0f);
                checkQuadColors(&data[0].colors[36], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[24], 0.0f, 0.0f, 0.1875f, 0.5f);
            }
        }
    }

    GIVEN("an element smaller than one tile set to be horizontally tiled")
    {
        gim::Element element({"tiled"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({20, 10})},
            {"stretch_mode", gim::StretchMode::H_TILED},
            {"image_id", 1},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            gim::RenderDataGenerator<Vec2, Color> generator;
            generator.registerImageInfo(1, Vec2({64, 64}));

            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 25.0f, 15.0f);
                checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.625f, 0.5f);
            }
        }
    }

    //borders
    GIVEN("an element set to have no border")
    {
        gim::Element element({"no border"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({32, 32})},
            {"image_id", 1},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))},
            {"border_mode", gim::BorderMode::NONE}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            gim::RenderDataGenerator<Vec2, Color> generator;
            generator.registerImageInfo(1, Vec2({64, 64}));

            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 37.0f, 37.0f);
                checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f);
            }
        }
    }

    GIVEN("an element set to have left-right border")
    {
        gim::Element element({"left-right"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({32, 32})},
            {"image_id", 1},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))},
            {"border_mode", gim::BorderMode::LEFT_RIGHT},
            {"border_coords_l", gim::Rectangle<Vec2>(Vec2({32,0}), Vec2({8,32}))},
            {"border_coords_r", gim::Rectangle<Vec2>(Vec2({40,0}), Vec2({8,32}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            gim::RenderDataGenerator<Vec2, Color> generator;
            generator.registerImageInfo(1, Vec2({64, 64}));

            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 37.0f, 37.0f);
                checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f);

                checkQuadPositions(&data[0].positions[18], -3.0f, 5.0f, 5.0f, 37.0f);
                checkQuadColors(&data[0].colors[18], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[12], 0.5f, 0.0f, 0.625f, 0.5f);

                checkQuadPositions(&data[0].positions[36], 37.0f, 5.0f, 45.0f, 37.0f);
                checkQuadColors(&data[0].colors[36], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[24], 0.625f, 0.0f, 0.75f, 0.5f);
            }
        }
    }

    GIVEN("an element set to have top-bottom border")
    {
        gim::Element element({"top-bottom"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({32, 32})},
            {"image_id", 1},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))},
            {"border_mode", gim::BorderMode::TOP_BOTTOM},
            {"border_coords_t", gim::Rectangle<Vec2>(Vec2({0,32}), Vec2({32,8}))},
            {"border_coords_b", gim::Rectangle<Vec2>(Vec2({0,40}), Vec2({32,8}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            gim::RenderDataGenerator<Vec2, Color> generator;
            generator.registerImageInfo(1, Vec2({64, 64}));

            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 37.0f, 37.0f);
                checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f);

                checkQuadPositions(&data[0].positions[18], 5.0f, -3.0f, 37.0f, 5.0f);
                checkQuadColors(&data[0].colors[18], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[12], 0.0f, 0.5f, 0.5f, 0.625f);

                checkQuadPositions(&data[0].positions[36], 5.0f, 37.0f, 37.0f, 45.0f);
                checkQuadColors(&data[0].colors[36], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[24], 0.0f, 0.625f, 0.5f, 0.75f);
            }
        }
    }

    GIVEN("an element set to have full border")
    {
        gim::Element element({"full"},
        {
            {"position", Vec2({0, 0})},
            {"size",     Vec2({48, 48})},
            {"image_id", 1},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({8, 8}), Vec2({48, 48}))},
            {"border_mode", gim::BorderMode::FULL},
            {"border_coords_tl", gim::Rectangle<Vec2>(Vec2({0 ,0 }), Vec2({8 ,8 }))},
            {"border_coords_t",  gim::Rectangle<Vec2>(Vec2({8 ,0 }), Vec2({48,8 }))},
            {"border_coords_tr", gim::Rectangle<Vec2>(Vec2({56,0 }), Vec2({8 ,8 }))},
            {"border_coords_r",  gim::Rectangle<Vec2>(Vec2({0 ,8 }), Vec2({8 ,48}))},
            {"border_coords_br", gim::Rectangle<Vec2>(Vec2({56,8 }), Vec2({8 ,8 }))},
            {"border_coords_b",  gim::Rectangle<Vec2>(Vec2({0 ,56}), Vec2({48,8 }))},
            {"border_coords_bl", gim::Rectangle<Vec2>(Vec2({8 ,56}), Vec2({8 ,8 }))},
            {"border_coords_l",  gim::Rectangle<Vec2>(Vec2({56,56}), Vec2({8 ,48}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            gim::RenderDataGenerator<Vec2, Color> generator;
            generator.registerImageInfo(1, Vec2({64, 64}));

            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                //main quad
                checkQuadPositions(&data[0].positions[0], 0.0f, 0.0f, 48.0f, 48.0f);
                checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[0], 0.125f, 0.125f, 0.875f, 0.875f);

                //tl
                checkQuadPositions(&data[0].positions[18], -8.0f, -8.0f, 0.0f, 0.0f);
                checkQuadColors(&data[0].colors[18], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[12], 0.0f, 0.0f, 0.125f, 0.125f);

                //t
                checkQuadPositions(&data[0].positions[36], 0.0f, -8.0f, 48.0f, 0.0f);
                checkQuadColors(&data[0].colors[36], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[24], 0.125f, 0.0f, 0.875f, 0.125f);

                //tr
                checkQuadPositions(&data[0].positions[54], 48.0f, -8.0f, 56.0f, 0.0f);
                checkQuadColors(&data[0].colors[54], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[36], 0.875f, 0.0f, 1.0f, 0.125f);

                //r
                checkQuadPositions(&data[0].positions[72], 48.0f, 0.0f, 56.0f, 48.0f);
                checkQuadColors(&data[0].colors[72], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[48], 0.875f, 0.125f, 1.0f, 0.875f);
                
                //br
                checkQuadPositions(&data[0].positions[90], 48.0f, 48.0f, 56.0f, 56.0f);
                checkQuadColors(&data[0].colors[90], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[60], 0.875f, 0.875f, 1.0f, 1.0f);
                
                //b
                checkQuadPositions(&data[0].positions[108], 0.0f, 48.0f, 48.0f, 56.0f);
                checkQuadColors(&data[0].colors[108], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[72], 0.125f, 0.875f, 0.875f, 1.0f);
                
                //bl
                checkQuadPositions(&data[0].positions[126], -8.0f, 48.0f, 0.0f, 56.0f);
                checkQuadColors(&data[0].colors[126], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[84], 0.0f, 0.875f, 0.125f, 1.0f);

                //l
                checkQuadPositions(&data[0].positions[144], -8.0f, 0.0f, 0.0f, 48.0f);
                checkQuadColors(&data[0].colors[144], 1.0f, 1.0f, 1.0f);
                checkQuadTexCoords(&data[0].texCoords[96], 0.0f, 0.125f, 125.0f, 0.875f);
            }
        }
    }
}
    //"border_mode":BorderMode;
    //"border_coords_tl":Rectangle<Vec2>(start, size);
    //"border_coords_t": Rectangle<Vec2>(start, size);
    //"border_coords_tr":Rectangle<Vec2>(start, size);
    //"border_coords_r": Rectangle<Vec2>(start, size);
    //"border_coords_br":Rectangle<Vec2>(start, size);
    //"border_coords_b": Rectangle<Vec2>(start, size);
    //"border_coords_bl":Rectangle<Vec2>(start, size);
    //"border_coords_l": Rectangle<Vec2>(start, size);
