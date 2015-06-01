#include <catch.hpp>
#include <gimgui/logic/fonttexturecache.hpp>
#include <gimgui/data/font.hpp>
#include <helpers/textureinterfacestub.hpp>
#include <fstream>

SCENARIO("FontTextureCache can be used to store and access glyph texture coordinates","[logic]")
{
    GIVEN("a FontTextureCache setup with a texture interface, and a font")
    {
        TextureInterfaceStub textureInterface({5, 5});
        std::ifstream file("resources/fonts/LiberationSans-Regular.ttf", std::ios::binary);
        gim::Font font(file);
        gim::internal::FontTextureCache textureCache(textureInterface);

        WHEN("the coordinates of a non-existing glyph is accessed")
        {
             auto texCoordsPointer = textureCache.glyphCoords('G', 16, 0);
             
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
            auto texCoords = textureCache.add(*glyph, 0);
            auto texCoordsPointer = textureCache.glyphCoords('G', 16, 0);

            THEN("the initial coordinates and subsequent coordinates match")
            {
                REQUIRE(texCoordsPointer != nullptr);
                CHECK(texCoords == *texCoordsPointer);
            }
        }

        WHEN("a glyph is added and a glyph of the same size but different glyph are accessed or vice versa")
        {
            font.resize(10);
            auto glyph = font.generateGlyph('A');
            REQUIRE(glyph != nullptr);
            textureCache.add(*glyph, 0);
            auto sameGlyph = textureCache.glyphCoords('A', 11, 0);
            auto sameSize = textureCache.glyphCoords('B', 11, 0);

            THEN("nullptrs are returned")
            {
                CHECK(sameGlyph == nullptr);
                CHECK(sameSize == nullptr);
            }
        }
    }
}

SCENARIO("FontTextureCache can be used to access coordinates of a whole-filled quad","[logic]")
{
    GIVEN("a FontTextureCache setup with a texture interface")
    {
        TextureInterfaceStub textureInterface({10, 10});
        gim::internal::FontTextureCache textureCache(textureInterface);

        WHEN("the coordinates of the solid quad are accessed")
        {
             auto texCoords = textureCache.solidCoords();
             
             THEN("the returned texcoords are sane")
             {
                 CHECK(texCoords.xStart >= 0.0f);
                 CHECK(texCoords.yStart >= 0.0f);
                 CHECK(texCoords.xEnd >= 0.0f);
                 CHECK(texCoords.yEnd >= 0.0f);
                 CHECK(texCoords.xStart <= 1.0f);
                 CHECK(texCoords.yStart <= 1.0f);
                 CHECK(texCoords.xEnd <= 1.0f);
                 CHECK(texCoords.yEnd <= 1.0f);
             }
        }
    }
}
