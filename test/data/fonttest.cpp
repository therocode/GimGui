#include <catch.hpp>
#include <fstream>
#include <algorithm>
#include <gimgui/data/font.hpp>

gim::Font loadFont(std::istream& in) //workaround due to Catch quirk. cannot construct stuff in CHECK_NOTHROW and CHECK_THROWS_AS for some reason
{
   return gim::Font(in);
}

SCENARIO("Fonts can be created from istreams. Invalid font data throw an exception", "[data]")
{
    GIVEN("nothing")
    {
        WHEN("a font is loaded from valid data")
        {
            THEN("no exception is thrown")
            {
                std::ifstream file("resources/fonts/LiberationSans-Regular.ttf", std::ios::binary);
                CHECK_NOTHROW(loadFont(file));
            }
        }

        WHEN("a font is loaded from invalid data")
        {
            THEN("an invalid font exception is thrown")
            {
                std::istringstream invalidData("asdfpasdkgask;gkas;ka;sdklfa;sdfjas;dfj");
                CHECK_THROWS_AS(loadFont(invalidData), gim::FontLoadException);

                std::istringstream emptyData;
                CHECK_THROWS_AS(loadFont(emptyData), gim::FontLoadException);
            }
        }

        
    }
}

SCENARIO("Fonts can be checked if they are freely scalable (vectorial) or not (bitmaps). For bitmaps, a list of available sizes can be gotten", "[data]")
{
    GIVEN("a vectorial and a bitmap font")
    {
        std::ifstream file1("resources/fonts/LiberationSans-Regular.ttf", std::ios::binary);
        gim::Font vectorial(file1);
        std::ifstream file2("resources/fonts/gohufont-14.pcf.gz", std::ios::binary);
        gim::Font bitmap(file2);

        WHEN("it is checked if they are freely scalable or not")
        {
            THEN("the returned value is correct")
            {
                CHECK(vectorial.isFreelyScalable());
                CHECK_FALSE(bitmap.isFreelyScalable());
            }
        }

        WHEN("the available sizes are accessed from the bitmap font")
        {
            THEN("they are correct")
            {
                CHECK(bitmap.availableSizes().size() == 1);
            }
        }
    }
}

SCENARIO("Fonts can be used to access various information", "[data]")
{
    GIVEN("a font")
    {
        std::ifstream file("resources/fonts/LiberationSans-Regular.ttf", std::ios::binary);
        gim::Font font(file);

        WHEN("the font family is accessed")
        {
            THEN("it matches the loaded file")
            {
                CHECK(font.family() == "Liberation Sanss");
            }
        }

        WHEN("the font style is accessed")
        {
            THEN("it matches the loaded file")
            {
                CHECK(font.style() == "Regular");
            }
        }

        WHEN("the font name is accessed")
        {
            THEN("it consists of the family and the style concatenated")
            {
                CHECK(font.name() == "Liberation Sans Regular");
            }
        }

        WHEN("the kerning of valid code points are accessed")
        {
            THEN("they are correct")
            {
                CHECK(font.kerning('A', 'V') == Approx(-1.0f));
            }
        }

        WHEN("the line spacing is accessed")
        {
            THEN("it is correct")
            {
                CHECK(font.lineSpacing() == Approx(18.0f));
            }
        }

        WHEN("the underline position is accessed")
        {
            THEN("it is correct")
            {
                CHECK(font.underlinePosition() == Approx(2.28125f));
            }
        }

        WHEN("the underline thickness is accessed")
        {
            THEN("it is correct")
            {
                CHECK(font.underlineThickness() == Approx(1.17188f));
            }
        }
    }
}

SCENARIO("Fonts can be resized", "[data]")
{
    GIVEN("a font with default size of 16, and a kerning value of it")
    {
        std::ifstream file("resources/fonts/LiberationSans-Regular.ttf", std::ios::binary);
        gim::Font font(file);
        float originalKerning = font.kerning('A', 'V') == -1.0f;

        WHEN("the font is sized up")
        {
            font.resize(50);

            THEN("the kerning of the same codepoint pair is 'bigger' (smaller in value)")
            {
                CHECK(originalKerning > font.kerning('A', 'V'));
            }
        }
    }
}

SCENARIO("Glyph bitmaps can be generated", "[data]")
{
    GIVEN("a font")
    {
        std::ifstream file("resources/fonts/LiberationSans-Regular.ttf", std::ios::binary);
        gim::Font font(file);

        WHEN("glyphs of existing codepoints such as A are generated")
        {
            std::unique_ptr<gim::Glyph> glyph = font.generateGlyph('A');

            THEN("the glyph contains non-zero data")
            {
                REQUIRE(glyph != nullptr);
                CHECK(glyph->size == font.size());
                CHECK(glyph->codePoint == 'A');
                CHECK(glyph->metrics.advance > 0.0f);
                CHECK(glyph->image.width > 0);
                CHECK(glyph->image.height > 0);
                CHECK(std::accumulate(glyph->image.pixels.begin(), glyph->image.pixels.end(), 0) > 0);
            }
        }

        WHEN("a glyph without graphics such as ' ' is generated")
        {
            std::unique_ptr<gim::Glyph> glyph = font.generateGlyph(' ');

            THEN("the glyph contains zero data")
            {
                REQUIRE(glyph != nullptr);
                CHECK(glyph->metrics.advance > 0.0f);
                CHECK(std::accumulate(glyph->image.pixels.begin(), glyph->image.pixels.end(), 0) == 0);
            }
        }

        WHEN("two glyphs are generated, one filled more than the other, such as '.' and 'g'")
        {
            std::unique_ptr<gim::Glyph> period = font.generateGlyph('.');
            std::unique_ptr<gim::Glyph> g = font.generateGlyph('g');

            THEN("the 'g' is more filled than '.'")
            {
                CHECK(std::accumulate(g->image.pixels.begin(), g->image.pixels.end(), 0) > 
                      std::accumulate(period->image.pixels.begin(), period->image.pixels.end(), 0));
            }
            THEN("the 'g' is larger than '.'")
            {
                CHECK(period->metrics.width < g->metrics.width);
                CHECK(period->metrics.height < g->metrics.height);
            }
        }

        WHEN("a glyph of a non-existing codepoint is generated")
        {
            THEN("a null pointer is returned")
            {
                CHECK(font.generateGlyph(-1) == nullptr);
            }
        }
    }
}
