#pragma once
#include <cstdint>
#include <functional>

namespace gim
{
    struct CodePointSizeId
    {
        bool operator==(const CodePointSizeId& other) const;
        uint32_t codePoint;
        uint32_t size;
        uint32_t id;
    };
}

namespace std
{
    template<>
        struct hash<gim::CodePointSizeId>
        {
            public:
                std::size_t operator()(const gim::CodePointSizeId& codePointSizeId) const 
                {
                    return static_cast<size_t>((((size_t)codePointSizeId.codePoint | codePointSizeId.size) << 16) | codePointSizeId.id);
                }
        };
}
