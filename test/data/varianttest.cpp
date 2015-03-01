#include <catch.hpp>

#include <gimgui/data/variant.hpp>

SCENARIO("Variants can be given values and accessed", "[data]")
{
    GIVEN("An empty variant")
    {
        gim::Variant variant;

        WHEN("nothing is done with it")
        {
            THEN("it is of type void")
            {
                CHECK(variant.isOfType<void>());
            }

            THEN("it is of the same type as another empty variant")
            {
                CHECK(variant.isSameTypeAs(gim::Variant()));
            }

            THEN("it is not of the same type as another non-empty variant")
            {
                CHECK_FALSE(variant.isSameTypeAs(gim::Variant(std::vector<float>())));
            }
        }
    }

    GIVEN("A variant with a value given initially")
    {
        gim::Variant variant(std::string("kalle"));
        
        WHEN("the type is accessed")
        {
            THEN("it is of the type of the given value")
            {
                CHECK(variant.isOfType<std::string>());
            }

            THEN("it is of the same type as another variant with the sama datatype stored")
            {
                CHECK(variant.isSameTypeAs(gim::Variant(std::string("ralle"))));
            }

            THEN("it is of a different type to a variant carrying another type of value")
            {
                CHECK_FALSE(variant.isSameTypeAs(gim::Variant(34)));
            }
        }

        WHEN("the value is accessed")
        {
            THEN("it is correct")
            {
                const std::string& value = variant.get<std::string>();
                CHECK(value == "kalle");
                const gim::Variant& constVariant = variant;
                CHECK(value == constVariant.get<std::string>());
            }
        }

        WHEN("the value is set to another value of the same type")
        {
            variant.set(std::string("ralle"));

            THEN("the value is set")
            {
                const std::string& value = variant.get<std::string>();
                CHECK(value == "ralle");
                const gim::Variant& constVariant = variant;
                CHECK(value == constVariant.get<std::string>());
            }
        }
    }
}

