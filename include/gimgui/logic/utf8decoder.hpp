#pragma once
#include <vector>
#include <string>
#include <cstdint>

namespace gim
{
    class Utf8Decoder
    {
        public:
            std::vector<uint32_t> decode(std::string hej);

        private:
    };
}
