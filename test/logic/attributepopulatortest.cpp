#include <catch.hpp>

#include <gimgui/data/element.hpp>
#include <gimgui/logic/attributepopulator.hpp>
#include <helpers/point.hpp>

SCENARIO("Attribute populator can be used to give attributes to elements in a hierarchy", "[logic]")
{
    GIVEN("An element hierarchy")
    {
        gim::TagSet NoTags;

        gim::Element root(NoTags, 
        {
            gim::Element(NoTags, 
            {
                gim::Element(NoTags),
                gim::Element(NoTags)
            })
        });

        WHEN("the root element is given to an attribute populator with some attribute configurations added using the adding functions")
        {
            gim::AttributePopulator populator;

            populator.addConfiguration("position", Point({0, 0}));
            populator.addConfiguration("name", std::string("default_name"));

            populator.populate(root);

            THEN("the attributes with correct value are added to all elements of the hierarchy")
            {
                REQUIRE(root.hasAttribute<Point>("position"));
                REQUIRE(root.hasAttribute<std::string>("name"));
                CHECK(root.getAttribute<Point>("position") == Point({0, 0}));
                CHECK(root.getAttribute<std::string>("name") == "default_name");
                const gim::Element& firstChild = *root.children().front();
                REQUIRE(firstChild.hasAttribute<Point>("position"));
                REQUIRE(firstChild.hasAttribute<std::string>("name"));
                CHECK(firstChild.getAttribute<Point>("position") == Point({0, 0}));
                CHECK(firstChild.getAttribute<std::string>("name") == "default_name");
                const gim::Element& firstLeaf = *firstChild.children().front();
                REQUIRE(firstLeaf.hasAttribute<Point>("position"));
                REQUIRE(firstLeaf.hasAttribute<std::string>("name"));
                CHECK(firstLeaf.getAttribute<Point>("position") == Point({0, 0}));
                CHECK(firstLeaf.getAttribute<std::string>("name") == "default_name");
                const gim::Element& secondLeaf = *firstChild.children().back();
                REQUIRE(secondLeaf.hasAttribute<Point>("position"));
                REQUIRE(secondLeaf.hasAttribute<std::string>("name"));
                CHECK(secondLeaf.getAttribute<Point>("position") == Point({0, 0}));
                CHECK(secondLeaf.getAttribute<std::string>("name") == "default_name");
            }
        }
    }

    GIVEN("An element with an attribute")
    {
        gim::Element element;
        element.createAttribute("name", std::string("kalle"));

        WHEN("an attribute populator with a configuration for the already existing attribute with the same type populates the element")
        {
            gim::AttributePopulator populator;

            populator.addConfiguration("name", std::string("ralle"));
            populator.populate(element);

            THEN("the already existing attribute is set to the value from the attribute populator")
            {
                CHECK(element.getAttribute<std::string>("name") == "ralle");
            }
        }
    }
}

SCENARIO("Attribute populator can be used to give attributes to elements in a hierarchy based on their tags", "[logic]")
{
    GIVEN("An element hierarchy with tags")
    {
        gim::Element root(gim::TagSet(), 
        {
            gim::Element({"label"}),
            gim::Element({"image"}),
            gim::Element({"label", "image"})
        });

        WHEN("a populator populates the hierarchy with configurations specific to certain tags")
        {
            gim::AttributePopulator populator;

            populator.addConfiguration("text", std::string("default_text"), {"label"});
            populator.addConfiguration("image_id", 0, {"image"});
            populator.addConfiguration("position", Point({0, 0}), {"image", "label"});

            populator.populate(root);

            THEN("relevant attributes are only given to the correct elements")
            {
                CHECK_FALSE(root.hasAttribute("text"));
                CHECK_FALSE(root.hasAttribute("image_id"));
                CHECK_FALSE(root.hasAttribute("position"));
                CHECK(root.children()[0]->hasAttribute("text"));
                CHECK_FALSE(root.children()[0]->hasAttribute("image_id"));
                CHECK(root.children()[0]->hasAttribute("position"));
                CHECK_FALSE(root.children()[1]->hasAttribute("text"));
                CHECK(root.children()[1]->hasAttribute("image_id"));
                CHECK(root.children()[1]->hasAttribute("position"));
                CHECK(root.children()[2]->hasAttribute("text"));
                CHECK(root.children()[2]->hasAttribute("image_id"));
                CHECK(root.children()[2]->hasAttribute("position"));
            }
        }
    }
}
