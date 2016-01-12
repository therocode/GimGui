#include <catch.hpp>
#include <helpers/vector2.hpp>
#include <helpers/color.hpp>
#include <helpers/closeenough.hpp>

TEST_CASE("Vector2 helper struct equality", "[test]")
{
    Vector2 pointA{45, 54};
    Vector2 pointB{45, 54};
    Vector2 pointC{45, 53};
    Vector2 pointD{46, 53};

    CHECK(pointA == pointB);
    CHECK(pointA != pointC);
    CHECK(pointA != pointD);
    CHECK(pointC != pointD);
}

TEST_CASE("Vector2 helper struct addition", "[test]")
{
    Vector2 pointA{5, 6};
    Vector2 pointB{7, 8};

    Vector2 pointC{1, 2};
    pointC += pointA;

    CHECK((pointA + pointB) == Vector2({12, 14}));
    CHECK(pointC == Vector2({6, 8}));
}

TEST_CASE("Color helper struct construction", "[test]")
{
    Color colorA(1, 2, 3);
    Color colorB(1, 2, 3, 4);

    CHECK(colorA == Color({1, 2, 3, 255}));
    CHECK(colorB == Color({1, 2, 3, 4}));
}

TEST_CASE("Color helper struct equality", "[test]")
{
    Color colorA{12, 13, 11, 5};
    Color colorB{100, 100, 100, 100};
    Color colorC{12, 13, 11, 5};

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
