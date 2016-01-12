#include <fstream>
#include <catch.hpp>
#include <gimgui/util/ref.hpp>
#include <gimgui/data/element.hpp>
#include <gimgui/data/font.hpp>
#include <gimgui/logic/renderdatagenerator.hpp>
#include <helpers/vector2.hpp>
#include <helpers/rectangle.hpp>
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

bool checkQuadColors(const float* firstColor, float r, float g, float b, float a)
{
    return(closeEnough(*(firstColor + 0) , r))&&
          (closeEnough(*(firstColor + 1) , g))&&
          (closeEnough(*(firstColor + 2) , b))&&
          (closeEnough(*(firstColor + 3) , a))&&
          (closeEnough(*(firstColor + 4) , r))&&
          (closeEnough(*(firstColor + 5) , g))&&
          (closeEnough(*(firstColor + 6) , b))&&
          (closeEnough(*(firstColor + 7) , a))&&
          (closeEnough(*(firstColor + 8) , r))&&
          (closeEnough(*(firstColor + 9) , g))&&
          (closeEnough(*(firstColor + 10), b))&&
          (closeEnough(*(firstColor + 11), a))&&
          (closeEnough(*(firstColor + 12), r))&&
          (closeEnough(*(firstColor + 13), g))&&
          (closeEnough(*(firstColor + 14), b))&&
          (closeEnough(*(firstColor + 15), a))&&
          (closeEnough(*(firstColor + 16), r))&&
          (closeEnough(*(firstColor + 17), g))&&
          (closeEnough(*(firstColor + 18), b))&&
          (closeEnough(*(firstColor + 19), a))&&
          (closeEnough(*(firstColor + 20), r))&&
          (closeEnough(*(firstColor + 21), g))&&
          (closeEnough(*(firstColor + 22), b))&&
          (closeEnough(*(firstColor + 23), a));
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
        TextureInterfaceStub textureAdaptor({64, 64}, 30);
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;
        generator.registerTexture("test_texture", textureAdaptor);

        gim::Element root({"container"},
        {
            {"color",    Color(255, 0, 0, 255)},
            {"position", Vector2({5, 5})},
            {"size",     Vector2({40, 30})},
            {"texture", std::string("test_texture")},
            {"image_coords", Rectangle({{0, 0}, {32, 32}})}
        },
        {
            gim::Element({"child"},
            {
                {"position", Vector2({10, 10})},
                {"size",     Vector2({10, 10})}
            })
        });

        WHEN("the RenderDataGenerator is used to get rendering info from the tree")
        {
            std::vector<gim::RenderData> data = generator.generate(root);

            THEN("the data is correct")
            {
                REQUIRE(data.size() == 2);
                REQUIRE(data[0].positions.size() == 18);
                REQUIRE(data[0].colors.size() == 24);
                REQUIRE(data[0].texCoords.size() == 12);
                REQUIRE(data[1].positions.size() == 0);
                REQUIRE(data[1].colors.size() == 0);
                REQUIRE(data[1].texCoords.size() == 0);

                CHECK(data[0].textureHandle == 30);
                CHECK(data[0].element == &root);
                CHECK(data[1].element == root.children()[0].get());

                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 45.0f, 35.0f));

                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 0.0f, 0.0f, 1.0f));

                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f));
            }
        }
    }

    //tiles
    GIVEN("an element set to not be tiled")
    {
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;
        TextureInterfaceStub textureAdaptor({64, 64}, 25);
        generator.registerTexture("test_texture", textureAdaptor);

        gim::Element element({"non-tiled"},
        {
            {"position", Vector2({5, 5})},
            {"size",     Vector2({40, 30})},
            {"stretch_mode", gim::StretchMode::STRETCHED},
            {"texture", std::string("test_texture")},
            {"image_coords", Rectangle({{0, 0}, {32, 32}})}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 45.0f, 35.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f));
            }
        }
    }

    GIVEN("an element bigger than one tile set to be fully tiled")
    {
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;
        TextureInterfaceStub textureAdaptor({64, 64}, 25);
        generator.registerTexture("test_texture", textureAdaptor);
        gim::Element element({"tiled"},
        {
            {"position", Vector2({5, 5})},
            {"size",     Vector2({70, 37})},
            {"stretch_mode", gim::StretchMode::TILED},
            {"texture", std::string("test_texture")},
            {"image_coords", Rectangle({{0, 0}, {32, 32}})}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 37.0f, 37.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f));

                CHECK(checkQuadPositions(&data[0].positions[18], 37.0f, 5.0f, 69.0f, 37.0f));
                CHECK(checkQuadColors(&data[0].colors[18], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[12], 0.0f, 0.0f, 0.5f, 0.5f));

                CHECK(checkQuadPositions(&data[0].positions[36], 69.0f, 5.0f, 75.0f, 37.0f));
                CHECK(checkQuadColors(&data[0].colors[36], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[24], 0.0f, 0.0f, 0.09375f, 0.5f));


                CHECK(checkQuadPositions(&data[0].positions[54], 5.0f, 37.0f, 37.0f, 42.0f));
                CHECK(checkQuadColors(&data[0].colors[54], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[36], 0.0f, 0.0f, 0.5f, 0.078125f));

                CHECK(checkQuadPositions(&data[0].positions[72], 37.0f, 37.0f, 69.0f, 42.0f));
                CHECK(checkQuadColors(&data[0].colors[72], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[48], 0.0f, 0.0f, 0.5f, 0.078125f));

                CHECK(checkQuadPositions(&data[0].positions[90], 69.0f, 37.0f, 75.0f, 42.0f));
                CHECK(checkQuadColors(&data[0].colors[90], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[60], 0.0f, 0.0f, 0.09375f, 0.078125f));
            }
        }
    }

    GIVEN("an element smaller than one tile set to be fully tiled")
    {
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;
        TextureInterfaceStub textureAdaptor({64, 64}, 25);
        generator.registerTexture("test_texture", textureAdaptor);
        gim::Element element({"tiled"},
        {
            {"position", Vector2({5, 5})},
            {"size",     Vector2({20, 10})},
            {"stretch_mode", gim::StretchMode::TILED},
            {"texture", std::string("test_texture")},
            {"image_coords", Rectangle({{0, 0}, {32, 32}})},
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 25.0f, 15.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.3125f, 0.15625f));
            }
        }
    }

    GIVEN("an element bigger than one tile set to be vertically tiled")
    {
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;
        TextureInterfaceStub textureAdaptor({64, 64}, 25);
        generator.registerTexture("test_texture", textureAdaptor);
        gim::Element element({"tiled"},
        {
            {"position", Vector2({5, 5})},
            {"size",     Vector2({70, 37})},
            {"stretch_mode", gim::StretchMode::V_TILED},
            {"texture", std::string("test_texture")},
            {"image_coords", Rectangle({{0, 0}, {32, 32}})}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 75.0f, 37.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f));

                CHECK(checkQuadPositions(&data[0].positions[18], 5.0f, 37.0f, 75.0f, 42.0f));
                CHECK(checkQuadColors(&data[0].colors[18], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[12], 0.0f, 0.0f, 0.5f, 0.078125f));
            }
        }
    }

    GIVEN("an element smaller than one tile set to be vertically tiled")
    {
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;
        TextureInterfaceStub textureAdaptor({64, 64}, 25);
        generator.registerTexture("test_texture", textureAdaptor);
        gim::Element element({"tiled"},
        {
            {"position", Vector2({5, 5})},
            {"size",     Vector2({20, 10})},
            {"stretch_mode", gim::StretchMode::V_TILED},
            {"texture", std::string("test_texture")},
            {"image_coords", Rectangle({{0, 0}, {32, 32}})}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 25.0f, 15.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.15625f));
            }
        }
    }

    GIVEN("an element bigger than one tile set to be horizontally tiled")
    {
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;
        TextureInterfaceStub textureAdaptor({64, 64}, 25);
        generator.registerTexture("test_texture", textureAdaptor);
        gim::Element element({"tiled"},
        {
            {"position", Vector2({5, 5})},
            {"size",     Vector2({70, 37})},
            {"stretch_mode", gim::StretchMode::H_TILED},
            {"texture", std::string("test_texture")},
            {"image_coords", Rectangle({{0, 0}, {32, 32}})}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 37.0f, 42.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f));

                CHECK(checkQuadPositions(&data[0].positions[18], 37.0f, 5.0f, 69.0f, 42.0f));
                CHECK(checkQuadColors(&data[0].colors[18], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[12], 0.0f, 0.0f, 0.5f, 0.5f));

                CHECK(checkQuadPositions(&data[0].positions[36], 69.0f, 5.0f, 75.0f, 42.0f));
                CHECK(checkQuadColors(&data[0].colors[36], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[24], 0.0f, 0.0f, 0.09375f, 0.5f));
            }
        }
    }

    GIVEN("an element smaller than one tile set to be horizontally tiled")
    {
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;
        TextureInterfaceStub textureAdaptor({64, 64}, 25);
        generator.registerTexture("test_texture", textureAdaptor);
        gim::Element element({"tiled"},
        {
            {"position", Vector2({5, 5})},
            {"size",     Vector2({20, 10})},
            {"stretch_mode", gim::StretchMode::H_TILED},
            {"texture", std::string("test_texture")},
            {"image_coords", Rectangle({{0, 0}, {32, 32}})}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 25.0f, 15.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.3125f, 0.5f));
            }
        }
    }

    GIVEN("an element smaller than one tile and having an image being a full subset of another one set to be fully tiled")
    {
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;
        TextureInterfaceStub textureAdaptor({64, 64}, 25);
        generator.registerTexture("test_texture", textureAdaptor);
        gim::Element element({"tiled"},
        {
            {"position", Vector2({5, 5})},
            {"size",     Vector2({8, 8})},
            {"stretch_mode", gim::StretchMode::TILED},
            {"texture", std::string("test_texture")},
            {"image_coords", Rectangle({{16, 16}, {32, 32}})}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 13.0f, 13.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.25f, 0.25f, 0.375f, 0.375f));
            }
        }
    }

    //borders
    GIVEN("an element set to have no border")
    {
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;
        TextureInterfaceStub textureAdaptor({64, 64}, 25);
        generator.registerTexture("test_texture", textureAdaptor);
        gim::Element element({"no border"},
        {
            {"position", Vector2({5, 5})},
            {"size",     Vector2({32, 32})},
            {"texture", std::string("test_texture")},
            {"image_coords", Rectangle({{0, 0}, {32, 32}})},
            {"border_mode", gim::BorderMode::NONE}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 5.0f, 37.0f, 37.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f));
            }
        }
    }

    GIVEN("an element set to have left-right border")
    {
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;
        TextureInterfaceStub textureAdaptor({64, 64}, 25);
        generator.registerTexture("test_texture", textureAdaptor);
        gim::Element element({"left-right"},
        {
            {"position", Vector2({5, 5})},
            {"size",     Vector2({32, 32})},
            {"texture", std::string("test_texture")},
            {"image_coords", Rectangle({{0, 0}, {32, 32}})},
            {"border_mode", gim::BorderMode::LEFT_RIGHT},
            {"border_coords_l", Rectangle({{32,0}, {8,32}})},
            {"border_coords_r", Rectangle({{40,0}, {8,32}})}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 13.0f, 5.0f, 29.0f, 37.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f));

                CHECK(checkQuadPositions(&data[0].positions[18], 5.0f, 5.0f, 13.0f, 37.0f));
                CHECK(checkQuadColors(&data[0].colors[18], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[12], 0.5f, 0.0f, 0.625f, 0.5f));

                CHECK(checkQuadPositions(&data[0].positions[36], 29.0f, 5.0f, 37.0f, 37.0f));
                CHECK(checkQuadColors(&data[0].colors[36], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[24], 0.625f, 0.0f, 0.75f, 0.5f));
            }
        }
    }

    GIVEN("an element set to have top-bottom border")
    {
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;
        TextureInterfaceStub textureAdaptor({64, 64}, 25);
        generator.registerTexture("test_texture", textureAdaptor);
        gim::Element element({"top-bottom"},
        {
            {"position", Vector2({5, 5})},
            {"size",     Vector2({32, 32})},
            {"texture", std::string("test_texture")},
            {"image_coords", Rectangle({{0, 0}, {32, 32}})},
            {"border_mode", gim::BorderMode::TOP_BOTTOM},
            {"border_coords_t", Rectangle({{0,32}, {32,8}})},
            {"border_coords_b", Rectangle({{0,40}, {32,8}})}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                CHECK(checkQuadPositions(&data[0].positions[0], 5.0f, 13.0f, 37.0f, 29.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.0f, 0.0f, 0.5f, 0.5f));

                CHECK(checkQuadPositions(&data[0].positions[18], 5.0f, 5.0f, 37.0f, 13.0f));
                CHECK(checkQuadColors(&data[0].colors[18], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[12], 0.0f, 0.5f, 0.5f, 0.625f));

                CHECK(checkQuadPositions(&data[0].positions[36], 5.0f, 29.0f, 37.0f, 37.0f));
                CHECK(checkQuadColors(&data[0].colors[36], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[24], 0.0f, 0.625f, 0.5f, 0.75f));
            }
        }
    }

    GIVEN("an element set to have full border")
    {
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;
        TextureInterfaceStub textureAdaptor({64, 64}, 25);
        generator.registerTexture("test_texture", textureAdaptor);
        gim::Element element({"full"},
        {
            {"position", Vector2({0, 0})},
            {"size",     Vector2({48, 48})},
            {"texture", std::string("test_texture")},
            {"image_coords", Rectangle({{8, 8}, {48, 48}})},
            {"border_mode", gim::BorderMode::FULL},
            {"border_coords_tl", Rectangle({{0 ,0 }, {8 ,8 }})},
            {"border_coords_t",  Rectangle({{8 ,0 }, {48,8 }})},
            {"border_coords_tr", Rectangle({{56,0 }, {8 ,8 }})},
            {"border_coords_r",  Rectangle({{56,8 }, {8 ,48}})},
            {"border_coords_br", Rectangle({{56,56}, {8 ,8 }})},
            {"border_coords_b",  Rectangle({{8 ,56}, {48,8 }})},
            {"border_coords_bl", Rectangle({{0 ,56}, {8 ,8 }})},
            {"border_coords_l",  Rectangle({{0 ,8 }, {8 ,48}})}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the data is correct")
            {
                //main quad
                CHECK(checkQuadPositions(&data[0].positions[0], 8.0f, 8.0f, 40.0f, 40.0f));
                CHECK(checkQuadColors(&data[0].colors[0], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[0], 0.125f, 0.125f, 0.875f, 0.875f));

                //t
                CHECK(checkQuadPositions(&data[0].positions[18], 8.0f, 0.0f, 40.0f, 8.0f));
                CHECK(checkQuadColors(&data[0].colors[18], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[12], 0.125f, 0.0f, 0.875f, 0.125f));
                
                //b
                CHECK(checkQuadPositions(&data[0].positions[36], 8.0f, 40.0f, 40.0f, 48.0f));
                CHECK(checkQuadColors(&data[0].colors[36], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[24], 0.125f, 0.875f, 0.875f, 1.0f));

                //l
                CHECK(checkQuadPositions(&data[0].positions[54], 0.0f, 8.0f, 8.0f, 40.0f));
                CHECK(checkQuadColors(&data[0].colors[54], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[36], 0.0f, 0.125f, 0.125f, 0.875f));

                //r
                CHECK(checkQuadPositions(&data[0].positions[72], 40.0f, 8.0f, 48.0f, 40.0f));
                CHECK(checkQuadColors(&data[0].colors[72], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[48], 0.875f, 0.125f, 1.0f, 0.875f));

                //tl
                CHECK(checkQuadPositions(&data[0].positions[90], 0.0f, 0.0f, 8.0f, 8.0f));
                CHECK(checkQuadColors(&data[0].colors[90], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[60], 0.0f, 0.0f, 0.125f, 0.125f));

                //tr
                CHECK(checkQuadPositions(&data[0].positions[108], 40.0f, 0.0f, 48.0f, 8.0f));
                CHECK(checkQuadColors(&data[0].colors[108], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[72], 0.875f, 0.0f, 1.0f, 0.125f));
                
                //bl
                CHECK(checkQuadPositions(&data[0].positions[126], 0.0f, 40.0f, 8.0f, 48.0f));
                CHECK(checkQuadColors(&data[0].colors[126], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[84], 0.0f, 0.875f, 0.125f, 1.0f));
                
                //br
                CHECK(checkQuadPositions(&data[0].positions[144], 40.0f, 40.0f, 48.0f, 48.0f));
                CHECK(checkQuadColors(&data[0].colors[144], 1.0f, 1.0f, 1.0f, 1.0f));
                CHECK(checkQuadTexCoords(&data[0].texCoords[96], 0.875f, 0.875f, 1.0f, 1.0f));
            }
        }
    }

    GIVEN("an element with no z_position set")
    {
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;
        TextureInterfaceStub textureAdaptor({64, 64}, 25);
        generator.registerTexture("test_texture", textureAdaptor);
        gim::Element element({"tiled"},
        {
            {"position", Vector2({5, 5})},
            {"size",     Vector2({20, 10})},
            {"texture", std::string("test_texture")},
            {"image_coords", Rectangle({{0, 0}, {32, 32}})}
        });

        WHEN("a RenderDataGenerator is used to get rendering info from the element")
        {
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the z values are all zero")
            {
                REQUIRE(data[0].positions.size() > 0);
                for(int32_t i = 0; i < data[0].positions.size(); i += 3)
                {
                    CHECK(data[0].positions[i + 2] == Approx(0.0f));
                }
            }
        }

        WHEN("z_position is set to something else than zero")
        {
            element.createAttribute("z_position", 10.0f);
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the z values are all set to that value")
            {
                REQUIRE(data[0].positions.size() > 0);
                for(int32_t i = 0; i < data[0].positions.size(); i += 3)
                {
                    CHECK(data[0].positions[i + 2] == Approx(10.0f));
                }
            }
        }
    }
}

