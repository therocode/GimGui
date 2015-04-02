#include <fstream>
#include <catch.hpp>
#include <gimgui/data/element.hpp>
#include <gimgui/data/font.hpp>
#include <gimgui/logic/renderdatagenerator.hpp>
#include <helpers/vec2.hpp>
#include <helpers/color.hpp>
#include <helpers/closeenough.hpp>
#include <helpers/textureinterfacestub.hpp>

bool checkQuadPositions(const float* firstPosition, float xMin, float yMin, float xMax, float yMax)
{
    return(closeEnough(*(firstPosition + 0) ,  xMin))&&//x
          (closeEnough(*(firstPosition + 1) ,  yMin))&&//y
          (closeEnough(*(firstPosition + 2) ,  0.0f))&&
          (closeEnough(*(firstPosition + 3) ,  xMin))&&//x
          (closeEnough(*(firstPosition + 4) ,  yMax))&&//y
          (closeEnough(*(firstPosition + 5) ,  0.0f))&&
          (closeEnough(*(firstPosition + 6) ,  xMax))&&//x
          (closeEnough(*(firstPosition + 7) ,  yMax))&&//y
          (closeEnough(*(firstPosition + 8) ,  0.0f))&&
          (closeEnough(*(firstPosition + 9) ,  xMax))&&//x
          (closeEnough(*(firstPosition + 10),  yMax))&&//y
          (closeEnough(*(firstPosition + 11),  0.0f))&&
          (closeEnough(*(firstPosition + 12),  xMax))&&//x
          (closeEnough(*(firstPosition + 13),  yMin))&&//y
          (closeEnough(*(firstPosition + 14),  0.0f))&&
          (closeEnough(*(firstPosition + 15),  xMin))&&//x
          (closeEnough(*(firstPosition + 16),  yMin))&&//y
          (closeEnough(*(firstPosition + 17),  0.0f));
}

bool checkQuadColors(const float* firstColor, float r, float g, float b)
{
    return(closeEnough(*(firstColor + 0) , r))&&//r
          (closeEnough(*(firstColor + 1) , g))&&//g
          (closeEnough(*(firstColor + 2) , b))&&//b
          (closeEnough(*(firstColor + 3) , r))&&//r
          (closeEnough(*(firstColor + 4) , g))&&//g
          (closeEnough(*(firstColor + 5) , b))&&//b
          (closeEnough(*(firstColor + 6) , r))&&//r
          (closeEnough(*(firstColor + 7) , g))&&//g
          (closeEnough(*(firstColor + 8) , b))&&//b
          (closeEnough(*(firstColor + 9) , r))&&//r
          (closeEnough(*(firstColor + 10), g))&&//g
          (closeEnough(*(firstColor + 11), b))&&//b
          (closeEnough(*(firstColor + 12), r))&&//r
          (closeEnough(*(firstColor + 13), g))&&//g
          (closeEnough(*(firstColor + 14), b))&&//b
          (closeEnough(*(firstColor + 15), r))&&//r
          (closeEnough(*(firstColor + 16), g))&&//g
          (closeEnough(*(firstColor + 17), b));//b
}

bool checkQuadTexCoords(const float* firstTexCoord, float xMin, float yMin, float xMax, float yMax)
{
    return(closeEnough(*(firstTexCoord + 0) ,  xMin))&&//u
          (closeEnough(*(firstTexCoord + 1) ,  yMin))&&//v
          (closeEnough(*(firstTexCoord + 2) ,  xMin))&&//u
          (closeEnough(*(firstTexCoord + 3) ,  yMax))&&//v
          (closeEnough(*(firstTexCoord + 4) ,  xMax))&&//u
          (closeEnough(*(firstTexCoord + 5) ,  yMax))&&//v
          (closeEnough(*(firstTexCoord + 6) ,  xMax))&&//u
          (closeEnough(*(firstTexCoord + 7) ,  yMax))&&//v
          (closeEnough(*(firstTexCoord + 8) ,  xMax))&&//u
          (closeEnough(*(firstTexCoord + 9) ,  yMin))&&//v
          (closeEnough(*(firstTexCoord + 10),  xMin))&&//u
          (closeEnough(*(firstTexCoord + 11),  yMin));//v
}

