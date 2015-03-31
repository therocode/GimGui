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
}

namespace std
{
    template<>
        struct hash<gim::CodePointSize>
        {
            public:
                std::size_t operator()(const gim::CodePointSize& codePointSize) const 
                {
                    return static_cast<size_t>(codePointSize.codePoint | codePointSize.size);
                }
        };
}