gim::Font loadFont(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    return gim::Font(file);
}

SCENARIO("Text quads can be generated from the RenderDataGenerator, with a few basic properties", "[logic]")
{
    GIVEN("A RenderDataGenerator with a font and texture registered")
    {
        gim::Font font = loadFont("resources/fonts/LiberationSans-Regular.ttf");

        TextureInterfaceStub textureAdaptor({20, 20}, 30);
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;

        generator.registerFontStorage({font}, textureAdaptor);

        WHEN("a gui element with the attributes 'text' and 'text_font' are set, and render data generated")
        {
            gim::Element element({"text"},
            {
                {"position", Vector2({0, 0})},
                {"size",     Vector2({48, 48})},
                {"text", std::string("AAbbA")},
                {"text_size", 16},
                {"font", font.name()}
            });

            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("quads for the text is returned with reasonable texture coordinates")
            {
                CHECK(data[0].positions.size() == 0);
                CHECK(data[0].colors.size() == 0);
                CHECK(data[0].texCoords.size() == 0);

                REQUIRE(data[0].textPositions.size() == 90);
                REQUIRE(data[0].textColors.size() == 120);
                REQUIRE(data[0].textTexCoords.size() == 60);
                CHECK(data[0].textTextureHandle == 30);

                //first A equals second A
                CHECK(std::equal(data[0].textTexCoords.begin() + 0, data[0].textTexCoords.begin() + 12, data[0].textTexCoords.begin() + 12));

                //first A equals third A
                CHECK(std::equal(data[0].textTexCoords.begin() + 0, data[0].textTexCoords.begin() + 12, data[0].textTexCoords.begin() + 48));

                //first b equals other b
                CHECK(std::equal(data[0].textTexCoords.begin() + 24, data[0].textTexCoords.begin() + 36, data[0].textTexCoords.begin() + 36));

                //first A does not equal first b
                CHECK(!std::equal(data[0].textTexCoords.begin() + 0, data[0].textTexCoords.begin() + 12, data[0].textTexCoords.begin() + 24));
            }
        }

        WHEN("when text is used like above but also given the 'text_color' attribute")
        {
            gim::Element element({"text"},
            {
                {"position", Vector2({0, 0})},
                {"size",     Vector2({48, 48})},
                {"text", std::string("hej")},
                {"text_color", Color{100, 0, 255, 200}},
                {"text_size", 16},
                {"font", font.name()}
            });

            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("quads for the text is returned with reasonable colour values")
            {
                CHECK(data[0].positions.size() == 0);
                CHECK(data[0].colors.size() == 0);
                CHECK(data[0].texCoords.size() == 0);

                REQUIRE(data[0].textPositions.size() == 54);
                REQUIRE(data[0].textColors.size() == 72);
                REQUIRE(data[0].textTexCoords.size() == 36);
                CHECK(data[0].textTextureHandle == 30);

                //colors are correct
                CHECK(data[0].textColors[0] == Approx(0.39215686274f));
                CHECK(data[0].textColors[1] == Approx(0.0f));
                CHECK(data[0].textColors[2] == Approx(1.0f));
                CHECK(data[0].textColors[3] == Approx(0.78431372549f));
            }
        }

        WHEN("bigger text scale is used, the output text is larger")
        {
            gim::Element element({"text"},
            {
                {"position", Vector2({0, 0})},
                {"size",     Vector2({48, 48})},
                {"text", std::string("hej")},
                {"text_size", 16},
                {"text_scale", 1.0f},
                {"font", font.name()}
            });

            std::vector<gim::RenderData> smallData = generator.generate(element);

            element.setAttribute("text_scale", 2.0f);

            std::vector<gim::RenderData> bigData = generator.generate(element);

            THEN("the small data is smaller")
            {
                REQUIRE(smallData[0].textPositions.size() == 54);
                REQUIRE(smallData[0].textColors.size() == 72);
                REQUIRE(smallData[0].textTexCoords.size() == 36);
                REQUIRE(bigData[0].textPositions.size() == 54);
                REQUIRE(bigData[0].textColors.size() == 72);
                REQUIRE(bigData[0].textTexCoords.size() == 36);

                float smallTotal = 0.0f;
                for(auto& data : smallData)
                    smallTotal = std::accumulate(data.textPositions.begin(), data.textPositions.end(), smallTotal);

                float bigTotal = 0.0f;
                for(auto& data : bigData)
                    bigTotal = std::accumulate(data.textPositions.begin(), data.textPositions.end(), bigTotal);

                CHECK(smallTotal < bigTotal);
            }
        }
    }
}

