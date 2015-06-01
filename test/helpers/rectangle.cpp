#include <helpers/rectangle.hpp>

bool Rectangle::operator==(const Rectangle& other) const
{
    return start == other.size && size == other.size;
}

bool Rectangle::operator!=(const Rectangle& other) const
{
    return !(other == *this);
}

RectangleAdaptor::RectangleAdaptor()
{
}

RectangleAdaptor::RectangleAdaptor(const Vec2Adaptor& start, const Vec2Adaptor& size):
    mRectangle{Vec2{start.x(), start.y()}, Vec2{size.x(), size.y()}}
{
}

RectangleAdaptor::RectangleAdaptor(const Rectangle& vec2):
    mRectangle(vec2)
{
}

Vec2Adaptor RectangleAdaptor::start() const
{
    return Vec2Adaptor(mRectangle.start);
}

Vec2Adaptor RectangleAdaptor::size() const
{
    return Vec2Adaptor(mRectangle.size);
}
