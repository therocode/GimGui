#pragma once

namespace gim
{
    template <typename ElementType, typename FunctionType>
    void forEach(ElementType&& element, FunctionType function)
    {
        bool skipChildren = function(element);

        if(!skipChildren)
        {
            for(auto&& child : element.children())
            {
                forEach(*child, function);
            }
        }
    }

    template <typename ElementType, typename FunctionType, typename CarryOverFunctionType>
    void forEach(ElementType&& element, FunctionType function, CarryOverFunctionType carryOverFunction, decltype(carryOverFunction(element, {})) initialCarryOverValue = {})
    {
        const auto& currentCarryOver = carryOverFunction(element, initialCarryOverValue);

        bool skipChildren = function(element, currentCarryOver);

        if(!skipChildren)
        {
            for(auto&& child : element.children())
            {
                forEach(*child, function, carryOverFunction, currentCarryOver);
            }
        }
    }

    //template <typename Vec2>
    //bool isPointWithinElement(const Vec2& point, const Element& element)
    //{
    //    GIM_ASSERT(element.hasAttribute<Vec2>("position"), "isPointWithinElement was given element which doesn't have the attribute 'position'");
    //    GIM_ASSERT(element.hasAttribute<Vec2>("size"), "isPointWithinElement was given element which doesn't have the attribute 'size'");
    //
    //    Vec2 position = mAbsoluteMap.absolutePositionOf<Vec2>(element);
    //    const Vec2& size = element.getAttribute<Vec2>("size");
    //
    //    return std::any_of(points.begin(), points.end(), [&] (const Vec2& point) { return point.x > position.x && 
    //           point.x < position.x + size.x &&
    //           point.y > position.y &&
    //           point.y < position.y + size.y;});
    //}

#include <gimgui/logic/foreach.inl>
}
