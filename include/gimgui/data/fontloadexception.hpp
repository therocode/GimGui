#pragma once
#include <stdexcept>

namespace gim
{
    class FontLoadException : public std::runtime_error
    {
        public:
            FontLoadException(const std::string& message);
    };
}
