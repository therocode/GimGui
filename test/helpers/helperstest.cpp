#include <catch.hpp>
#include <helpers/vec2.hpp>
#include <helpers/color.hpp>
#include <helpers/closeenough.hpp>

TEST_CASE("Vec2 helper struct equality", "[test]")
{
    Vec2 pointA{45, 54};
    Vec2 pointB{45, 54};
    Vec2 pointC{45, 53};
    Vec2 pointD{46, 53};

    CHECK(pointA == pointB);
    CHECK(pointA != pointC);
    CHECK(pointA != pointD);
    CHECK(pointC != pointD);
}

TEST_CASE("Vec2 helper struct addition", "[test]")
{
    Vec2 pointA{5, 6};
    Vec2 pointB{7, 8};

    Vec2 pointC{1, 2};
    pointC += pointA;

    CHECK((pointA + pointB) == Vec2({12, 14}));
    CHECK(pointC == Vec2({6, 8}));
}

TEST_CASE("Color helper struct equality", "[test]")
{
    Color colorA{12, 13, 11};
    Color colorB{100, 100, 100};
    Color colorC{12, 13, 11};

    CHECK(colorA == colorC);
    CHECK_FALSE(colorB == colorC);
    CHECK_FALSE(colorA == colorB);
}

TEST_CASE("Close enough function can be used to compare floats", "[test]")
{
    float a = 5.0f;
    float b = 10.0f;

    float c = a * b;

    CHECK(closeEnough(c, 50.0f));
    CHECK_FALSE(closeEnough(c, 49.0f));
}
