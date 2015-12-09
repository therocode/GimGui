#pragma once
#include <gimgui/data/fontloadexception.hpp>
#include <gimgui/data/glyph.hpp>
#include <memory>
#include <istream>
#include <vector>

struct FT_LibraryRec_;
typedef struct FT_LibraryRec_  *FT_Library;
struct FT_FaceRec_;
typedef struct FT_FaceRec_ *FT_Face;

namespace gim
{
    class Font
    {
        class Freetype
        {
            public:
                Freetype();
                ~Freetype();
                Freetype(Freetype&& other);
                Freetype& operator=(Freetype&& other);
                Freetype(const Freetype& other) = delete;
                Freetype& operator=(const Freetype& other) = delete;
                FT_Library& library() const;
                std::unique_ptr<FT_Library> mLibrary;
        };

        class FontFace
        {
            public:
                FontFace(Freetype& freetype, const std::vector<char>& fontDataVector);
                ~FontFace();
                FontFace(FontFace&& other);
                FontFace& operator=(FontFace&& other);
                FontFace(const FontFace& other) = delete;
                FontFace& operator=(const FontFace& other) = delete;
                FT_Face& face();
                std::unique_ptr<FT_Face> mFace;
        };

        public:
            Font(std::istream& fontData);
            Font(std::istream&& fontData);
            Font(Font&& other);
            Font& operator=(Font&& other);
            Font(const Font& other) = delete;
            Font& operator=(const Font& other) = delete;
            bool isFreelyScalable() const;
            std::vector<uint32_t> availableSizes() const;
            const std::string& family() const;
            const std::string& style() const;
            std::string name() const;
            void resize(uint32_t size);
            uint32_t size() const;
            float kerning(uint32_t codePointA, uint32_t codePointB) const;
            float lineSpacing() const;
            float underlinePosition() const;
            float underlineThickness() const;
            std::unique_ptr<Glyph> generateGlyph(uint32_t codePoint) const;
        private:
            void loadFontFromStream(std::istream& fontData);
            Freetype mFreetype;
            std::unique_ptr<FontFace> mFace;
            std::vector<char> mFontDataVector;
            std::string mFamily;
            std::string mStyle;
            uint32_t mSize;
    };
}
