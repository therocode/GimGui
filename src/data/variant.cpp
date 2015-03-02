#include <gimgui/data/variant.hpp>

namespace gim
{
    Variant::Variant():
        mStoredType(typeid(void))
    {
        mCopier = [](std::shared_ptr<void> data)
        {
            return data;
        };
    }

    Variant::Variant(Variant&& other):
        mStoredType(typeid(void))
    {
        std::swap(other.mStoredData, mStoredData);
        std::swap(mStoredType, other.mStoredType);
        std::swap(mCopier, other.mCopier);
    }

    Variant& Variant::operator=(Variant&& other)
    {
        std::swap(mStoredData, other.mStoredData);
        std::swap(mStoredType, other.mStoredType);
        std::swap(mCopier, other.mCopier);

        return *this;
    }

    Variant::Variant(const Variant& other):
        mStoredType(other.mStoredType)
    {
        mStoredData = other.mCopier(other.mStoredData);
        mCopier = other.mCopier;
    }

    Variant& Variant::operator=(const Variant& other)
    {
        mStoredType = other.mStoredType;
        mStoredData = other.mCopier(other.mStoredData);
        mCopier = other.mCopier;
        return *this;
    }
    
    bool Variant::isSameTypeAs(const Variant& other) const
    {
        return mStoredType == other.mStoredType;
    }
}
