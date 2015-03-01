#pragma once
    
template<typename Type>
Variant::Variant(const Type& data) :
    mStoredType(typeid(data))
{
    mStoredData = std::static_pointer_cast<void>(std::make_shared<Type>(data));

    mCopier = [this]()
    {
        std::shared_ptr<Type> toCopy = std::static_pointer_cast<Type>(mStoredData);
        std::shared_ptr<Type> copy = std::make_shared<Type>(*toCopy);
        return std::static_pointer_cast<void>(copy);
    };
}

template<typename Type>
Type& Variant::get()
{
    GIM_ASSERT(mStoredType == typeid(Type), "Trying to get variant as the type " + std::string(typeid(Type).name()) + " when it is of type " + std::string(mStoredType.name()));
    GIM_ASSERT(mStoredData != nullptr, "Trying to get uninitialised variant");

    return *std::static_pointer_cast<Type>(mStoredData);
}

template<typename Type>
const Type& Variant::get() const
{
    GIM_ASSERT(mStoredType == typeid(Type), "Trying to get variant as the type " + std::string(typeid(Type).name()) + " when it is of type " + std::string(mStoredType.name()));
    GIM_ASSERT(mStoredData != nullptr, "Trying to set uninitialised variant");

    return *std::static_pointer_cast<Type>(mStoredData);
}

template<typename Type>
void Variant::set(const Type& data)
{
    GIM_ASSERT(mStoredType == typeid(Type), "Trying to set variant as the type " + std::string(typeid(Type).name()) + " when it is of type " + std::string(mStoredType.name()));

    *std::static_pointer_cast<Type>(mStoredData) = data;
}

template<typename Type>
bool Variant::isOfType() const
{
    return mStoredType == typeid(Type);
}
