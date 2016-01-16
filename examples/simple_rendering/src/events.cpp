#include "events.hpp"
#include <helpers/color.hpp>
#include <algorithm>
#include <gimgui/logic/absolutemap.hpp>
#include <gimgui/logic/foreach.hpp>
//#include <gimgui/logic/boundarypropagator.hpp>
//#include <gimgui/logic/allpropagator.hpp>
#include <gimgui/util/getorfallback.hpp>

bool overlapsPoint(const gim::Element& element, const Vector2& absolutePosition, const Vector2& point)
{
    GIM_ASSERT(element.hasAttribute<Vector2>("size"), "overlapsPoint was given element which doesn't have the attribute 'size'");
    const Vector2& size = element.getAttribute<Vector2>("size");

    return  point.x > absolutePosition.x && 
            point.x < absolutePosition.x + size.x &&
            point.y > absolutePosition.y &&
            point.y < absolutePosition.y + size.y;
}

void mouseClicked(gim::Element& element, const Vector2& position, MouseButton button)
{
    std::vector<std::pair<std::reference_wrapper<gim::Element>, Vector2>> clickedElements;
    std::vector<std::pair<std::reference_wrapper<gim::Element>, Vector2>> allElements;

    gim::forEach(element, [&position, &clickedElements, &allElements] (gim::Element& element, const Vector2& absolutePosition)
    {
        bool overlaps = overlapsPoint(element, absolutePosition, position);
        
        allElements.emplace_back(element, absolutePosition);   

        if(overlaps)
            clickedElements.emplace_back(element, absolutePosition);   

        return !overlaps;
    },
    [] (const gim::Element& element, const Vector2& parentPosition)
    {
        const Vector2* positionPtr = element.findAttribute<Vector2>("position");

        GIM_ASSERT(positionPtr != nullptr, "Element lacks position value. Cannot be rendered.");
        
        return parentPosition + *positionPtr;
    });

    std::reverse(clickedElements.begin(), clickedElements.end());
    std::reverse(allElements.begin(), allElements.end());

    std::vector<std::reference_wrapper<gim::Element>> toDrag;

    for(auto&& currentElementIter : clickedElements)
    {
        auto&& currentElement = currentElementIter.first;
        Vector2 absolutePosition = currentElementIter.second;

        CallbackExecutor executor("on_click");
        executor.execute(currentElement, {{"position", position},
                                           {"relative_position", Vector2({position.x - absolutePosition.x, position.y - absolutePosition.y})},
                                           {"button", button}});

        toDrag.push_back(currentElement);

        if(bool* blocks = currentElement.get().findAttribute<bool>("block_event"))
            break;
    }

    for(auto&& currentElementIter : allElements)
    {
        auto&& currentElement = currentElementIter.first;
        Vector2 absolutePosition = currentElementIter.second;
        CallbackExecutor executor("on_global_click");
        executor.execute(currentElement, {{"position", position},
                                           {"relative_position", Vector2({position.x - absolutePosition.x, position.y - absolutePosition.y})},
                                           {"button", button}});
    }

    for(auto element : toDrag)
    {
        int32_t oldDrag = element.get().getAttribute<int32_t>("dragged");
        element.get().setAttribute("dragged", oldDrag | button);
    }
}