SCENARIO("RenderDataGenerator can be used to turn a gui tree into triangle buffers and such data for rendering", "[logic]")
{
    GIVEN("a gui tree with elements with the attributes width and height, and a RenderDataGenerator")
    {
        gim::RenderDataGenerator<Vec2, Color> generator;
        uint32_t imageId = generator.registerImageInfo(Vec2({64, 64}));

        gim::Element root({"container"},
        {
            {"color",    Color(255, 0, 0)},
            {"position", Vec2({5, 5})},
            {"size",     Vec2({40, 30})},
            {"image_id", imageId},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))}
        },
        {
            gim::Element({"child"},
            {
                {"position", Vec2({10, 10})},
                {"size",     Vec2({10, 10})}
            })
        });

        WHEN("the RenderDataGenerator is used to get rendering info from the tree")
        {
            std::vector<gim::RenderData> data = generator.generate(root);

            THEN("the data is correct")
            {
                REQUIRE(data.size() == 2);
                REQUIRE(data[0].positions.size() == 18);
                REQUIRE(data[0].colors.size() == 18);
                REQUIRE(data[0].texCoords.size() == 12);
                REQUIRE(data[1].positions.size() == 0);
                REQUIRE(data[1].colors.size() == 0);
                REQUIRE(data[1].texCoords.size() == 0);

                CHECK(data[0].imageId == imageId);
                CHECK(data[0].element == &root);
                CHECK(data[1].element == root.children()[0].get());

                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 45.0f, 35.0f));

                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 0.0f, 0.0f));

                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f));
            }
        }
    }

    //tiles
    GIVEN("an element set to not be tiled")
    {
        gim::RenderDataGenerator<Vec2, Color> generator;
        uint32_t imageId = generator.registerImageInfo(Vec2({64, 64}));

        gim::Element element({"non-tiled"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({40, 30})},
            {"stretch_mode", gim::StretchMode::STRETCHED},
            {"image_id", imageId},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 45.0f, 35.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f));
            }
        }
    }

    GIVEN("an element bigger than one tile set to be fully tiled")
    {
        gim::RenderDataGenerator<Vec2, Color> generator;
        uint32_t imageId = generator.registerImageInfo(Vec2({64, 64}));
        gim::Element element({"tiled"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({70, 37})},
            {"stretch_mode", gim::StretchMode::TILED},
            {"image_id", imageId},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 37.0f, 37.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f));

                CHECK(checkQuadPositions(&data[0].positions[18], 37.0f, 5.0f, 69.0f, 37.0f));
                CHECK(checkQuadColors(&data[0].colors[18], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[12], 0.0f, 0.0f, 0.5f, 0.5f));

                CHECK(checkQuadPositions(&data[0].positions[36], 69.0f, 5.0f, 75.0f, 37.0f));
                CHECK(checkQuadColors(&data[0].colors[36], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[24], 0.0f, 0.0f, 0.09375f, 0.5f));


                CHECK(checkQuadPositions(&data[0].positions[54], 5.0f, 37.0f, 37.0f, 42.0f));
                CHECK(checkQuadColors(&data[0].colors[54], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[36], 0.0f, 0.0f, 0.5f, 0.078125f));

                CHECK(checkQuadPositions(&data[0].positions[72], 37.0f, 37.0f, 69.0f, 42.0f));
                CHECK(checkQuadColors(&data[0].colors[72], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[48], 0.0f, 0.0f, 0.5f, 0.078125f));

                CHECK(checkQuadPositions(&data[0].positions[90], 69.0f, 37.0f, 75.0f, 42.0f));
                CHECK(checkQuadColors(&data[0].colors[90], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[60], 0.0f, 0.0f, 0.09375f, 0.078125f));
            }
        }
    }

    GIVEN("an element smaller than one tile set to be fully tiled")
    {
        gim::RenderDataGenerator<Vec2, Color> generator;
        uint32_t imageId = generator.registerImageInfo(Vec2({64, 64}));
        gim::Element element({"tiled"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({20, 10})},
            {"stretch_mode", gim::StretchMode::TILED},
            {"image_id", imageId},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 25.0f, 15.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.3125f, 0.15625f));
            }
        }
    }

    GIVEN("an element bigger than one tile set to be vertically tiled")
    {
        gim::RenderDataGenerator<Vec2, Color> generator;
        uint32_t imageId = generator.registerImageInfo(Vec2({64, 64}));
        gim::Element element({"tiled"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({70, 37})},
            {"stretch_mode", gim::StretchMode::V_TILED},
            {"image_id", imageId},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 75.0f, 37.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f));

                CHECK(checkQuadPositions(&data[0].positions[18], 5.0f, 37.0f, 75.0f, 42.0f));
                CHECK(checkQuadColors(&data[0].colors[18], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[12], 0.0f, 0.0f, 0.5f, 0.078125f));
            }
        }
    }

    GIVEN("an element smaller than one tile set to be vertically tiled")
    {
        gim::RenderDataGenerator<Vec2, Color> generator;
        uint32_t imageId = generator.registerImageInfo(Vec2({64, 64}));
        gim::Element element({"tiled"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({20, 10})},
            {"stretch_mode", gim::StretchMode::V_TILED},
            {"image_id", imageId},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 25.0f, 15.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.15625f));
            }
        }
    }

    GIVEN("an element bigger than one tile set to be horizontally tiled")
    {
        gim::RenderDataGenerator<Vec2, Color> generator;
        uint32_t imageId = generator.registerImageInfo(Vec2({64, 64}));
        gim::Element element({"tiled"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({70, 37})},
            {"stretch_mode", gim::StretchMode::H_TILED},
            {"image_id", imageId},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 37.0f, 42.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f));

                CHECK(checkQuadPositions(&data[0].positions[18], 37.0f, 5.0f, 69.0f, 42.0f));
                CHECK(checkQuadColors(&data[0].colors[18], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[12], 0.0f, 0.0f, 0.5f, 0.5f));

                CHECK(checkQuadPositions(&data[0].positions[36], 69.0f, 5.0f, 75.0f, 42.0f));
                CHECK(checkQuadColors(&data[0].colors[36], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[24], 0.0f, 0.0f, 0.09375f, 0.5f));
            }
        }
    }

    GIVEN("an element smaller than one tile set to be horizontally tiled")
    {
        gim::RenderDataGenerator<Vec2, Color> generator;
        uint32_t imageId = generator.registerImageInfo(Vec2({64, 64}));
        gim::Element element({"tiled"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({20, 10})},
            {"stretch_mode", gim::StretchMode::H_TILED},
            {"image_id", imageId},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 25.0f, 15.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.3125f, 0.5f));
            }
        }
    }

    GIVEN("an element smaller than one tile and having an image being a full subset of another one set to be fully tiled")
    {
        gim::RenderDataGenerator<Vec2, Color> generator;
        uint32_t imageId = generator.registerImageInfo(Vec2({64, 64}));
        gim::Element element({"tiled"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({8, 8})},
            {"stretch_mode", gim::StretchMode::TILED},
            {"image_id", imageId},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({16, 16}), Vec2({32, 32}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 13.0f, 13.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.25f, 0.25f, 0.375f, 0.375f));
            }
        }
    }

    //borders
    GIVEN("an element set to have no border")
    {
        gim::RenderDataGenerator<Vec2, Color> generator;
        uint32_t imageId = generator.registerImageInfo(Vec2({64, 64}));
        gim::Element element({"no border"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({32, 32})},
            {"image_id", imageId},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))},
            {"border_mode", gim::BorderMode::NONE}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 37.0f, 37.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f));
            }
        }
    }

    GIVEN("an element set to have left-right border")
    {
        gim::RenderDataGenerator<Vec2, Color> generator;
        uint32_t imageId = generator.registerImageInfo(Vec2({64, 64}));
        gim::Element element({"left-right"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({32, 32})},
            {"image_id", imageId},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))},
            {"border_mode", gim::BorderMode::LEFT_RIGHT},
            {"border_coords_l", gim::Rectangle<Vec2>(Vec2({32,0}), Vec2({8,32}))},
            {"border_coords_r", gim::Rectangle<Vec2>(Vec2({40,0}), Vec2({8,32}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 13.0f, 5.0f, 29.0f, 37.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f));

                CHECK(checkQuadPositions(&data[0].positions[18], 5.0f, 5.0f, 13.0f, 37.0f));
                CHECK(checkQuadColors(&data[0].colors[18], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[12], 0.5f, 0.0f, 0.625f, 0.5f));

                CHECK(checkQuadPositions(&data[0].positions[36], 29.0f, 5.0f, 37.0f, 37.0f));
                CHECK(checkQuadColors(&data[0].colors[36], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[24], 0.625f, 0.0f, 0.75f, 0.5f));
            }
        }
    }

    GIVEN("an element set to have top-bottom border")
    {
        gim::RenderDataGenerator<Vec2, Color> generator;
        uint32_t imageId = generator.registerImageInfo(Vec2({64, 64}));
        gim::Element element({"top-bottom"},
        {
            {"position", Vec2({5, 5})},
            {"size",     Vec2({32, 32})},
            {"image_id", imageId},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({0, 0}), Vec2({32, 32}))},
            {"border_mode", gim::BorderMode::TOP_BOTTOM},
            {"border_coords_t", gim::Rectangle<Vec2>(Vec2({0,32}), Vec2({32,8}))},
            {"border_coords_b", gim::Rectangle<Vec2>(Vec2({0,40}), Vec2({32,8}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 13.0f, 37.0f, 29.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f));

                CHECK(checkQuadPositions(&data[0].positions[18], 5.0f, 5.0f, 37.0f, 13.0f));
                CHECK(checkQuadColors(&data[0].colors[18], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[12], 0.0f, 0.5f, 0.5f, 0.625f));

                CHECK(checkQuadPositions(&data[0].positions[36], 5.0f, 29.0f, 37.0f, 37.0f));
                CHECK(checkQuadColors(&data[0].colors[36], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[24], 0.0f, 0.625f, 0.5f, 0.75f));
            }
        }
    }

    GIVEN("an element set to have full border")
    {
        gim::RenderDataGenerator<Vec2, Color> generator;
        uint32_t imageId = generator.registerImageInfo(Vec2({64, 64}));
        gim::Element element({"full"},
        {
            {"position", Vec2({0, 0})},
            {"size",     Vec2({48, 48})},
            {"image_id", imageId},
            {"image_coords", gim::Rectangle<Vec2>(Vec2({8, 8}), Vec2({48, 48}))},
            {"border_mode", gim::BorderMode::FULL},
            {"border_coords_tl", gim::Rectangle<Vec2>(Vec2({0 ,0 }), Vec2({8 ,8 }))},
            {"border_coords_t",  gim::Rectangle<Vec2>(Vec2({8 ,0 }), Vec2({48,8 }))},
            {"border_coords_tr", gim::Rectangle<Vec2>(Vec2({56,0 }), Vec2({8 ,8 }))},
            {"border_coords_r",  gim::Rectangle<Vec2>(Vec2({56,8 }), Vec2({8 ,48}))},
            {"border_coords_br", gim::Rectangle<Vec2>(Vec2({56,56}), Vec2({8 ,8 }))},
            {"border_coords_b",  gim::Rectangle<Vec2>(Vec2({8 ,56}), Vec2({48,8 }))},
            {"border_coords_bl", gim::Rectangle<Vec2>(Vec2({0 ,56}), Vec2({8 ,8 }))},
            {"border_coords_l",  gim::Rectangle<Vec2>(Vec2({0 ,8 }), Vec2({8 ,48}))}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                //main quad
                CHECK(checkQuadPositions(&data[0].positions[0], 8.0f, 8.0f, 40.0f, 40.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.125f, 0.125f, 0.875f, 0.875f));

                //t
                CHECK(checkQuadPositions(&data[0].positions[18], 8.0f, 0.0f, 40.0f, 8.0f));
                CHECK(checkQuadColors(&data[0].colors[18], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[12], 0.125f, 0.0f, 0.875f, 0.125f));
                
                //b
                CHECK(checkQuadPositions(&data[0].positions[36], 8.0f, 40.0f, 40.0f, 48.0f));
                CHECK(checkQuadColors(&data[0].colors[36], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[24], 0.125f, 0.875f, 0.875f, 1.0f));

                //l
                CHECK(checkQuadPositions(&data[0].positions[54], 0.0f, 8.0f, 8.0f, 40.0f));
                CHECK(checkQuadColors(&data[0].colors[54], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[36], 0.0f, 0.125f, 0.125f, 0.875f));

                //r
                CHECK(checkQuadPositions(&data[0].positions[72], 40.0f, 8.0f, 48.0f, 40.0f));
                CHECK(checkQuadColors(&data[0].colors[72], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[48], 0.875f, 0.125f, 1.0f, 0.875f));

                //tl
                CHECK(checkQuadPositions(&data[0].positions[90], 0.0f, 0.0f, 8.0f, 8.0f));
                CHECK(checkQuadColors(&data[0].colors[90], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[60], 0.0f, 0.0f, 0.125f, 0.125f));

                //tr
                CHECK(checkQuadPositions(&data[0].positions[108], 40.0f, 0.0f, 48.0f, 8.0f));
                CHECK(checkQuadColors(&data[0].colors[108], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[72], 0.875f, 0.0f, 1.0f, 0.125f));
                
                //bl
                CHECK(checkQuadPositions(&data[0].positions[126], 0.0f, 40.0f, 8.0f, 48.0f));
                CHECK(checkQuadColors(&data[0].colors[126], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[84], 0.0f, 0.875f, 0.125f, 1.0f));
                
                //br
                CHECK(checkQuadPositions(&data[0].positions[144], 40.0f, 40.0f, 48.0f, 48.0f));
                CHECK(checkQuadColors(&data[0].colors[144], 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[96], 0.875f, 0.875f, 1.0f, 1.0f));
            }
        }
    }
}

