#include <gimgui/data/fontloadexception.hpp>

namespace gim
{
    FontLoadException::FontLoadException(const std::string& message) : runtime_error(message)
    {
    }
}
