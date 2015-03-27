#include <gimgui/data/font.hpp>
#include <gimgui/assert.hpp>
#include <vector>
#include <algorithm>

namespace gim
{
    Font::Freetype::Freetype():
        mLibrary(new FT_Library())
    {
        int32_t error = FT_Init_FreeType(mLibrary.get());

        if(error)
        {
            throw FontLoadException("could not initialize freetype");
        }
    }

    Font::Freetype::~Freetype()
    {
        FT_Done_FreeType(*mLibrary);
    }

    Font::Freetype::Freetype(Freetype&& other)
    {
        mLibrary = std::move(other.mLibrary);
    }

    Font::Freetype& Font::Freetype::operator=(Freetype&& other)
    {
        mLibrary = std::move(other.mLibrary);
        return *this;
    }
    
    FT_Library& Font::Freetype::library()
    {
        return *mLibrary;
    }


    Font::FontFace::FontFace(Freetype& freetype, const std::vector<char>& fontDataVector):
        mFace(new FT_Face())
    {
        int32_t error = FT_New_Memory_Face(freetype.library(), reinterpret_cast<const FT_Byte*>(fontDataVector.data()), static_cast<FT_Long>(fontDataVector.size()), 0, mFace.get());
        if(error)
        {
            throw FontLoadException("failed to create font face");
        }
    }

    Font::FontFace::~FontFace()
    {
        FT_Done_Face(*mFace);
    }

    Font::FontFace::FontFace(FontFace&& other)
    {
        mFace = std::move(other.mFace);
    }

    Font::FontFace& Font::FontFace::operator=(FontFace&& other)
    {
        mFace = std::move(other.mFace);
        return *this;
    }

    FT_Face& Font::FontFace::face()
    {
        return *mFace;
    }


    Font::Font(std::istream& fontData)
    {
        if(!fontData)
        {
            throw FontLoadException("invalid font data given. maybe the file does not exist?");
        }

        std::vector<char> fontDataVector;
        if(!fontData.eof() && !fontData.fail())
        {
            fontData.seekg(0, std::ios_base::end);
            std::streampos fileSize = fontData.tellg();
            fontDataVector.resize(fileSize);

            fontData.seekg(0, std::ios_base::beg);
            fontData.read(&fontDataVector[0], fileSize);
        }

        mFace = std::unique_ptr<FontFace>(new FontFace(mFreetype, fontDataVector));

        //Select the Unicode character map
        if(FT_Select_Charmap(mFace->face(), FT_ENCODING_UNICODE) != 0)
        {
            throw FontLoadException("failed to set the Unicode character set");
        }

        //Store the font name
        mFamily = mFace->face()->family_name ? mFace->face()->family_name : std::string();
        
        //default size
        if(isFreelyScalable())
            resize(16);
        else
            resize(availableSizes().front());
    }

    Font::Font(Font&& other)
    {
        mFreetype = std::move(other.mFreetype);
        mFace = std::move(other.mFace);
        mFamily = std::move(other.mFamily);
    }

    Font& Font::operator=(Font&& other)
    {
        mFreetype = std::move(other.mFreetype);
        mFace = std::move(other.mFace);
        mFamily = std::move(other.mFamily);

        return *this;
    }
    
    bool Font::isFreelyScalable() const
    {
        return FT_IS_SCALABLE(mFace->face());
    }

    std::vector<uint32_t> Font::availableSizes() const
    {
        GIM_ASSERT(!isFreelyScalable(), "it doesn't make sense to return available sizes for a font which isFreelyScalable()");

        std::vector<uint32_t> sizeList;

        for(int32_t i = 0; i < mFace->face()->num_fixed_sizes; ++i)
        {
            sizeList.push_back(mFace->face()->available_sizes[i].height);
        }
        
        return  sizeList;
    }

    const std::string Font::family() const
    {
        return mFamily;
    }

    void Font::resize(uint32_t size)
    {
        if(!isFreelyScalable())
        {
            GIM_ASSERT(std::count(availableSizes().begin(), availableSizes().end(), size) != 0,"trying to resize a font which is not isFreelyScalable() to a size not in the list of availableSizes()");
        }

        mSize = size;
        FT_Error error = FT_Set_Pixel_Sizes(mFace->face(), size, size);
        GIM_ASSERT(error == FT_Err_Ok, "cannot resize font to " + std::to_string(size));
    }

    uint32_t Font::size() const
    {
        return mSize;
    }
    
    float Font::kerning(uint32_t codePointA, uint32_t codePointB) const
    {
        if(FT_HAS_KERNING(mFace->face()))
        {
            FT_Vector result;

            FT_UInt indexA = FT_Get_Char_Index(mFace->face(), codePointA);
            FT_UInt indexB = FT_Get_Char_Index(mFace->face(), codePointB);

            FT_Get_Kerning(mFace->face(), indexA, indexB, FT_KERNING_DEFAULT, &result);

            if(isFreelyScalable())
                return result.x / 64.0f; //divide by 64 because FT_Vectors define positions in 1/64th of a pixel for vectorial fonts
            else
                return result.x;
        }
        else
        {
            return 0.0f;
        }
    }

    float Font::lineSpacing() const
    {
        return mFace->face()->size->metrics.height / 64.0f;
    }

    float Font::underlinePosition() const
    {
        if(!isFreelyScalable())
        {
            return mSize / 10.f;
        }
        else
        {
            return -static_cast<float>(FT_MulFix(mFace->face()->underline_position, mFace->face()->size->metrics.y_scale)) / 64.0f;
        }
    }

    float Font::underlineThickness() const
    {
        if(!isFreelyScalable())
        {
            return mSize / 14.f;
        }
        else
        {
            return static_cast<float>(FT_MulFix(mFace->face()->underline_thickness, mFace->face()->size->metrics.y_scale)) / 64.0f;
        }
    }
}
