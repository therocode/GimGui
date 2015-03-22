template<typename ToType, typename FromType>
ToType resolve(const FromType& from, const std::map<FromType, ToType>& dictionary, const ToType& defaultResult)
{
    auto iterator = dictionary.find(from);

    if(iterator != dictionary.end())
        return iterator->second;
    else
        return defaultResult;
}

template<typename ToType, typename FromType>
ToType resolve(const FromType& from, const std::map<FromType, ToType>& dictionary)
{
    GIM_ASSERT(dictionary.count(from) != 0, "invalid value given to resolver");
    return dictionary.at(from);
}
