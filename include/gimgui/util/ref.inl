template <typename Type>
ref<Type> makeRef(Type& value)
{
    return ref<Type>(value);
}
