#include "../catch.hpp"

#include <gimgui/data/element.hpp>

SCENARIO("Elements can be created with tags that can be accessed again", "[data]")
{
    GIVEN("An element with tags")
    {
        gim::Element element({"tag1", "tag2"});

        WHEN("the tags are accessed")
        {
            const gim::TagSet& tags = element.getTags();

            THEN("the proper tags are returned")
            {
                CHECK(tags.size() == 2);
                CHECK(tags.count("tag1") == 1);
                CHECK(tags.count("tag2") == 1);
            }
        }
    }
}

SCENARIO("Child elements can be appended or prepended or inserted at any index and the order is retained", "[data]")
{
    //non const
    GIVEN("A parent element")
    {
        gim::Element parent({"parent"});

        WHEN("a single child is added")
        {
            parent.append(gim::Element({"child"}));

            THEN("that child is accessible at the first index")
            {
                CHECK(parent.getChildren()[0]->getTags().count("child") == 1);
            }
        }

        WHEN("two children are appended")
        {
            parent.append(gim::Element({"child1"}));
            parent.append(gim::Element({"child2"}));

            THEN("they are accessible in correct order")
            {
                REQUIRE(parent.getChildren().size() == 2);
                CHECK(parent.getChildren()[0]->getTags().count("child1") == 1);
                CHECK(parent.getChildren()[1]->getTags().count("child2") == 1);
            }
        }

        WHEN("two children are appended and one is prepended")
        {
            parent.append(gim::Element({"child1"}));
            parent.append(gim::Element({"child2"}));
            parent.prepend(gim::Element({"child3"}));

            THEN("they are accessible in correct order")
            {
                REQUIRE(parent.getChildren().size() == 3);
                CHECK(parent.getChildren()[0]->getTags().count("child3") == 1);
                CHECK(parent.getChildren()[1]->getTags().count("child1") == 1);
                CHECK(parent.getChildren()[2]->getTags().count("child2") == 1);
            }
        }

        WHEN("two children are appended and one is inserted in between")
        {
            parent.append(gim::Element({"child1"}));
            parent.append(gim::Element({"child2"}));
            parent.insert(1, gim::Element({"child3"}));

            THEN("they are accessible in correct order")
            {
                REQUIRE(parent.getChildren().size() == 3);
                CHECK(parent.getChildren()[0]->getTags().count("child1") == 1);
                CHECK(parent.getChildren()[1]->getTags().count("child3") == 1);
                CHECK(parent.getChildren()[2]->getTags().count("child2") == 1);
            }
        }
    }

    //const
    GIVEN("A const parent element")
    {
        gim::Element nonConstParent({"parent"});
        const gim::Element& parent = nonConstParent;

        WHEN("a single child is added")
        {
            nonConstParent.append(gim::Element({"child"}));

            THEN("that child is accessible at the first index")
            {
                CHECK(parent.getChildren()[0]->getTags().count("child") == 1);
            }
        }

        WHEN("two children are appended")
        {
            nonConstParent.append(gim::Element({"child1"}));
            nonConstParent.append(gim::Element({"child2"}));

            THEN("they are accessible in correct order")
            {
                REQUIRE(parent.getChildren().size() == 2);
                CHECK(parent.getChildren()[0]->getTags().count("child1") == 1);
                CHECK(parent.getChildren()[1]->getTags().count("child2") == 1);
            }
        }

        WHEN("two children are appended and one is prepended")
        {
            nonConstParent.append(gim::Element({"child1"}));
            nonConstParent.append(gim::Element({"child2"}));
            nonConstParent.prepend(gim::Element({"child3"}));

            THEN("they are accessible in correct order")
            {
                REQUIRE(parent.getChildren().size() == 3);
                CHECK(parent.getChildren()[0]->getTags().count("child3") == 1);
                CHECK(parent.getChildren()[1]->getTags().count("child1") == 1);
                CHECK(parent.getChildren()[2]->getTags().count("child2") == 1);
            }
        }

        WHEN("two children are appended and one is inserted in between")
        {
            nonConstParent.append(gim::Element({"child1"}));
            nonConstParent.append(gim::Element({"child2"}));
            nonConstParent.insert(1, gim::Element({"child3"}));

            THEN("they are accessible in correct order")
            {
                REQUIRE(parent.getChildren().size() == 3);
                CHECK(parent.getChildren()[0]->getTags().count("child1") == 1);
                CHECK(parent.getChildren()[1]->getTags().count("child3") == 1);
                CHECK(parent.getChildren()[2]->getTags().count("child2") == 1);
            }
        }
    }
}

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