SCENARIO("By registering a font and a texture with it, the RenderDataGenerator can output text quads", "[logic]")
{
    GIVEN("A RenderDataGenerator with a font and texture registered")
    {
        std::ifstream file("resources/fonts/LiberationSans-Regular.ttf", std::ios::binary);
        gim::Font font(file);
        TextureInterfaceStub textureAdaptor;

        gim::RenderDataGenerator<Vec2, Color> generator;

        auto ids = generator.registerFont(font, textureAdaptor);

        WHEN("a gui element with the attributes 'text' and 'text_font' are set, and render data generated")
        {
            gim::Element element({"text"},
            {
                {"position", Vec2({0, 0})},
                {"size",     Vec2({48, 48})},
                {"text", std::string("AAbbA")},
                {"font", ids.fontId}
            });

            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("quads for the text is returned with reasonable texture coordinates")
            {
               // REQUIRE(data[0].positions.size() == 90);
               // REQUIRE(data[0].colors.size() == 90);
               // REQUIRE(data[0].texCoords.size() == 60);

               // //first A equals second A
               // CHECK(std::equal(data[0].colors.begin() + 0, data[0].colors.begin() + 18, data[0].colors.begin() + 18));
               // CHECK(std::equal(data[0].texCoords.begin() + 0, data[0].texCoords.begin() + 12, data[0].texCoords.begin() + 12));

               // //first A equals third A
               // CHECK(std::equal(data[0].colors.begin() + 0, data[0].colors.begin() + 18, data[0].colors.begin() + 72));
               // CHECK(std::equal(data[0].texCoords.begin() + 0, data[0].texCoords.begin() + 12, data[0].texCoords.begin() + 48));

               // //first b equals other b
               // CHECK(std::equal(data[0].colors.begin() + 36, data[0].colors.begin() + 54, data[0].colors.begin() + 54));
               // CHECK(std::equal(data[0].texCoords.begin() + 24, data[0].texCoords.begin() + 36, data[0].texCoords.begin() + 36));

               // //first A does not equal first b
               // CHECK(!std::equal(data[0].colors.begin() + 0, data[0].colors.begin() + 18, data[0].colors.begin() + 36));
               // CHECK(!std::equal(data[0].texCoords.begin() + 0, data[0].texCoords.begin() + 12, data[0].texCoords.begin() + 24));
            }
        }
    }
}
