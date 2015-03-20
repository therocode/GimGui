#include <catch.hpp>
#include <gimgui/data/element.hpp>
#include <gimgui/data/variant.hpp>
#include <gimgui/util/getorfallback.hpp>
#include <unordered_map>

SCENARIO("getOrFallback can be used to access element attributes or return a fallback value if such an attribute doesn't exist", "[util]")
{
    GIVEN("An element with an attribute set")
    {
        gim::Element element({}, {{"gold", 42}});

        WHEN("a non-existing attribute is accessed with a fallback given")
        {
            int32_t value = getOrFallback(element, "non-existing", 234);

            THEN("the gotten value is the given fallback value")
            {
                CHECK(value == 234);
            }
        }

        WHEN("an existing attribute is accessed with a fallback given")
        {
            int32_t value = getOrFallback(element, "gold", 234);

            THEN("the gotten value is the stored value")
            {
                CHECK(value == 42);
            }
        }
    }
}

SCENARIO("getOrFallback can be used to access data from variant maps", "[util]")
{
    GIVEN("An unordered_map with variants")
    {
        std::unordered_map<std::string, gim::Variant> map = {{"number", 12}, {"string", std::string("text")} ,{"float", 1.0f}};

        WHEN("a non-existing attribute is accessed with a fallback given")
        {
            int32_t value = getOrFallback(map, "non-existing", 234);

            THEN("the gotten value is the given fallback value")
            {
                CHECK(value == 234);
            }
        }

        WHEN("an existing attribute is accessed with a fallback given")
        {
            int32_t number = getOrFallback(map, "number", 234);
            std::string text = getOrFallback(map, "string", std::string("hej"));
            float fNumber = getOrFallback(map, "float", 10.0f);

            THEN("the gotten value is the stored value")
            {
                CHECK(number == 12);
                CHECK(text == "text");
                CHECK(fNumber == 1.0f);
            }
        }
    }
}
