#include <catch.hpp>
#include <gimgui/data/element.hpp>
#include <gimgui/logic/boundarypropagator.hpp>
#include <helpers/vector2.hpp>
#include <set>

SCENARIO("BoundaryPropagator propagates through the hierarchy based on which elements are within the boundary of the access point", "[logic]")
{
    GIVEN("a hierarchy of elements")
    {
        gim::TagSet NoTags;

            gim::Element root({"in"}, 
            {
                {"position", Vector2({0, 0})},
                {"size", Vector2({20, 20})}
            },
            {
                gim::Element({"in"},
                {
                    {"position", Vector2({8, 8})},
                    {"size", Vector2({5, 5})}
                },
                {
                    gim::Element({"not_in"},
                    {
                        {"position", Vector2({8, 8})},
                        {"size", Vector2({5, 5})}
                    },
                    {
                        gim::Element({"in"},
                        {
                            {"position", Vector2({-8, -8})},
                            {"size", Vector2({5, 5})}
                        })
                    }),
                    gim::Element({"in"},
                    {
                        {"position", Vector2({0, 0})},
                        {"size", Vector2({5, 5})}
                    })
                }),
                gim::Element({"in_second"}, 
                {
                    {"position", Vector2({11, 8})},
                    {"size", Vector2({5, 5})}
                })
            });

        WHEN("the hierarchy is given to a BoundaryPropagator with one point added")
        {
            gim::BoundaryPropagator<Vector2> propagator(root, {Vector2({10, 10})});

            THEN("the .next() method can be used to grab all elements within the bounds with the given point one by one, ending indefinitely on nullptr")
            {
                auto inList = root.recursiveFindChildren({"in"});
                std::set<gim::Element*> mandatory(inList.begin(), inList.end());

                CHECK(propagator.size() == 4);
                CHECK(mandatory.erase(propagator.next()) == 1);
                CHECK(mandatory.erase(propagator.next()) == 1);
                CHECK(mandatory.erase(propagator.next()) == 1);
                CHECK(mandatory.erase(propagator.next()) == 1);
                CHECK(propagator.next() == nullptr);
                CHECK(propagator.next() == nullptr);
            }
        }

        WHEN("the hierarchy is given to a BoundaryPropagator with two points added")
        {
            gim::BoundaryPropagator<Vector2> propagator(root, {Vector2({10, 10}), Vector2({12, 9})});

            THEN("the .next() method can be used to grab all elements within the bounds with the given point one by one, ending indefinitely on nullptr")
            {
                auto inList = root.recursiveFindChildren({"in"});
                std::set<gim::Element*> mandatory(inList.begin(), inList.end());
                auto inSecondList = root.recursiveFindChildren({"in_second"});
                mandatory.insert(inSecondList[0]);

                CHECK(propagator.size() == 5);
                CHECK(mandatory.erase(propagator.next()) == 1);
                CHECK(mandatory.erase(propagator.next()) == 1);
                CHECK(mandatory.erase(propagator.next()) == 1);
                CHECK(mandatory.erase(propagator.next()) == 1);
                CHECK(mandatory.erase(propagator.next()) == 1);
                CHECK(propagator.next() == nullptr);
                CHECK(propagator.next() == nullptr);
            }
        }

        WHEN("the hierarchy is given to a BoundaryPropagator and the order is reversed")
        {
            gim::BoundaryPropagator<Vector2> propagator(root, {Vector2({10, 10})});
            gim::BoundaryPropagator<Vector2> reversePropagator(root, {Vector2({10, 10})});
            reversePropagator.reverse();

            THEN("the order is the opposite to that of a normal execution")
            {
                std::vector<gim::Element*> normal;

                while(gim::Element* current = propagator.next())
                    normal.push_back(current);

                std::vector<gim::Element*> reversed;

                while(gim::Element* current = reversePropagator.next())
                    reversed.push_back(current);

                std::reverse(reversed.begin(), reversed.end());

                CHECK(normal == reversed);
            }
        }
    }
}
