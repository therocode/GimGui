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

SCENARIO("Child elements can be appended or prepended or inserted at any index and a pointer to the new child is returned", "[data]")
{
    GIVEN("A parent element with some children")
    {
        gim::Element parent({"parent"});

        parent.append(gim::Element({"wrong"}));
        parent.append(gim::Element({"wrong"}));
        parent.append(gim::Element({"wrong"}));

        WHEN("a child is appended")
        {
            auto* child = parent.append(gim::Element({"right"}));

            THEN("the returned child is the newly created one")
            {
                CHECK(child->getTags().count("right") == 1);
            }
        }

        WHEN("a child is prepended")
        {
            auto* child = parent.prepend(gim::Element({"right"}));

            THEN("the returned child is the newly created one")
            {
                CHECK(child->getTags().count("right") == 1);
            }
        }

        WHEN("a child is inserted")
        {
            auto* child = parent.insert(2, gim::Element({"right"}));

            THEN("the returned child is the newly created one")
            {
                CHECK(child->getTags().count("right") == 1);
            }
        }
    }
}

SCENARIO("Child elements can be appended or prepended or inserted at any index and the order is retained, both const and non const access", "[data]")
{
    GIVEN("A parent element")
    {
        gim::Element parent({"parent"});
        const gim::Element& constParent = parent;

        WHEN("a single child is added")
        {
            parent.append(gim::Element({"child"}));

            THEN("that child is accessible at the first index")
            {
                CHECK(parent.getChildren()[0]->getTags().count("child") == 1);
                CHECK(constParent.getChildren() == parent.getChildren());
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
                CHECK(constParent.getChildren() == parent.getChildren());
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
                CHECK(constParent.getChildren() == parent.getChildren());
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
                CHECK(constParent.getChildren() == parent.getChildren());
            }
        }
    }
}

SCENARIO("Elements can be created with tags and attached to other elements, and then accessed with those tags in both a non const and a const way", "[data]")
{
    GIVEN("A parent element with children with unique tags appended to it")
    {
        gim::Element parent({"container"});
        const gim::Element& constParent = parent;

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

                CHECK(std::equal(child1List.begin(), child1List.end(), constParent.find({"child1"}).begin()));
                CHECK(std::equal(child2List.begin(), child2List.end(), constParent.find({"child2"}).begin()));
            }
        }

        WHEN("a tag belonging to many elements is used")
        {
            gim::ElementPtrList childList = parent.find({"element"});

            THEN("all of those elements are retreived")
            {
                CHECK(childList.size() == 5);
                CHECK(std::equal(childList.begin(), childList.end(), constParent.find({"element"}).begin()));
            }
        }

        WHEN("a tag belonging to no elements is used")
        {
            gim::ElementPtrList childList = parent.find({"invalid"});

            THEN("an empty list is returned")
            {
                CHECK(childList.empty());
                CHECK(std::equal(childList.begin(), childList.end(), constParent.find({"invalid"}).begin()));
            }
        }

        WHEN("two tags are used")
        {
            gim::ElementPtrList childList = parent.find({"health_bar", "gold_meter"});

            THEN("only elements with both tags are returned")
            {
                CHECK(childList.size() == 2);
                CHECK(std::equal(childList.begin(), childList.end(), constParent.find({"health_bar", "gold_meter"}).begin()));
            }
        }
    }
}

SCENARIO("Elements can be searched for recursively depending on tags", "[data]")
{
    //non const
    GIVEN("An element tree where a tag is existant on more than one level")
    {
        gim::Element root({"container"});
        const gim::Element& constRoot = root;

        auto subContainer1 = root.append(gim::Element({"sub_container", "target"}));
        subContainer1->append(gim::Element({"leaf_of_first", "target"}));
        subContainer1->append(gim::Element({"leaf_of_first"}));

        auto subContainer2 = root.append(gim::Element({"sub_container"}));
        subContainer2->append(gim::Element({"leaf_of_second", "target"}));
        subContainer2->append(gim::Element({"leaf_of_second"}));
        
        WHEN("that tag is searched for recursively")
        {
            gim::ElementPtrList childList = root.recursiveFind({"target"});

            THEN("all elements with that tag are returned")
            {
                CHECK(childList.size() == 3);
                CHECK(std::equal(childList.begin(), childList.end(), constRoot.recursiveFind({"target"}).begin()));
            }
        }
    }
}
