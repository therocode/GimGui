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

SCENARIO("Element hierarchies can be created inline using the constructor of element", "[data]")
{
    GIVEN("an inline created hierarchy")
    {
        gim::Element root({"root"}, 
        {
            gim::Element({"container", "spec"},
            {
                gim::Element({"leaf1"})
            }),
            gim::Element({"container"},
            {
                gim::Element({"leaf2"}),
                gim::Element({"leaf2"}),
                gim::Element({"leaf2"})
            })
        });

        WHEN("elements are accessed")
        {
            THEN("they are retreived according to the hierarchy")
            {
                auto containers = root.findChildren({"container"});
                auto firstContainerChildren = containers[0]->findChildren({"leaf1"});
                auto secondContainerChildren = containers[1]->findChildren({"leaf2"});

                REQUIRE(containers[1]->parent() != nullptr);

                REQUIRE(containers.size() == 2);
                CHECK(firstContainerChildren.size() == 1);
                CHECK(secondContainerChildren.size() == 3);
                CHECK(containers[1]->parent()->tags().count("root") == 1);
                CHECK(firstContainerChildren[0]->parent()->tags().count("container") == 1);
                CHECK(secondContainerChildren[2]->parent()->parent()->tags().count("root") == 1);
            }
        }
    }
}

SCENARIO("Elements can be copied and moved")
{
    GIVEN("an element with a parent, some attributes and some children with attributes")
    {
        gim::Element root({"root"});

        gim::Element& element = root.append(gim::Element({"element"}));
        const gim::Element& constElement = element; 

        gim::Element& child1 = element.append(gim::Element({"child"}));
        gim::Element& child2 = element.append(gim::Element({"child"}));

        root.createAttribute("name", std::string("the_root"));
        element.createAttribute("name", std::string("middle"));
        element.createAttribute("star_amount", 10);
        child1.createAttribute("name", std::string("child_1"));
        child1.createAttribute("child_number", 1);
        child2.createAttribute("name", std::string("child_2"));
        child2.createAttribute("child_number", 2);

        WHEN("the element is copied using the copy constructor, and any modifications only apply to the copy and unmodified values are the same value as the original")
        {
            gim::Element copy = constElement;

            copy.setAttribute("star_amount", 5);

            copy.children()[0]->setAttribute("child_number", 105);

            THEN("the copy has a copy of all tags and the parent is set to nullptr")
            {
                CHECK(copy.tags() == element.tags());
                CHECK(copy.parent() == nullptr);
            }

            THEN("copied attributes are the correct value")
            {
                CHECK(copy.getAttribute<std::string>("name") == "middle");
            }

            THEN("modified values do not modify the original")
            {
                CHECK(element.getAttribute<int32_t>("star_amount") == 10);
            }

            THEN("copied unmodified children have the same value as the original")
            {
                CHECK(copy.children()[1]->tags() == element.children()[1]->tags());
                CHECK(copy.children()[1]->getAttribute<int32_t>("child_number") == 2);
            }
            
            THEN("copied modified children do not modify the original")
            {
                CHECK(element.children()[0]->getAttribute<int32_t>("child_number") == 1);
            }
        }

        WHEN("the element is copied using copy assignment, and any modifications only apply to the copy and unmodified values are the same value as the original")
        {
            gim::Element copy;
            copy = constElement;

            copy.setAttribute("star_amount", 5);

            copy.children()[0]->setAttribute("child_number", 105);

            THEN("the copy has a copy of all tags and the parent is set to nullptr")
            {
                CHECK(copy.tags() == element.tags());
                CHECK(copy.parent() == nullptr);
            }

            THEN("copied attributes are the correct value")
            {
                CHECK(copy.getAttribute<std::string>("name") == "middle");
            }

            THEN("modified values do not modify the original")
            {
                CHECK(element.getAttribute<int32_t>("star_amount") == 10);
            }

            THEN("copied unmodified children have the same value as the original")
            {
                CHECK(copy.children()[1]->tags() == element.children()[1]->tags());
                CHECK(copy.children()[1]->getAttribute<int32_t>("child_number") == 2);
            }
            
            THEN("copied modified children do not modify the original")
            {
                CHECK(element.children()[0]->getAttribute<int32_t>("child_number") == 1);
            }
        }

        WHEN("the element is moved using the move constructor")
        {
            gim::Element moved = std::move(element);

            THEN("the moved element has the right tags and parent")
            {
                CHECK(moved.tags() == gim::TagSet({"element"}));
                CHECK(moved.parent() == &root);
            }

            THEN("the moved attributes are the correct value")
            {
                CHECK(moved.getAttribute<std::string>("name") == "middle");
                CHECK(moved.getAttribute<int32_t>("star_amount") == 10);
            }

            THEN("the moved children have the same values as the original")
            {
                CHECK(moved.children()[0]->tags() == gim::TagSet({"child"}));
                CHECK(moved.children()[0]->getAttribute<int32_t>("child_number") == 1);
                CHECK(moved.children()[1]->tags() == gim::TagSet({"child"}));
                CHECK(moved.children()[1]->getAttribute<int32_t>("child_number") == 2);
            }
        }

        WHEN("the element is moved using move assignment")
        {
            gim::Element moved;
            moved = std::move(element);

            THEN("the moved element has the right tags and parent")
            {
                CHECK(moved.tags() == gim::TagSet({"element"}));
                CHECK(moved.parent() == &root);
            }

            THEN("the moved attributes are the correct value")
            {
                CHECK(moved.getAttribute<std::string>("name") == "middle");
                CHECK(moved.getAttribute<int32_t>("star_amount") == 10);
            }

            THEN("the moved children have the same values as the original")
            {
                CHECK(moved.children()[0]->tags() == gim::TagSet({"child"}));
                CHECK(moved.children()[0]->getAttribute<int32_t>("child_number") == 1);
                CHECK(moved.children()[1]->tags() == gim::TagSet({"child"}));
                CHECK(moved.children()[1]->getAttribute<int32_t>("child_number") == 2);
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
            gim::ElementPtrList child1List = parent.findChildren({"child1"});
            gim::ElementPtrList child2List = parent.findChildren({"child2"});

            THEN("only one element is returned")
            {
                CHECK(child1List.size() == 1);
                CHECK(child2List.size() == 1);

                CHECK(std::equal(child1List.begin(), child1List.end(), constParent.findChildren({"child1"}).begin()));
                CHECK(std::equal(child2List.begin(), child2List.end(), constParent.findChildren({"child2"}).begin()));
            }
        }

        WHEN("a tag belonging to many elements is used")
        {
            gim::ElementPtrList childList = parent.findChildren({"element"});

            THEN("all of those elements are retreived")
            {
                CHECK(childList.size() == 5);
                CHECK(std::equal(childList.begin(), childList.end(), constParent.findChildren({"element"}).begin()));
            }
        }

        WHEN("a tag belonging to no elements is used")
        {
            gim::ElementPtrList childList = parent.findChildren({"invalid"});

            THEN("an empty list is returned")
            {
                CHECK(childList.empty());
                CHECK(std::equal(childList.begin(), childList.end(), constParent.findChildren({"invalid"}).begin()));
            }
        }

        WHEN("two tags are used")
        {
            gim::ElementPtrList childList = parent.findChildren({"health_bar", "gold_meter"});

            THEN("only elements with both tags are returned")
            {
                CHECK(childList.size() == 2);
                CHECK(std::equal(childList.begin(), childList.end(), constParent.findChildren({"health_bar", "gold_meter"}).begin()));
            }
        }
    }
}

SCENARIO("Parents of elements can be accessed")
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

    GIVEN("a structure of elements")
    {
        gim::TagSet NoTags;

        gim::Element root({"root"},
        {
            gim::Element(NoTags,
            {
                gim::Element(NoTags)
            }),
            gim::Element({"grandfather"},
            {
                gim::Element(NoTags),
                gim::Element({"parent"}, 
                {
                    gim::Element({"innermost"})
                }),
                gim::Element(NoTags)
            })
        });

        WHEN("the parent structure from the innermost element is checked")
        {
            THEN("it is correct")
            {
                const gim::Element* innermost = root.recursiveFindChildren({"innermost"})[0];

                CHECK(innermost->parent() == root.recursiveFindChildren({"parent"})[0]);
                CHECK(innermost->parent()->parent() == root.recursiveFindChildren({"grandfather"})[0]);
                CHECK(innermost->parent()->parent()->parent() == &root);
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
            gim::ElementPtrList childList = root.recursiveFindChildren({"target"});

            THEN("all elements with that tag are returned")
            {
                CHECK(childList.size() == 3);
                CHECK(std::equal(childList.begin(), childList.end(), constRoot.recursiveFindChildren({"target"}).begin()));
            }
        }
    }
}

SCENARIO("Element attributes can be created, accessed, modified and deleted", "[data]")
{
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

SCENARIO("Elements in a hierarchy can be detached to remove them from the hierarchy", "[data]")
{
    GIVEN("an inline created hierarchy")
    {
        gim::Element root({"root"}, 
        {
            gim::Element({"container", "spec"},
            {
                gim::Element({"leaf1"})
            }),
            gim::Element({"container"},
            {
                gim::Element({"leaf2"}),
                gim::Element({"leaf2"}),
                gim::Element({"leaf2"})
            })
        });

        WHEN("an element is detached")
        {
            gim::Element detached = root.detachChild(1);

            THEN("previous owner no longer has that child")
            {
                CHECK(root.children().size() == 1);
                CHECK(root.recursiveFindChildren({"leaf2"}).size() == 0);
            }

            THEN("the other children of the previous owner are intact")
            {
                CHECK(root.children()[0]->tags().count("container") == 1);
                CHECK(root.recursiveFindChildren({"leaf1"}).size() == 1);
            }

            THEN("the detached element has no parent")
            {
                CHECK(detached.parent() == nullptr);
            }

            THEN("the detached element's children are intact")
            {
                CHECK(detached.children().size() == 3);
                CHECK(detached.findChildren({"leaf2"}).size() == 3);
            }
        }
    }
}
