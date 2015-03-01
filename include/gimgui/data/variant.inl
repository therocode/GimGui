#pragma once
    
template<typename Type>
Variant::Variant(const Type& data) :
    storedType(typeid(data))
{
    storedData = std::static_pointer_cast<void>(std::shared_ptr<Type>(new Type(data)));
}

template<typename Type>
Type& Variant::get()
{
    GIM_ASSERT(storedType == typeid(Type), "Trying to get variant as the type " + std::string(typeid(Type).name()) + " when it is of type " + std::string(storedType.name()));
    GIM_ASSERT(storedData != nullptr, "Trying to get uninitialised variant");

    return *std::static_pointer_cast<Type>(storedData);
}

template<typename Type>
const Type& Variant::get() const
{
    GIM_ASSERT(storedType == typeid(Type), "Trying to get variant as the type " + std::string(typeid(Type).name()) + " when it is of type " + std::string(storedType.name()));
    GIM_ASSERT(storedData != nullptr, "Trying to set uninitialised variant");

    return *std::static_pointer_cast<Type>(storedData);
}

template<typename Type>
void Variant::set(const Type& data)
{
    GIM_ASSERT(storedType == typeid(Type), "Trying to set variant as the type " + std::string(typeid(Type).name()) + " when it is of type " + std::string(storedType.name()));

    *std::static_pointer_cast<Type>(storedData) = data;
}

template<typename Type>
bool Variant::isOfType() const
{
    return storedType == typeid(Type);
}