SCENARIO("Character spacing can be controlled in different ways when rendering text", "[logic]")
{
    GIVEN("A RenderDataGenerator with a font and texture registered")
    {
        gim::Font font = loadFont("resources/fonts/LiberationSans-Regular.ttf");

        TextureInterfaceStub textureAdaptor({20, 20}, 30);
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;

        generator.registerFontStorage({font}, textureAdaptor);

        WHEN("character_spacing and line_spacing are set")
        {
            gim::Element element({"text"},
            {
                {"position", Vector2({0, 0})},
                {"size",     Vector2({48, 48})},
                {"text", std::string("hej")},
                {"text_size", 16},
                {"character_spacing", 0.0f},
                {"line_spacing", 0.0f},
                {"font", font.name()}
            });

            std::vector<gim::RenderData> neutralData = generator.generate(element);

            element.setAttribute("character_spacing", 10.0f);
            std::vector<gim::RenderData> characterSpacedData = generator.generate(element);

            element.setAttribute("character_spacing", 0.0f);
            element.setAttribute("text", std::string("h\ne\nj"));
            std::vector<gim::RenderData> neutralData2 = generator.generate(element);
            element.setAttribute("line_spacing", 20.0f);
            std::vector<gim::RenderData> lineSpacedData = generator.generate(element);

            THEN("the quads are spaced accordingly")
            {
                REQUIRE(neutralData[0].textPositions.size() == 54);
                REQUIRE(neutralData[0].textColors.size() == 72);
                REQUIRE(neutralData[0].textTexCoords.size() == 36);
                REQUIRE(characterSpacedData[0].textPositions.size() == 54);
                REQUIRE(characterSpacedData[0].textColors.size() == 72);
                REQUIRE(characterSpacedData[0].textTexCoords.size() == 36);
                REQUIRE(neutralData2[0].textPositions.size() == 54);
                REQUIRE(neutralData2[0].textColors.size() == 72);
                REQUIRE(neutralData2[0].textTexCoords.size() == 36);
                REQUIRE(lineSpacedData[0].textPositions.size() == 54);
                REQUIRE(lineSpacedData[0].textColors.size() == 72);
                REQUIRE(lineSpacedData[0].textTexCoords.size() == 36);

                CHECK(neutralData[0].textPositions[0]  == Approx(characterSpacedData[0].textPositions[0]));
                CHECK(neutralData[0].textPositions[18] == Approx(characterSpacedData[0].textPositions[18] - 10.0f));
                CHECK(neutralData[0].textPositions[36] == Approx(characterSpacedData[0].textPositions[36] - 20.0f));

                CHECK(neutralData2[0].textPositions[1]  == Approx(lineSpacedData[0].textPositions[1]));
                CHECK(neutralData2[0].textPositions[19] == Approx(lineSpacedData[0].textPositions[19] - 20.0f));
                CHECK(neutralData2[0].textPositions[37] == Approx(lineSpacedData[0].textPositions[37] - 40.0f));
            }
        }

        WHEN("tab_width is set to different things")
        {
            gim::Element element({"text"},
            {
                {"position", Vector2({0, 0})},
                {"size",     Vector2({480, 480})},
                {"text", std::string("\thej")},
                {"text_size", 16},
                {"tab_width", 4},
                {"font", font.name()}
            });

            std::vector<gim::RenderData> neutralData = generator.generate(element);

            element.setAttribute("tab_width", 8);
            std::vector<gim::RenderData> doubleTabbedData = generator.generate(element);

            THEN("the double tabbed data is twice as much spaced")
            {
                REQUIRE(neutralData[0].textPositions.size() == 54);
                REQUIRE(neutralData[0].textColors.size() == 72);
                REQUIRE(neutralData[0].textTexCoords.size() == 36);
                REQUIRE(doubleTabbedData[0].textPositions.size() == 54);
                REQUIRE(doubleTabbedData[0].textColors.size() == 72);
                REQUIRE(doubleTabbedData[0].textTexCoords.size() == 36);

                CHECK(neutralData[0].textPositions[0]  == Approx(doubleTabbedData[0].textPositions[0] / 2.0f));
            }
        }
    }
}

