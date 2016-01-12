#pragma once
#include <cstdint>
#include <functional>

namespace gim
{
    struct CodePointSize
    {
        bool operator==(const CodePointSize& other) const;
        uint32_t codePoint;
        uint32_t size;
    };

    struct CodePointSizeId
    {
        bool operator==(const CodePointSizeId& other) const;
        int32_t id;
        uint32_t codePoint;
        uint32_t size;
    };
}

namespace std
{
    template<>
        struct hash<gim::CodePointSize>
        {
            public:
                std::size_t operator()(const gim::CodePointSize& codePointSize) const 
                {
                    return static_cast<size_t>(((size_t)codePointSize.codePoint | codePointSize.size) << 16);
                }
        };

    template<>
    struct hash<gim::CodePointSizeId>
    {
        public:
            std::size_t operator()(const gim::CodePointSizeId& codePointSizeId) const 
            {
                return static_cast<size_t>(((size_t)codePointSizeId.codePoint | codePointSizeId.size << 8) | codePointSizeId.id << 8);
            }
    };
}
