#include <gimgui/data/codepointsizeid.hpp>

namespace gim
{
    bool CodePointSizeId::operator==(const CodePointSizeId& other) const
    {
        return codePoint == other.codePoint && size == other.size && id == other.id;
    }
}
