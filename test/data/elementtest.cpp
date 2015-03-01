#include <catch.hpp>

#include <gimgui/data/element.hpp>
#include <helpers/point.hpp>

SCENARIO("Elements can be created with tags that can be accessed again", "[data]")
{
    GIVEN("An element with tags")
    {
        gim::Element element({"tag1", "tag2"});

        WHEN("the tags are accessed")
        {
            const gim::TagSet& tags = element.tags();

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
            gim::Element& child = parent.append(gim::Element({"right"}));

            THEN("the returned child is the newly created one")
            {
                CHECK(child.tags().count("right") == 1);
            }
        }

        WHEN("a child is prepended")
        {
            gim::Element& child = parent.prepend(gim::Element({"right"}));

            THEN("the returned child is the newly created one")
            {
                CHECK(child.tags().count("right") == 1);
            }
        }

        WHEN("a child is inserted")
        {
            gim::Element& child = parent.insert(2, gim::Element({"right"}));

            THEN("the returned child is the newly created one")
            {
                CHECK(child.tags().count("right") == 1);
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
                CHECK(parent.children()[0]->tags().count("child") == 1);
                CHECK(constParent.children() == parent.children());
            }
        }

        WHEN("two children are appended")
        {
            parent.append(gim::Element({"child1"}));
            parent.append(gim::Element({"child2"}));

            THEN("they are accessible in correct order")
            {
                REQUIRE(parent.children().size() == 2);
                CHECK(parent.children()[0]->tags().count("child1") == 1);
                CHECK(parent.children()[1]->tags().count("child2") == 1);
                CHECK(constParent.children() == parent.children());
            }
        }

        WHEN("two children are appended and one is prepended")
        {
            parent.append(gim::Element({"child1"}));
            parent.append(gim::Element({"child2"}));
            parent.prepend(gim::Element({"child3"}));

            THEN("they are accessible in correct order")
            {
                REQUIRE(parent.children().size() == 3);
                CHECK(parent.children()[0]->tags().count("child3") == 1);
                CHECK(parent.children()[1]->tags().count("child1") == 1);
                CHECK(parent.children()[2]->tags().count("child2") == 1);
                CHECK(constParent.children() == parent.children());
            }
        }

        WHEN("two children are appended and one is inserted in between")
        {
            parent.append(gim::Element({"child1"}));
            parent.append(gim::Element({"child2"}));
            parent.insert(1, gim::Element({"child3"}));

            THEN("they are accessible in correct order")
            {
                REQUIRE(parent.children().size() == 3);
                CHECK(parent.children()[0]->tags().count("child1") == 1);
                CHECK(parent.children()[1]->tags().count("child3") == 1);
                CHECK(parent.children()[2]->tags().count("child2") == 1);
                CHECK(constParent.children() == parent.children());
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

SCENARIO("Children added to an element can access the parent element")
{
    GIVEN("A parent element and a child element")
    {
        gim::Element parent({"parent"});
        const gim::Element& constParent = parent;

        WHEN("the parent is not added to any other element")
        {
            THEN("its parent is nullptr")
            {
                CHECK(parent.parent() == nullptr);
                CHECK(constParent.parent() == nullptr);
            }
        }

        WHEN("a child is added with append")
        {
            gim::Element& child = parent.append(gim::Element({"child"}));
            const gim::Element& constChild = child;

            THEN("the parent is set to the parent")
            {
                REQUIRE(child.parent() != nullptr);
                CHECK(child.parent()->tags().count("parent") != 0);
                REQUIRE(constChild.parent() != nullptr);
                CHECK(constChild.parent()->tags().count("parent") != 0);
            }
        }

        WHEN("a child is added with prepend")
        {
            gim::Element& child = parent.prepend(gim::Element({"child"}));
            const gim::Element& constChild = child;

            THEN("the parent is set to the parent")
            {
                REQUIRE(child.parent() != nullptr);
                CHECK(child.parent()->tags().count("parent") != 0);
                REQUIRE(constChild.parent() != nullptr);
                CHECK(constChild.parent()->tags().count("parent") != 0);
            }
        }

        WHEN("a child is added with insert")
        {
            gim::Element& child = parent.insert(0, gim::Element({"child"}));
            const gim::Element& constChild = child;

            THEN("the parent is set to the parent")
            {
                REQUIRE(child.parent() != nullptr);
                CHECK(child.parent()->tags().count("parent") != 0);
                REQUIRE(constChild.parent() != nullptr);
                CHECK(constChild.parent()->tags().count("parent") != 0);
            }
        }
    }
}

SCENARIO("Elements can be searched for recursively depending on tags", "[data]")
{
    GIVEN("An element tree where a tag is existant on more than one level")
    {
        gim::Element root({"container"});
        const gim::Element& constRoot = root;

        auto& subContainer1 = root.append(gim::Element({"sub_container", "target"}));
        subContainer1.append(gim::Element({"leaf_of_first", "target"}));
        subContainer1.append(gim::Element({"leaf_of_first"}));

        auto& subContainer2 = root.append(gim::Element({"sub_container"}));
        subContainer2.append(gim::Element({"leaf_of_second", "target"}));
        subContainer2.append(gim::Element({"leaf_of_second"}));
        
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

SCENARIO("Element attributes can be created, accessed, modified and deleted", "[data]")
{
    GIVEN("An element with an attribute created")
    {
        gim::Element element;
        element.createAttribute<int32_t>("gold");

        WHEN("the attribute's existence is checked for")
        {
            THEN("the attribute exists")
            {
                CHECK(element.hasAttribute("gold"));
                CHECK(element.hasAttribute<int32_t>("gold"));
                CHECK(element.findAttribute<int32_t>("gold") != nullptr);
            }
        }

        WHEN("a non-existant attribute is accessed")
        {
            THEN("nothing is returned")
            {
                CHECK_FALSE(element.hasAttribute("health"));
                CHECK_FALSE(element.hasAttribute<int32_t>("health"));
                CHECK(element.findAttribute<int32_t>("health") == nullptr);
            }
        }

        WHEN("an existing attribute is searched for with the wrong type")
        {
            THEN("nothing is returned")
            {
                CHECK(element.findAttribute<std::string>("gold") == nullptr);
                CHECK_FALSE(element.hasAttribute<std::string>("gold"));
            }
        }

        WHEN("the attribute is set to a value through the set function")
        {
            element.setAttribute("gold", 35);

            THEN("the value is stored")
            {
                CHECK(element.getAttribute<int32_t>("gold") == 35);
                CHECK(*element.findAttribute<int32_t>("gold") == 35);
            }
        }

        WHEN("the attribute is set to a value through the find function")
        {
            *element.findAttribute<int32_t>("gold") = 35;

            THEN("the value is stored")
            {
                CHECK(element.getAttribute<int32_t>("gold") == 35);
                CHECK(*element.findAttribute<int32_t>("gold") == 35);
            }
        }

        WHEN("a const version of the attribute is accessed")
        {
            THEN("the result is identical to non const access")
            {
                const gim::Element& constElement = element;
                CHECK(element.findAttribute<int32_t>("gold") == constElement.findAttribute<int32_t>("gold"));
                CHECK(element.findAttribute<int32_t>("health") == constElement.findAttribute<int32_t>("health"));
            }
        }

        WHEN("a non-existing attribute is deleted")
        {
            bool deleted = element.deleteAttribute("health");

            THEN("the returned value is false")
            {
                CHECK_FALSE(deleted);
            }
        }

        WHEN("the existing attribute is deleted")
        {
            bool deleted = element.deleteAttribute("gold");

            THEN("the attribute is no more there and the return value is true")
            {
                CHECK(deleted);
                CHECK_FALSE(element.hasAttribute("gold"));
                CHECK_FALSE(element.hasAttribute<int32_t>("gold"));
                CHECK(element.findAttribute<int32_t>("gold") == nullptr);
            }
        }
    }

    GIVEN("an element with an attribute created with a default value")
    {
        gim::Element element;
        element.createAttribute("gold", 42);

        WHEN("the attribute is retreived")
        {
            THEN("the correct value is gotten")
            {
                CHECK(element.getAttribute<int32_t>("gold") == 42);
                CHECK(*element.findAttribute<int32_t>("gold") == 42);
            }
        }
    }

    GIVEN("an element with attributes of different types created")
    {
        gim::Element element;
        element.createAttribute("gold", 10);
        element.createAttribute("name", std::string("xalle"));
        element.createAttribute("position", Point{35, 53});
        element.createAttribute("id_list", std::vector<uint32_t>{4u, 5u, 6u, 7u});

        WHEN("the values are accessed")
        {
            THEN("they are all correct")
            {
                CHECK(element.getAttribute<int32_t>("gold") == 10);
                CHECK(element.getAttribute<std::string>("name") == "xalle");
                CHECK(element.getAttribute<Point>("position") == Point({35, 53}));
                CHECK(element.getAttribute<std::vector<uint32_t>>("id_list") == std::vector<uint32_t>({4u, 5u, 6u, 7u}));
                CHECK(*element.findAttribute<int32_t>("gold") == 10);
                CHECK(*element.findAttribute<std::string>("name") == "xalle");
                CHECK(*element.findAttribute<Point>("position") == Point({35, 53}));
                CHECK(*element.findAttribute<std::vector<uint32_t>>("id_list") == std::vector<uint32_t>({4u, 5u, 6u, 7u}));
            }
        }
    }
}
