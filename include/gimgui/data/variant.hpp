#pragma once
#include <typeindex>
#include <memory>
#include <functional>
#include <gimgui/assert.hpp>

namespace gim
{
    class Variant
    {
        public:
            Variant();
            Variant(Variant&& other);
            Variant& operator=(Variant&& other);
            Variant(const Variant& other);
            Variant& operator=(const Variant& other);
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
            std::type_index mStoredType;
            std::shared_ptr<void> mStoredData;
            std::function<std::shared_ptr<void>()> mCopier;
    };
#include <gimgui/data/variant.inl>
}
