#include "events.hpp"
#include <helpers/color.hpp>
#include <gimgui/logic/absolutemap.hpp>
#include <gimgui/logic/boundarypropagator.hpp>
#include <gimgui/logic/allpropagator.hpp>
#include <gimgui/util/getorfallback.hpp>

void mouseClicked(gim::Element& element, const Vec2& position, MouseButton button)
{
    gim::AbsoluteMap<Vec2> positionMap("position");

    gim::BoundaryPropagator<Vec2> boundaryPropagator(element, {position});
    boundaryPropagator.reverse();

    std::vector<gim::Element*> toDrag;

    while(gim::Element* currentElement = boundaryPropagator.next())
    {
        Vec2 absolutePosition = positionMap.getAbsoluteOf(*currentElement);
        CallbackExecutor executor("on_click");
        executor.execute(*currentElement, {{"position", position},
                                           {"relative_position", Vec2({position.x - absolutePosition.x, position.y - absolutePosition.y})},
                                           {"button", button}});

        toDrag.push_back(currentElement);

        if(bool* blocks = currentElement->findAttribute<bool>("block_event"))
            break;
    }

    gim::AllPropagator allPropagator(element);

    while(gim::Element* currentElement = allPropagator.next())
    {
        Vec2 absolutePosition = positionMap.getAbsoluteOf(*currentElement);
        CallbackExecutor executor("on_global_click");
        executor.execute(*currentElement, {{"position", position},
                                           {"relative_position", Vec2({position.x - absolutePosition.x, position.y - absolutePosition.y})},
                                           {"button", button}});
    }

    for(auto element : toDrag)
    {
        int32_t oldDrag = element->getAttribute<int32_t>("dragged");
        element->setAttribute("dragged", oldDrag | button);
    }
}

void mouseReleased(gim::Element& element, const Vec2& position, MouseButton button)
{
    gim::AbsoluteMap<Vec2> positionMap("position");

    gim::BoundaryPropagator<Vec2> boundaryPropagator(element, {position});
    boundaryPropagator.reverse();

    std::vector<gim::Element*> toUnDrag;

    while(gim::Element* currentElement = boundaryPropagator.next())
    {
        Vec2 absolutePosition = positionMap.getAbsoluteOf(*currentElement);
        CallbackExecutor executor("on_release");
        executor.execute(*currentElement, {{"position", position},
                                           {"relative_position", Vec2({position.x - absolutePosition.x, position.y - absolutePosition.y})},
                                           {"button", button}});
        toUnDrag.push_back(currentElement);

        if(bool* blocks = currentElement->findAttribute<bool>("block_event"))
            break;
    }

    gim::AllPropagator allPropagator(element);

    while(gim::Element* currentElement = allPropagator.next())
    {
        Vec2 absolutePosition = positionMap.getAbsoluteOf(*currentElement);
        CallbackExecutor executor("on_global_release");
        executor.execute(*currentElement, {{"position", position},
                                           {"relative_position", Vec2({position.x - absolutePosition.x, position.y - absolutePosition.y})},
                                           {"button", button}});
    }

    for(auto element : toUnDrag)
    {
        int32_t oldDrag = element->getAttribute<int32_t>("dragged");
        element->setAttribute("dragged", oldDrag & (~button));
    }
}

void moveMouse(gim::Element& element, const Vec2& currentPosition, const Vec2& lastPosition)
{
    Vec2 delta;
    delta.x = currentPosition.x - lastPosition.x;
    delta.y = currentPosition.y - lastPosition.y;

    auto overlaps = [] (const Vec2& posA, const Vec2& posB, const Vec2& size) 
    {
        return posA.x > posB.x &&
            posA.x < posB.x + size.x &&
            posA.y > posB.y &&
            posA.y < posB.y + size.y;
    };

    gim::BoundaryPropagator<Vec2> propagator(element, {currentPosition, lastPosition});


    while(gim::Element* currentElement = propagator.next())
    {
        if(currentElement->getAttribute<int32_t>("dragged"))
        {//got dragged
            CallbackExecutor executor("on_drag");
            executor.execute(*currentElement, {{"position", currentPosition},
                    {"delta", delta}});
        }

        gim::AbsoluteMap<Vec2> absoluteMap("position");
        Vec2 elementPosition = absoluteMap.getAbsoluteOf(*currentElement);
        Vec2 elementSize = currentElement->getAttribute<Vec2>("size");

        bool currentPosOverlaps = overlaps(currentPosition, elementPosition, elementSize);
        bool lastPosOverlaps = overlaps(lastPosition, elementPosition, elementSize);

        Vec2 relativePosition;
        relativePosition.x = currentPosition.x - elementPosition.x;
        relativePosition.y = currentPosition.y - elementPosition.y;

        absoluteMap = gim::AbsoluteMap<Vec2>("position");
        elementPosition = absoluteMap.getAbsoluteOf(*currentElement);
        elementSize = currentElement->getAttribute<Vec2>("size");

        if(currentPosOverlaps && !lastPosOverlaps)
        {//got hovered
            CallbackExecutor executor("on_hover");
            executor.execute(*currentElement, {{"position", currentPosition},
                    {"delta", delta}});
        }
        else if(lastPosOverlaps && !currentPosOverlaps)
        {//got blurred
            CallbackExecutor executor("on_blur");
            executor.execute(*currentElement, {{"position", currentPosition},
                    {"delta", delta}});
            currentElement->setAttribute("dragged", 0);
        }
    }
}
