#pragma once
#include <cstdint>

namespace gim
{
    class TextStyle
    {
        public:
           static TextStyle NORMAL;
           static TextStyle BOLD;
           static TextStyle ITALIC;
           static TextStyle UNDERLINED;
           static TextStyle STRIKETHROUGH;
           static TextStyle HOLLOW;
           TextStyle();
           TextStyle(int32_t value);
           TextStyle operator&(const TextStyle& other) const;
           TextStyle operator|(const TextStyle& other) const;
           operator bool()const;
        private:
           int32_t mValue;
    };
}