SCENARIO("Different text styles can be applied using the text_style attribute", "[logic]")
{
    GIVEN("A RenderDataGenerator with fonts and texture registered")
    {
        gim::Font font = loadFont("resources/fonts/LiberationSans-Regular.ttf");
        gim::Font boldFont = loadFont("resources/fonts/LiberationSans-Bold.ttf");
        gim::Font italicFont = loadFont("resources/fonts/LiberationSans-Italic.ttf");
        gim::Font boldItalicFont = loadFont("resources/fonts/LiberationSans-BoldItalic.ttf");

        TextureInterfaceStub textureAdaptor({20, 20}, 30);
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;

        generator.registerFontStorage({font, boldFont, italicFont, boldItalicFont}, textureAdaptor);

        WHEN("text_style is set to different styles")
        {
            gim::Element element({"text"},
            {
                {"position", Vector2({0, 0})},
                {"size",     Vector2({48, 48})},
                {"text", std::string("hej")},
                {"text_size", 16},
                {"text_style", gim::TextStyle::NORMAL},
                {"font", font.name()},
                {"bold_font", boldFont.name()},
                {"italic_font", italicFont.name()},
                {"bold_italic_font", boldItalicFont.name()},
            });

            std::vector<gim::RenderData> normalData = generator.generate(element);

            element.setAttribute("text_style", gim::TextStyle::BOLD);
            std::vector<gim::RenderData> boldData = generator.generate(element);

            element.setAttribute("text_style", gim::TextStyle::ITALIC);
            std::vector<gim::RenderData> italicData = generator.generate(element);

            element.setAttribute("text_style", gim::TextStyle::BOLD | gim::TextStyle::ITALIC);
            std::vector<gim::RenderData> boldItalicData = generator.generate(element);

            THEN("normal/bold/italic/bold-italic font data all have unique texture coordinates")
            {
                REQUIRE(normalData[0].textPositions.size() == 54);
                REQUIRE(normalData[0].textColors.size() == 72);
                REQUIRE(normalData[0].textTexCoords.size() == 36);
                REQUIRE(boldData[0].textPositions.size() == 54);
                REQUIRE(boldData[0].textColors.size() == 72);
                REQUIRE(boldData[0].textTexCoords.size() == 36);
                REQUIRE(italicData[0].textPositions.size() == 54);
                REQUIRE(italicData[0].textColors.size() == 72);
                REQUIRE(italicData[0].textTexCoords.size() == 36);
                REQUIRE(boldItalicData[0].textPositions.size() == 54);
                REQUIRE(boldItalicData[0].textColors.size() == 72);
                REQUIRE(boldItalicData[0].textTexCoords.size() == 36);

                CHECK((normalData[0].textTexCoords[0] != boldData[0].textTexCoords[0]       || normalData[0].textTexCoords[1] != boldData[0].textTexCoords[1]));
                CHECK((normalData[0].textTexCoords[0] != italicData[0].textTexCoords[0]     || normalData[0].textTexCoords[1] != italicData[0].textTexCoords[1]));
                CHECK((normalData[0].textTexCoords[0] != boldItalicData[0].textTexCoords[0] || normalData[0].textTexCoords[1] != boldItalicData[0].textTexCoords[1]));
                CHECK((boldData[0].textTexCoords[0]   != italicData[0].textTexCoords[0]     || boldData[0].textTexCoords[1]   != italicData[0].textTexCoords[1]));
                CHECK((boldData[0].textTexCoords[0]   != boldItalicData[0].textTexCoords[0] || boldData[0].textTexCoords[1]   != boldItalicData[0].textTexCoords[1]));
                CHECK((italicData[0].textTexCoords[0] != boldItalicData[0].textTexCoords[0] || italicData[0].textTexCoords[1] != boldItalicData[0].textTexCoords[1]));
            }
        }
    }
}