void mouseReleased(gim::Element& element, const Vector2& position, MouseButton button)
{
    std::vector<std::pair<std::reference_wrapper<gim::Element>, Vector2>> releasedElements;
    std::vector<std::pair<std::reference_wrapper<gim::Element>, Vector2>> allElements;

    gim::forEach(element, [&position, &releasedElements, &allElements] (gim::Element& element, const Vector2& absolutePosition)
    {
        bool overlaps = overlapsPoint(element, absolutePosition, position);
        
        allElements.emplace_back(element, absolutePosition);   

        if(overlaps)
            releasedElements.emplace_back(element, absolutePosition);   

        return !overlaps;
    },
    [] (const gim::Element& element, const Vector2& parentPosition)
    {
        const Vector2* positionPtr = element.findAttribute<Vector2>("position");

        GIM_ASSERT(positionPtr != nullptr, "Element lacks position value. Cannot be rendered.");
        
        return parentPosition + *positionPtr;
    });

    std::reverse(releasedElements.begin(), releasedElements.end());
    std::reverse(allElements.begin(), allElements.end());

    std::vector<std::reference_wrapper<gim::Element>> toUnDrag;

    for(auto&& currentElementIter : releasedElements)
    {
        auto&& currentElement = currentElementIter.first;
        Vector2 absolutePosition = currentElementIter.second;

        CallbackExecutor executor("on_release");
        executor.execute(currentElement, {{"position", position},
                                           {"relative_position", Vector2({position.x - absolutePosition.x, position.y - absolutePosition.y})},
                                           {"button", button}});
        toUnDrag.push_back(currentElement);

        if(bool* blocks = currentElement.get().findAttribute<bool>("block_event"))
            break;
    }

    for(auto&& currentElementIter : allElements)
    {
        auto&& currentElement = currentElementIter.first;
        Vector2 absolutePosition = currentElementIter.second;

        CallbackExecutor executor("on_global_release");
        executor.execute(currentElement, {{"position", position},
                                           {"relative_position", Vector2({position.x - absolutePosition.x, position.y - absolutePosition.y})},
                                           {"button", button}});
    }

    for(auto element : toUnDrag)
    {
        int32_t oldDrag = element.get().getAttribute<int32_t>("dragged");
        element.get().setAttribute("dragged", oldDrag & (~button));
    }
}

void moveMouse(gim::Element& element, const Vector2& currentPosition, const Vector2& lastPosition)
{
    std::vector<std::pair<std::reference_wrapper<gim::Element>, Vector2>> movedElements;

    gim::forEach(element, [&currentPosition, &lastPosition, &movedElements] (gim::Element& element, const Vector2& absolutePosition)
    {
        bool overlaps = overlapsPoint(element, absolutePosition, currentPosition) || overlapsPoint(element, absolutePosition, lastPosition);
        
        if(overlaps)
            movedElements.emplace_back(element, absolutePosition);   

        return !overlaps;
    },
    [] (const gim::Element& element, const Vector2& parentPosition)
    {
        const Vector2* positionPtr = element.findAttribute<Vector2>("position");

        GIM_ASSERT(positionPtr != nullptr, "Element lacks position value. Cannot be rendered.");
        
        return parentPosition + *positionPtr;
    });

    std::reverse(movedElements.begin(), movedElements.end());

    auto overlaps = [] (const Vector2& posA, const Vector2& posB, const Vector2& size) 
    {
        return posA.x > posB.x &&
            posA.x < posB.x + size.x &&
            posA.y > posB.y &&
            posA.y < posB.y + size.y;
    };

    Vector2 delta;
    delta.x = currentPosition.x - lastPosition.x;
    delta.y = currentPosition.y - lastPosition.y;

    for(auto&& currentElementIter : movedElements)
    {
        auto&& currentElement = currentElementIter.first.get();
        Vector2 absolutePosition = currentElementIter.second;

        if(currentElement.getAttribute<int32_t>("dragged"))
        {//got dragged
            CallbackExecutor executor("on_drag");
            executor.execute(currentElement, {{"position", currentPosition},
                    {"delta", delta}});
        }

        Vector2 elementSize = currentElement.getAttribute<Vector2>("size");

        bool currentPosOverlaps = overlaps(currentPosition, absolutePosition, elementSize);
        bool lastPosOverlaps = overlaps(lastPosition, absolutePosition, elementSize);

        Vector2 relativePosition;
        relativePosition.x = currentPosition.x - absolutePosition.x;
        relativePosition.y = currentPosition.y - absolutePosition.y;

        elementSize = currentElement.getAttribute<Vector2>("size");

        if(currentPosOverlaps && !lastPosOverlaps)
        {//got hovered
            CallbackExecutor executor("on_hover");
            executor.execute(currentElement, {{"position", currentPosition},
                    {"delta", delta}});
        }
        else if(lastPosOverlaps && !currentPosOverlaps)
        {//got blurred
            CallbackExecutor executor("on_blur");
            executor.execute(currentElement, {{"position", currentPosition},
                    {"delta", delta}});
            currentElement.setAttribute("dragged", 0);
        }
    }
}
