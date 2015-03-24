#pragma once
#include <gimgui/data/fontloadexception.hpp>
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace gim
{
    class Font
    {
        public:
            Font(std::istream& fontData);
        private:
            FT_Library mFreetype;
    };
}
