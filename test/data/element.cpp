#include "../catch.hpp"

#include <gimgui/data/element.hpp>

SCENARIO("Elements can be created with tags and attached to other elements, and then accessed with those tags", "[data]")
{
    GIVEN("A parent element with children with unique tags appended to it")
    {
        gim::Element parent({"container"});

        parent.append(gim::Element({"child1"}));
        parent.append(gim::Element({"child2"}));

        WHEN("Elements of those tags are requested")
        {
            gim::ElementPtrList child1List = parent.find({"child1"});
            gim::ElementPtrList child2List = parent.find({"child2"});

            THEN("Those elements are returned")
            {
                CHECK(child1List.size() == 1);
                CHECK(child2List.size() == 1);
            }
        }
    }
}

SCENARIO("More than one element can share the same tag, then they are all retreivable using that tag", "[data]")
{
    GIVEN("A parent element with children with identical tags appended to it")
    {
        gim::Element parent({"container"});

        parent.append(gim::Element({"child"}));
        parent.append(gim::Element({"child"}));
        parent.append(gim::Element({"child"}));

        WHEN("Elements of that tag are requested")
        {
            gim::ElementPtrList childList = parent.find({"child"});

            THEN("Those elements are returned")
            {
                CHECK(childList.size() == 3);
            }
        }
    }
}

SCENARIO("Accessing with tags that are not associated with any children returns nothing", "[data]")
{
    GIVEN("A parent element with a child with a tag appended to it")
    {
        gim::Element parent({"container"});

        parent.append(gim::Element({"child"}));

        WHEN("Elements of another tag are requested")
        {
            gim::ElementPtrList childList = parent.find({"invalid"});

            THEN("Nothing is returned")
            {
                CHECK(childList.empty());
            }
        }
    }
}

SCENARIO("Multiple tags can be given to elements, and when a tag is requested, the union of the elements containing that tag is returned", "[data]")
{
    GIVEN("A parent element with children associated with different tags")
    {
        gim::Element parent({"container"});

        parent.append(gim::Element({"child", "health_meter"}));
        parent.append(gim::Element({"child", "health_meter"}));
        parent.append(gim::Element({"child", "extra"}));
        parent.append(gim::Element({"child", "animated", "health_meter"}));

        WHEN("Elements of the 'health_meter' tag are requested")
        {
            gim::ElementPtrList childList = parent.find({"health_meter"});

            THEN("The right amount is returned")
            {
                CHECK(childList.size() == 3);
            }
        }
    }
}

SCENARIO("When finding elements using two tags in the query, only elements associated with both tags are returned", "[data]")
{
    GIVEN("A parent element with children associated with different tags")
    {
        gim::Element parent({"container"});

        parent.append(gim::Element({"child"}));
        parent.append(gim::Element({"child", "health_meter"}));
        parent.append(gim::Element({"health_meter"}));

        WHEN("Elements of the 'health_meter' and the 'child' tag are requested")
        {
            gim::ElementPtrList childList = parent.find({"child", "health_meter"});

            THEN("Only one is returned")
            {
                CHECK(childList.size() == 1);
            }
        }
    }
}

SCENARIO("When finding elements using an empty tag set, nothing is returned", "[data]")
{
    GIVEN("A parent element with a child")
    {
        gim::Element parent({"container"});

        parent.append(gim::Element({"child"}));

        WHEN("An empty tag set is used for requesting")
        {
            gim::ElementPtrList childList = parent.find(gim::TagSet());

            THEN("An empty list is returned")
            {
                CHECK(childList.empty());
            }
        }
    }
}
