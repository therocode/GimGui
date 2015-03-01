#include <catch.hpp>
#include <helpers/point.hpp>

bool Point::operator==(const Point& other) const
{
    return x == other.x && y == other.y;
}

bool Point::operator!=(const Point& other) const
{
    return !(other == *this);
}

TEST_CASE("Point helper struct", "[test]")
{
    Point pointA{45, 54};
    Point pointB{45, 54};
    Point pointC{45, 53};
    Point pointD{46, 53};

    CHECK(pointA == pointB);
    CHECK(pointA != pointC);
    CHECK(pointA != pointD);
    CHECK(pointC != pointD);
}