SCENARIO("Text flow behaviour can be controlled using line_wrap", "[logic]")
{
    GIVEN("A RenderDataGenerator with a font and texture registered, and render data without line wrapping")
    {
        gim::Font font = loadFont("resources/fonts/LiberationSans-Regular.ttf");

        TextureInterfaceStub textureAdaptor({20, 20}, 30);
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;

        generator.registerFontStorage({font}, textureAdaptor);

        gim::Element element({"text"},
        {
            {"position", Vector2({0, 0})},
            {"size",     Vector2({100, 256})},
            {"text", std::string("Hello, I like old pillows")},
            {"text_size", 16},
            {"text_style", gim::TextStyle::NORMAL},
            {"font", font.name()}
        });

        std::vector<gim::RenderData> normalData = generator.generate(element);

        WHEN("line_wrap is set to WORDS")
        {
            element.createAttribute("line_wrap", gim::WrapMode::WORDS);

            std::vector<gim::RenderData> wordWrappedData = generator.generate(element);

            THEN("it is the same as without since it is the default")
            {
                CHECK(normalData[0].textPositions == wordWrappedData[0].textPositions);
            }

            THEN("it wraps the whole word which reaches the border of the element")
            {
                REQUIRE(normalData[0].textPositions.size() > 0);
                REQUIRE(wordWrappedData[0].textPositions.size() > 0);

                CHECK(wordWrappedData[0].textPositions[10 * 3 * 3 * 2] > wordWrappedData[0].textPositions[11 * 3 * 3 * 2]);
            }
        }

        WHEN("line_wrap is set to CHARACTERS, and data is generated")
        {
            element.createAttribute("line_wrap", gim::WrapMode::CHARACTERS);

            std::vector<gim::RenderData> characterWrappedData = generator.generate(element);

            THEN("it wraps mid-words when the border of the element is reached")
            {
                REQUIRE(normalData[0].textPositions.size() > 0);
                REQUIRE(characterWrappedData[0].textPositions.size() > 0);

                CHECK_FALSE(normalData[0].textPositions[12 * 3 * 3 * 2 + 1] == characterWrappedData[0].textPositions[12 * 3 * 3 * 2 + 1]);
                CHECK(normalData[0].textPositions[13 * 3 * 3 * 2 + 1] == characterWrappedData[0].textPositions[13 * 3 * 3 * 2 + 1]);
            }
        }

        WHEN("line_wrap is set to NONE, and data is generated")
        {
            element.createAttribute("line_wrap", gim::WrapMode::NONE);

            std::vector<gim::RenderData> noneWrappedData = generator.generate(element);

            THEN("There is no wrapping at all")
            {
                REQUIRE(normalData[0].textPositions.size() > 0);
                REQUIRE(noneWrappedData[0].textPositions.size() > 0);

                CHECK_FALSE(normalData[0].textPositions[20 * 3 * 3 * 2 + 1] == noneWrappedData[0].textPositions[20 * 3 * 3 * 2 + 1]);
            }
        }
    }
}

