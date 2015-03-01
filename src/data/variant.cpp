#include <gimgui/data/variant.hpp>

namespace gim
{
    Variant::Variant():
        storedType(typeid(void))
    {
    }

    Variant::Variant(Variant&& other):
        storedType(typeid(void))
    {
        std::swap(other.storedData, storedData);
        std::swap(storedType, other.storedType);
    }

    Variant& Variant::operator=(Variant&& other)
    {
        std::swap(storedData, other.storedData);
        std::swap(storedType, other.storedType);

        return *this;
    }
    
    bool Variant::isSameTypeAs(const Variant& other) const
    {
        return storedType == other.storedType;
    }
}
