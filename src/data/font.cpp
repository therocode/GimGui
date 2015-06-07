#include <gimgui/data/font.hpp>
#include <gimgui/assert.hpp>
#include <vector>
#include <algorithm>
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H

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
        if(mLibrary != nullptr)
            FT_Done_FreeType(*mLibrary);
    }

    Font::Freetype::Freetype(Freetype&& other)
    {
        mLibrary = std::move(other.mLibrary);
    }

    Font::Freetype& Font::Freetype::operator=(Freetype&& other)
    {
        if(mLibrary != nullptr)
            FT_Done_FreeType(*mLibrary);

        mLibrary = std::move(other.mLibrary);
        return *this;
    }
    
    FT_Library& Font::Freetype::library() const
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
        if(mFace != nullptr)
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


    Font::Font(std::istream& fontData):
        mSize(0)
    {
        if(!fontData)
        {
            throw FontLoadException("invalid font data given. maybe the file does not exist?");
        }

        if(!fontData.eof() && !fontData.fail())
        {
            fontData.seekg(0, std::ios_base::end);
            std::streampos fileSize = fontData.tellg();
            mFontDataVector.resize(fileSize);

            fontData.seekg(0, std::ios_base::beg);
            fontData.read(mFontDataVector.data(), fileSize);
        }

        mFace = std::unique_ptr<FontFace>(new FontFace(mFreetype, mFontDataVector));

        //Select the Unicode character map
        if(FT_Select_Charmap(mFace->face(), FT_ENCODING_UNICODE) != 0)
        {
            throw FontLoadException("failed to set the Unicode character set");
        }

        //Store the font name
        mFamily = mFace->face()->family_name ? mFace->face()->family_name : std::string();
        mStyle = mFace->face()->style_name ? mFace->face()->style_name : std::string();
        
        //default size
        if(isFreelyScalable())
            resize(16);
        else
        {
            if(availableSizes().size() != 0)
                resize(availableSizes().front());
        }
    }

    Font::Font(Font&& other)
    {
        mFreetype = std::move(other.mFreetype);
        mFace = std::move(other.mFace);
        mFamily = std::move(other.mFamily);
        mStyle = std::move(other.mStyle);
        mSize = std::move(other.mSize);
        mFontDataVector = std::move(other.mFontDataVector);
    }

    Font& Font::operator=(Font&& other)
    {
        mFreetype = std::move(other.mFreetype);
        mFace = std::move(other.mFace);
        mFamily = std::move(other.mFamily);
        mStyle = std::move(other.mStyle);
        mSize = std::move(other.mSize);
        mFontDataVector = std::move(other.mFontDataVector);

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

    const std::string& Font::family() const
    {
        return mFamily;
    }

    const std::string& Font::style() const
    {
        return mStyle;
    }

    std::string Font::name() const
    {
        return mStyle != "" ? mFamily + " " + mStyle : mFamily;
    }

    void Font::resize(uint32_t size)
    {
        if(mSize != size)
        {
            if(!isFreelyScalable())
            {
                auto sizes = availableSizes();
                GIM_ASSERT(std::count(sizes.begin(), sizes.end(), size) != 0,"trying to resize a font which is not isFreelyScalable() to a size not in the list of availableSizes()");
            }

            mSize = size;
            FT_Error error = FT_Set_Pixel_Sizes(mFace->face(), size, size);
            GIM_ASSERT(error == FT_Err_Ok, "cannot resize font to " + std::to_string(size));
        }
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
    
    std::unique_ptr<Glyph> Font::generateGlyph(uint32_t codePoint) const
    {
        int32_t charError = FT_Load_Char(mFace->face(), codePoint, FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT);

        FT_Glyph glyph;
        int32_t glyphError = FT_Get_Glyph(mFace->face()->glyph, &glyph);

        bool hasGlyph = FT_Get_Char_Index(mFace->face(), codePoint);
        if(charError + glyphError == 0 && hasGlyph)
        {
            std::unique_ptr<Glyph> toReturn = std::unique_ptr<Glyph>(new Glyph());

            FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
            FT_Bitmap& bitmap = reinterpret_cast<FT_BitmapGlyph>(glyph)->bitmap;

            toReturn->size = size();
            toReturn->codePoint = codePoint;
            toReturn->metrics.advance = mFace->face()->glyph->metrics.horiAdvance / 64.0f;
            toReturn->metrics.left = mFace->face()->glyph->metrics.horiBearingX / 64.0f;
            toReturn->metrics.top = -mFace->face()->glyph->metrics.horiBearingY / 64.0f;
            toReturn->metrics.width = mFace->face()->glyph->metrics.width / 64.0f;
            toReturn->metrics.height = mFace->face()->glyph->metrics.height / 64.0f;

            uint32_t width = bitmap.width;
            uint32_t height = bitmap.rows;

            toReturn->image.width = width;
            toReturn->image.height = height;
            toReturn->image.pixels.resize(width * height);

            bool monochrome = bitmap.pixel_mode == FT_PIXEL_MODE_MONO;
            const uint8_t* sourcePixels = bitmap.buffer;
            for(int32_t y = 0; y < height; y++)
            {
                for(int32_t x = 0; x < width; x++)
                {
                    uint32_t targetIndex = x + y * width;

                    if(monochrome)
                        toReturn->image.pixels[targetIndex] = ((sourcePixels[x / 8]) & (1 << (7 - (x % 8)))) ? 255 : 0;
                    else
                        toReturn->image.pixels[targetIndex] = sourcePixels[x];
                }
                sourcePixels += bitmap.pitch;
            }

            FT_Done_Glyph(glyph);
            return toReturn;
        }
        else
        {
            FT_Done_Glyph(glyph);
            return nullptr;
        }
    }
}