SCENARIO("Text row alignment behavior can be controlled using the text_alignment attribute", "[logic]")
{
    GIVEN("A RenderDataGenerator with a font and texture registered, and render data without text alignment specified")
    {
        gim::Font font = loadFont("resources/fonts/LiberationSans-Regular.ttf");

        TextureInterfaceStub textureAdaptor({20, 20}, 30);
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;

        generator.registerFontStorage({font}, textureAdaptor);

        gim::Element element({"text"},
        {
            {"position", Vector2({0, 0})},
            {"size",     Vector2({100, 256})},
            {"text", std::string("This is a text which will span more than two lines")},
            {"text_size", 16},
            {"text_style", gim::TextStyle::NORMAL},
            {"font", font.name()}
        });

        std::vector<gim::RenderData> normalData = generator.generate(element);

        WHEN("text_alignment is set to LEFT")
        {
            element.createAttribute("text_alignment", gim::TextAlign::LEFT);

            std::vector<gim::RenderData> leftWrappedData = generator.generate(element);

            THEN("it is the same as without since it is the default")
            {
                CHECK(normalData[0].textPositions == leftWrappedData[0].textPositions);
            }

            THEN()
            {
                CHECK(fabs(leftWrappedData[0].textPositions[0 * 3 * 3 * 2] - leftWrappedData[0].textPositions[11 * 3 * 3 * 2]) < 5.0f);
                CHECK(fabs(leftWrappedData[0].textPositions[0 * 3 * 3 * 2] - leftWrappedData[0].textPositions[20 * 3 * 3 * 2]) < 5.0f);
            }
        }

        WHEN("text_alignment is set to RIGHT")
        {
            element.createAttribute("text_alignment", gim::TextAlign::RIGHT);

            std::vector<gim::RenderData> rightWrappedData = generator.generate(element);

            THEN()
            {
                CHECK(fabs(rightWrappedData[0].textPositions[10 * 3 * 3 * 2] - rightWrappedData[0].textPositions[19 * 3 * 3 * 2]) < 5.0f);
                CHECK(fabs(rightWrappedData[0].textPositions[10 * 3 * 3 * 2] - rightWrappedData[0].textPositions[27 * 3 * 3 * 2]) < 5.0f);
            }
        }

        WHEN("text_alignment is set to CENTER")
        {
            element.createAttribute("text_alignment", gim::TextAlign::CENTER);

            std::vector<gim::RenderData> centerWrappedData = generator.generate(element);

            THEN()
            {
                CHECK_FALSE(fabs(centerWrappedData[0].textPositions[0 * 3 * 3 * 2] - centerWrappedData[0].textPositions[11 * 3 * 3 * 2]) < 5.0f);
                CHECK_FALSE(fabs(centerWrappedData[0].textPositions[0 * 3 * 3 * 2] - centerWrappedData[0].textPositions[20 * 3 * 3 * 2]) < 5.0f);
                CHECK_FALSE(fabs(centerWrappedData[0].textPositions[10 * 3 * 3 * 2] - centerWrappedData[0].textPositions[19 * 3 * 3 * 2]) < 5.0f);
                CHECK_FALSE(fabs(centerWrappedData[0].textPositions[10 * 3 * 3 * 2] - centerWrappedData[0].textPositions[27 * 3 * 3 * 2]) < 5.0f);
            }
        }
    }
}

