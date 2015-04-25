#pragma once
#include <functional>

namespace gim
{
    template <typename Type>
    using ref = std::reference_wrapper<Type>;
    
    template <typename Type>
    ref<Type> makeRef(Type& value);

#include <gimgui/util/ref.inl>
}
