#include <catch.hpp>
#include <gimgui/util/ref.hpp>

SCENARIO("makeRef can be used to create references from values", "[util]")
{
    GIVEN("an object")
    {
        int32_t number = 42;

        WHEN("makeRef is used to create a reference to that object and is subsequently changed")
        {
            auto reference = gim::makeRef(number);

            reference.get() = 31;

            THEN("the original changed as well")
            {
                CHECK(number == 31);
            }
        }
    }
}
