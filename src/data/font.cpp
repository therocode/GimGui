#include <gimgui/data/font.hpp>
#include <vector>
#include <istream>

namespace gim
{
    Font::Font(std::istream& fontData)
    {
        if(!fontData)
        {
            throw FontLoadException("invalid font data given. maybe the file does not exist?");
        }
        int32_t error = FT_Init_FreeType(&mFreetype);

        if(error)
        {
            throw FontLoadException("could not initialize freetype");
        }

        std::vector<char> fontDataVector;
        if (!fontData.eof() && !fontData.fail())
        {
            fontData.seekg(0, std::ios_base::end);
            std::streampos fileSize = fontData.tellg();
            fontDataVector.resize(fileSize);

            fontData.seekg(0, std::ios_base::beg);
            fontData.read(&fontDataVector[0], fileSize);
        }

        // Load the new font face from the specified file
        FT_Face face;
        if (FT_New_Memory_Face(static_cast<FT_Library>(mFreetype), reinterpret_cast<const FT_Byte*>(fontDataVector.data()), static_cast<FT_Long>(fontDataVector.size()), 0, &face) != 0)
        {
            throw FontLoadException("failed to create font face");
        }

        // Select the Unicode character map
        if (FT_Select_Charmap(face, FT_ENCODING_UNICODE) != 0)
        {
            throw FontLoadException("failed to set the Unicode character set");
        }

        //// Store the loaded font in our ugly void* :)
        //m_face = face;

        //// Store the font information
        //m_info.family = face->family_name ? face->family_name : std::string();

    }
}
