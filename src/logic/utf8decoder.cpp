#include <gimgui/logic/utf8decoder.hpp> 

/*                Converting from UTF-8 to Unicode                 */
/*******************************************************************/
// UTF-8 (char*)                       | Unicode (21 bits)         //
// ------------------------------------+-------------------------- //
// 0xxxxxxx                            | 00000000000000000xxxxxxx  //
// ------------------------------------+-------------------------- //
// 110yyyyy 10xxxxxx                   | 0000000000000yyyyyxxxxxx  //
// ------------------------------------+-------------------------- //
// 1110zzzz 10yyyyyy 10xxxxxx          | 00000000zzzzyyyyyyxxxxxx  //
// ------------------------------------+-------------------------- //
// 11110www 10zzzzzz 10yyyyyy 10xxxxxx | 000wwwzzzzzzyyyyyyxxxxxx  //
/*******************************************************************/

namespace gim
{
    std::vector<uint32_t> Utf8Decoder::decode(std::string str)
    {
        std::vector<uint32_t> codePoints;

        for(uint32_t i = 0; i < str.size(); ++i)
        {
            if((str[i] >> 7) == 0)
            {
                codePoints.push_back(uint32_t(str[i]));
            }
            else if(((uint8_t)str[i] >> 5) == 6)
            {
                // add "x"s to the unicode bits //
                uint8_t character = str[i + 1];
                // take away the "10" //
                uint8_t bitmask = 127; // 0x01111111
                uint32_t codePoint = uint32_t(character & bitmask);

                // add "y"s to the unicode bits //
                character = str[i];
                // take away the "110" //
                bitmask = 63; // 0x00111111
                uint32_t yValues = uint32_t(character & bitmask);
                codePoint = codePoint | (yValues << 6);

                codePoints.push_back(codePoint);
                ++i;
            }
            else if(((uint8_t)str[i] >> 4) == 14)
            {
                // add "x"s to the unicode bits //
                uint8_t character = str[i + 2];
                // take away the "10" //
                uint8_t bitmask = 127; // 0x01111111
                uint32_t codePoint = uint32_t(character & bitmask);

                // add "y"s to the unicode bits //
                character = str[i + 1];
                // take away the "10" //
                uint32_t yValues = uint32_t(character & bitmask);
                codePoint = codePoint | (yValues << 6);

                // add "z"s to the unicode bits //
                character = str[i];
                // take away the "1110" //
                bitmask = 31; // 0x00011111
                uint32_t zValues = uint32_t(character & bitmask);
                codePoint = codePoint | (zValues << 12);

                codePoints.push_back(codePoint);
                i += 2;
            }
            else if(((uint8_t)str[i] >> 3) == 30)
            {
                // add "x"s to the unicode bits //
                uint8_t character = str[i + 3];
                // take away the "10" //
                uint8_t bitmask = 127; // 0x01111111
                uint32_t codePoint = uint32_t(character & bitmask);

                // add "y"s to the unicode bits //
                character = str[i + 2];
                // take away the "10" //
                uint32_t yValues = uint32_t(character & bitmask);
                codePoint = codePoint | (yValues << 6);

                // add "z"s to the unicode bits //
                character = str[i + 1];
                // take away the "10" //
                uint32_t zValues = uint32_t(character & bitmask);
                codePoint = codePoint | (zValues << 12);

                // add "w"s to the unicode bits //
                character = str[i];
                // take away the "11110" //
                bitmask = 7; // 0x00001111
                uint32_t wValues = uint32_t(character & bitmask);
                codePoint = codePoint | (wValues << 18);

                codePoints.push_back(codePoint);
                i += 3;
            }
        }
        return codePoints;
    }
}
