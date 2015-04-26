#include <gimgui/data/textstyles.hpp>

namespace gim
{
    TextStyle TextStyle::NORMAL = TextStyle(0);
    TextStyle TextStyle::BOLD = TextStyle(1 << 0);
    TextStyle TextStyle::ITALIC = TextStyle(1 << 1);
    TextStyle TextStyle::UNDERLINED = TextStyle(1 << 2);
    TextStyle TextStyle::STRIKETHROUGH = TextStyle(1 << 3);
    TextStyle TextStyle::HOLLOW = TextStyle(1 << 4);

    TextStyle::TextStyle():
        mValue(0)
    {
    }

    TextStyle::TextStyle(int32_t value):
        mValue(value)
    {
    }

    TextStyle TextStyle::operator&(const TextStyle& other) const
    {
        TextStyle result;
        result.mValue = mValue & other.mValue;
        return result;
    }

    TextStyle TextStyle::operator|(const TextStyle& other) const
    {
        TextStyle result;
        result.mValue = mValue | other.mValue;
        return result;
    }

    TextStyle::operator bool()const
    {
        return mValue != 0;
    }
}
