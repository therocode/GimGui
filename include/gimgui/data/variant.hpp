#pragma once
#include <typeindex>
#include <memory>
#include <gimgui/assert.hpp>

namespace gim
{
    class Variant
    {
        public:
            Variant();
            Variant(Variant&& other);
            Variant& operator=(Variant&& other);
            Variant(const Variant& other) = delete;
            Variant& operator=(const Variant& other) = delete;
            template<typename Type>
            Variant(const Type& data);
            template<typename Type>
            const Type& get() const;
            template<typename Type>
            Type& get();
            template<typename Type>
            void set(const Type& data);
            template<typename Type>
            bool isOfType() const;
            bool isSameTypeAs(const Variant& other) const;
        private:
            std::type_index storedType;
            std::shared_ptr<void> storedData;
    };
#include <gimgui/data/variant.inl>
}
