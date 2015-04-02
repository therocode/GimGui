#include <catch.hpp>
#include <gimgui/logic/utf8decoder.hpp>

SCENARIO("A utf8decoder can be used to decode UTF-8 strings into unicode code points", "[logic]")
{
    GIVEN("a utf8decoder")
    {
        gim::Utf8Decoder decoder;

        WHEN("the decoder decodes first order UTF-8 values")
        {
            std::vector<uint32_t> codePoints = decoder.decode("bg");

            THEN("the values are correct")
            {
                REQUIRE(codePoints.size() == 2);
                CHECK(codePoints[0] == 98);
                CHECK(codePoints[1] == 103);
            }
        }

        WHEN("the decoder decodes second order UTF-8 values")
        {
            std::vector<uint32_t> codePoints = decoder.decode("Ö\U0000014E");

            THEN("the values are correct")
            {
                REQUIRE(codePoints.size() == 2);
                CHECK(codePoints[0] == 214);
                CHECK(codePoints[1] == 334);
            }
        }

        WHEN("the decoder decodes third order UTF-8 values")
        {
            std::vector<uint32_t> codePoints = decoder.decode("\U00004000\U00004500");

            THEN("the values are correct")
            {
                REQUIRE(codePoints.size() == 2);
                CHECK(codePoints[0] == 16384);
                CHECK(codePoints[1] == 17664);
            }
        }

        WHEN("the decoder decodes fourth order UTF-8 values")
        {
            std::vector<uint32_t> codePoints = decoder.decode("\U00080000\U00098765");

            THEN("the values are correct")
            {
                REQUIRE(codePoints.size() == 2);
                CHECK(codePoints[0] == 524288);
                CHECK(codePoints[1] == 624485);
            }
        }

        WHEN("the decoder decodes mixed order UTF-8 values")
        {
            std::vector<uint32_t> codePoints = decoder.decode("\U00004000b\U00098765Ö");

            THEN("the values are correct")
            {
                REQUIRE(codePoints.size() == 4);
                CHECK(codePoints[0] == 16384);
                CHECK(codePoints[1] == 98);
                CHECK(codePoints[2] == 624485);
                CHECK(codePoints[3] == 214);
            }
        }
    }
}