SCENARIO("text_borders can be used to control position and wrap position of text", "[logic]")
{
    GIVEN("A RenderDataGenerator with a font and texture registered, and render data without text clipping disabled")
    {
        gim::Font font = loadFont("resources/fonts/LiberationSans-Regular.ttf");

        TextureInterfaceStub textureAdaptor({20, 20}, 30);
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;

        generator.registerFontStorage({font}, textureAdaptor);

        gim::Element element({"text"},
        {
            {"position", Vector2({10, 20})},
            {"size",     Vector2({100, 30})},
            {"text", std::string("This is a text which is somewhat long so that it would span many lines and that is a good thing for the test.")},
            {"text_size", 16},
            //{"text_borders", Rectangle({{5, 15}, {25, 35}})},
            {"font", font.name()},
        });

        WHEN("the default value is used")
        {
            std::vector<gim::RenderData> defaultData = generator.generate(element);

            THEN("the text generated is limited by the position and size of the element")
            {
                REQUIRE(defaultData[0].textPositions.size() > 0);

                bool allInRange = true;
                for(int32_t i = 0; i < defaultData[0].textPositions.size(); i += 3)
                {
                    float x = defaultData[0].textPositions[i + 0];   
                    float y = defaultData[0].textPositions[i + 1];   
                    float z = defaultData[0].textPositions[i + 2];   

                    if(x < 8.0f || x > 110.0f || y < 18.0f)
                    {
                        FAIL("failed for index " <<  std::to_string(i / 3) << " with x y z: " << std::to_string(x) << " " << std::to_string(y) << " " << std::to_string(z));
                        allInRange = false;
                    }
                }

                CHECK(allInRange);
            }
        }

        WHEN("the text_borders attribute is set to a region")
        {
            element.createAttribute("text_borders", Rectangle({{40, 70}, {150, 10}}));
            std::vector<gim::RenderData> textBordersData = generator.generate(element);

            THEN("the text generated is limited by the position and size of the text_borders")
            {
                REQUIRE(textBordersData[0].textPositions.size() > 0);

                bool allInRange = true;
                for(int32_t i = 0; i < textBordersData[0].textPositions.size(); i += 3)
                {
                    float x = textBordersData[0].textPositions[i + 0];   
                    float y = textBordersData[0].textPositions[i + 1];   
                    float z = textBordersData[0].textPositions[i + 2];   

                    if(x < 48.0f || x > 202.0f || y < 90.0f)
                    {
                        FAIL("failed for index " <<  std::to_string(i / 3) << " with x y z: " << std::to_string(x) << " " << std::to_string(y) << " " << std::to_string(z));
                        allInRange = false;
                    }
                }

                CHECK(allInRange);
            }
        }
    }
}

