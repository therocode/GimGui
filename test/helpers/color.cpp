#include <helpers/color.hpp>

Color::Color():
    r(255), g(255), b(255), a(255)
{
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a):
    r(r), g(g), b(b), a(a)
{
}

bool Color::operator==(const Color& other) const
{
    return r == other.r && g == other.g && b == other.b && a == other.a;
}

bool Color::operator!=(const Color& other) const
{
    return !(other == *this);
}

ColorAdaptor::ColorAdaptor()
{
}

ColorAdaptor::ColorAdaptor(uint8_t r, uint8_t g, uint8_t b, uint8_t a):
    mColor({r, g, b, a})
{
}

ColorAdaptor::ColorAdaptor(const Color& color):
    mColor(color)
{
}

uint8_t ColorAdaptor::r() const
{
    return mColor.r;
}

uint8_t ColorAdaptor::g() const
{
    return mColor.g;
}

uint8_t ColorAdaptor::b() const
{
    return mColor.b;
}

uint8_t ColorAdaptor::a() const
{
    return mColor.a;
}
