#include <catch.hpp>
#include <gimgui/util/resolve.hpp>

SCENARIO("The resolve function can be used to resolve a value into another based on a list of criteria", "[util]")
{
    GIVEN("a value to be resolved")
    {
        int32_t three = 3;

        WHEN("the resolve function is used")
        {
            std::string result1 = gim::resolve(three, {{1, std::string("one")}, {2, std::string("two")}, {3, std::string("three")}}, std::string("zero"));
            std::string result2 = gim::resolve<std::string>(three, {{1, std::string("one")}, {2, std::string("two")}, {3, std::string("three")}});
            std::string defaultResult = gim::resolve(three, {{1, std::string("one")}, {2, std::string("two")}}, std::string("zero"));

            THEN("the right value is resolved")
            {
                CHECK(result1 == "three");
                CHECK(result2 == "three");
                CHECK(defaultResult == "zero");
            }
        }
    }
}
