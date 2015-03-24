#include <catch.hpp>
#include <fstream>
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
                std::ifstream file("test/resources/fonts/SourceSansPro-Regular.ttf", std::ios::binary);
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
