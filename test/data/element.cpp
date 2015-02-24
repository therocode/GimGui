#include "../catch.hpp"

#include <gimgui/data/element.hpp>

SCENARIO("Elements can be created with tags and attached to other elements, and then accessed with those tags in both a non const and a const way", "[data]")
{
    //non const
    GIVEN("A parent element with children with unique tags appended to it")
    {
        gim::Element parent({"container"});

        parent.append(gim::Element({"child1", "element", "health_bar", "gold_meter"}));
        parent.append(gim::Element({"child2", "element", "gold_meter"}));
        parent.append(gim::Element({"child3", "element", "gold_meter"}));
        parent.append(gim::Element({"child4", "element", "health_bar"}));
        parent.append(gim::Element({"child5", "element", "health_bar", "gold_meter"}));

        WHEN("a unique tag is used")
        {
            gim::ElementPtrList child1List = parent.find({"child1"});
            gim::ElementPtrList child2List = parent.find({"child2"});

            THEN("only one element is returned")
            {
                CHECK(child1List.size() == 1);
                CHECK(child2List.size() == 1);
            }
        }

        WHEN("a tag belonging to many elements is used")
        {
            gim::ElementPtrList childList = parent.find({"element"});

            THEN("all of those elements are retreived")
            {
                CHECK(childList.size() == 5);
            }
        }

        WHEN("a tag belonging to no elements is used")
        {
            gim::ElementPtrList childList = parent.find({"invalid"});

            THEN("an empty list is returned")
            {
                CHECK(childList.empty());
            }
        }

        WHEN("two tags are used")
        {
            gim::ElementPtrList childList = parent.find({"health_bar", "gold_meter"});

            THEN("only elements with both tags are returned")
            {
                CHECK(childList.size() == 2);
            }
        }
    }
    
    //const
    GIVEN("A const parent element with children with unique tags appended to it")
    {
        gim::Element nonConstParent({"container"});

        nonConstParent.append(gim::Element({"child1", "element", "health_bar", "gold_meter"}));
        nonConstParent.append(gim::Element({"child2", "element", "gold_meter"}));
        nonConstParent.append(gim::Element({"child3", "element", "gold_meter"}));
        nonConstParent.append(gim::Element({"child4", "element", "health_bar"}));
        nonConstParent.append(gim::Element({"child5", "element", "health_bar", "gold_meter"}));

        const gim::Element& parent = nonConstParent;

        WHEN("a unique tag is used")
        {
            gim::ElementConstPtrList child1List = parent.find({"child1"});
            gim::ElementConstPtrList child2List = parent.find({"child2"});

            THEN("only one element is returned")
            {
                CHECK(child1List.size() == 1);
                CHECK(child2List.size() == 1);
            }
        }

        WHEN("a tag belonging to many elements is used")
        {
            gim::ElementConstPtrList childList = parent.find({"element"});

            THEN("all of those elements are retreived")
            {
                CHECK(childList.size() == 5);
            }
        }

        WHEN("a tag belonging to no elements is used")
        {
            gim::ElementConstPtrList childList = parent.find({"invalid"});

            THEN("an empty list is returned")
            {
                CHECK(childList.empty());
            }
        }

        WHEN("two tags are used")
        {
            gim::ElementConstPtrList childList = parent.find({"health_bar", "gold_meter"});

            THEN("only elements with both tags are returned")
            {
                CHECK(childList.size() == 2);
            }
        }
    }
}
