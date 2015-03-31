#include <catch.hpp>
#include <gimgui/logic/fonttexturecache.hpp>
#include <gimgui/data/font.hpp>
#include <helpers/textureinterfacestub.hpp>
#include <fstream>

SCENARIO("FontTextureCache can be used to store and access glyph texture coordinates","[util]")
{
    GIVEN("a FontTextureCache setup with a texture interface, and a font")
    {
        TextureInterfaceStub textureInterface;
        std::ifstream file("resources/fonts/LiberationSans-Regular.ttf", std::ios::binary);
        gim::Font font(file);
        gim::FontTextureCache textureCache(textureInterface);

        WHEN("the coordinates of a non-existing glyph is accessed")
        {
             auto texCoordsPointer = textureCache.glyphCoords('G', 16);
             
             THEN("the returned pointer is nullptr")
             {
                 CHECK(texCoordsPointer == nullptr);
             }
        }

        WHEN("a glyph bitmap with a size is added and subequently accessed")
        {
            font.resize(16);
            auto glyph = font.generateGlyph('G');
            REQUIRE(glyph != nullptr);
            auto texCoords = textureCache.add(*glyph);
            auto texCoordsPointer = textureCache.glyphCoords('G', 16);

            THEN("the initial coordinates and subsequent coordinates match")
            {
                REQUIRE(texCoordsPointer != nullptr);
                CHECK(texCoords == *texCoordsPointer);
            }
        }

        WHEN("a glyph is added and a glyph of the same size and different glyph or vice versa")
        {
            font.resize(10);
            auto glyph = font.generateGlyph('A');
            REQUIRE(glyph != nullptr);
            textureCache.add(*glyph);
            auto sameGlyph = textureCache.glyphCoords('A', 11);
            auto sameSize = textureCache.glyphCoords('B', 11);

            THEN("nullptrs are returned")
            {
                CHECK(sameGlyph == nullptr);
                CHECK(sameSize == nullptr);
            }
        }
    }
}
