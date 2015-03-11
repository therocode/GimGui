#include <catch.hpp>
#include <gimgui/data/element.hpp>
#include <gimgui/logic/boundarypropagator.hpp>
#include <helpers/vec2.hpp>
#include <set>

SCENARIO("BoundaryPropagator propagates through the hierarchy based on which elements are within the boundary of the access point", "[logic]")
{
    GIVEN("a hierarchy of elements")
    {
        gim::TagSet NoTags;

            gim::Element root({"in"}, 
            {
                {"position", Vec2({0, 0})},
                {"size", Vec2({20, 20})}
            },
            {
                gim::Element({"in"},
                {
                    {"position", Vec2({8, 8})},
                    {"size", Vec2({5, 5})}
                },
                {
                    gim::Element({"not_in"},
                    {
                        {"position", Vec2({8, 8})},
                        {"size", Vec2({5, 5})}
                    },
                    {
                        gim::Element({"in"},
                        {
                            {"position", Vec2({-8, -8})},
                            {"size", Vec2({5, 5})}
                        })
                    }),
                    gim::Element({"in"},
                    {
                        {"position", Vec2({0, 0})},
                        {"size", Vec2({5, 5})}
                    })
                }),
                gim::Element({"in_second"}, 
                {
                    {"position", Vec2({11, 8})},
                    {"size", Vec2({5, 5})}
                })
            });

        WHEN("the hierarchy is given to a BoundaryPropagator with one point added")
        {
            gim::BoundaryPropagator<Vec2> propagator(root, {Vec2({10, 10})});

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
            gim::BoundaryPropagator<Vec2> propagator(root, {Vec2({10, 10}), Vec2({12, 9})});

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
    }
}
