#pragma once

template <typename Vec2>
BoundaryPropagator<Vec2>::BoundaryPropagator(Element& first, const std::deque<Vec2>& positions):
    mAbsoluteMap("position")
{
    std::list<Element*> toInvestigate({&first});

    if(anyPointWithinElement(positions, first))
        mElements.push_back(&first);

    for(auto elementIterator = toInvestigate.begin(); elementIterator != toInvestigate.end(); elementIterator++)
    {
        for(auto& child : (*elementIterator)->children())
        {
            toInvestigate.push_back(child.get());
            if(anyPointWithinElement(positions, *child))
                mElements.push_back(child.get());
        }
    }

    mCurrentElement = mElements.begin();
}

template <typename Vec2>
Element* BoundaryPropagator<Vec2>::next()
{
    if(mCurrentElement != mElements.end())
    {
        Element* toReturn = *mCurrentElement;
        ++mCurrentElement;
        return toReturn;
    }
    else
    {
        return nullptr;
    }
}

template <typename Vec2>
size_t BoundaryPropagator<Vec2>::size() const
{
    return mElements.size();
}

template <typename Vec2>
bool BoundaryPropagator<Vec2>::anyPointWithinElement(const std::deque<Vec2>& points, const Element& element)
{
    GIM_ASSERT(element.hasAttribute<Vec2>("position"), "BoundaryPropagator was given element which doesn't have the attribute 'position'");
    GIM_ASSERT(element.hasAttribute<Vec2>("size"), "BoundaryPropagator was given element which doesn't have the attribute 'size'");

    Vec2 position = mAbsoluteMap.getAbsoluteOf(element);
    const Vec2& size = element.getAttribute<Vec2>("size");

    return std::any_of(points.begin(), points.end(), [&] (const Vec2& point) { return point.x > position.x && 
           point.x < position.x + size.x &&
           point.y > position.y &&
           point.y < position.y + size.y;});
}
