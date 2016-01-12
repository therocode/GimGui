#include <gimgui/data/codepointsize.hpp>

namespace gim
{
    bool CodePointSize::operator==(const CodePointSize& other) const
    {
        return codePoint == other.codePoint && size == other.size;
    }

    bool CodePointSizeId::operator==(const CodePointSizeId& other) const
    {
        return codePoint == other.codePoint && size == other.size && id == other.id;
    }
}
