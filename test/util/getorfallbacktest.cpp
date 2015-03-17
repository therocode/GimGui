#include <catch.hpp>
#include <gimgui/data/element.hpp>
#include <gimgui/util/getorfallback.hpp>

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
