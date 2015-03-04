#include <catch.hpp>
#include <gimgui/data/element.hpp>
#include <gimgui/logic/allpropagator.hpp>
#include <set>

SCENARIO("AllPropagator propagates through all elements of a gui hierarchy", "[logic]")
{
    GIVEN("a hierarchy of elements")
    {
        gim::TagSet NoTags;

            gim::Element root(NoTags,
            {
                gim::Element(NoTags,
                {
                    gim::Element(NoTags)
                }),
                gim::Element(NoTags)
            });

        WHEN("the hierarchy is given to an AllPropagator")
        {
            gim::AllPropagator propagator(root);

            THEN("the .next() method can be used to grab all elements one by one, ending indefinitely on nullptr")
            {
                std::set<gim::Element*> pointers(
                {
                    &root,
                    &*root.children()[0],
                    &*root.children()[1],
                    &*root.children()[0]->children()[0]
                });

                CHECK(pointers.erase(propagator.next()) == 1);
                CHECK(pointers.erase(propagator.next()) == 1);
                CHECK(pointers.erase(propagator.next()) == 1);
                CHECK(pointers.erase(propagator.next()) == 1);
                CHECK(propagator.next() == nullptr);
                CHECK(propagator.next() == nullptr);
            }
        }
    }
}