SCENARIO("By default, the RenderData contains text clip rectangles but these can be switched off", "[logic]")
{
    GIVEN("A RenderDataGenerator with a font and texture registered, and render data without text clipping disabled")
    {
        gim::Font font = loadFont("resources/fonts/LiberationSans-Regular.ttf");

        TextureInterfaceStub textureAdaptor({20, 20}, 30);
        gim::RenderDataGenerator<Vec2Adaptor, RectangleAdaptor, ColorAdaptor> generator;

        generator.registerFontStorage({font}, textureAdaptor);

        gim::Element element({"text"},
        {
            {"position", Vector2({10, 20})},
            {"size",     Vector2({30, 40})},
            {"text", std::string("This is a text")},
            {"text_size", 16},
            {"text_borders", Rectangle({{5, 15}, {25, 35}})},
            {"font", font.name()},
        });

        std::vector<gim::RenderData> normalData = generator.generate(element);

        WHEN("text_clipping is set to true")
        {
            element.createAttribute("text_clipping", true);

            std::vector<gim::RenderData> textClippedData = generator.generate(element);

            THEN("it is the same as without since it is the default")
            {
                REQUIRE(normalData[0].clipRectangle != nullptr);
                REQUIRE(textClippedData[0].clipRectangle != nullptr);
                CHECK(normalData[0].clipRectangle->xStart == textClippedData[0].clipRectangle->xStart);
                CHECK(normalData[0].clipRectangle->yStart == textClippedData[0].clipRectangle->yStart);
                CHECK(normalData[0].clipRectangle->width == textClippedData[0].clipRectangle->width);
                CHECK(normalData[0].clipRectangle->height == textClippedData[0].clipRectangle->height);
            }

            THEN("it is positioned correctly in regards to the text_borders")
            {
                REQUIRE(normalData[0].clipRectangle != nullptr);
                auto rectangle = *normalData[0].clipRectangle;
                CHECK(rectangle.xStart == Approx(15.0f));
                CHECK(rectangle.yStart == Approx(35.0f));
                CHECK(rectangle.width == Approx(25.0f));
                CHECK(rectangle.height == Approx(35.0f));
            }
        }

        WHEN("text_clipping is set to false")
        {
            element.createAttribute("text_clipping", false);

            std::vector<gim::RenderData> noClipData = generator.generate(element);

            THEN("it contains no clipping rectangle")
            {
                CHECK(noClipData[0].clipRectangle == nullptr);
            }
        }

        WHEN("text_z_position is set to something else than zero")
        {
            element.createAttribute("text_z_position", 10.0f);
            std::vector<gim::RenderData> data = generator.generate(element);

            THEN("the z values of text are all set to that value")
            {
                REQUIRE(data[0].textPositions.size() > 0);
                for(int32_t i = 0; i < data[0].textPositions.size(); i += 3)
                {
                    CHECK(data[0].textPositions[i + 2] == Approx(10.0f));
                }
            }
        }
    }
}
