#include "events.hpp"
#include <gimgui/data/element.hpp>
#include <helpers/color.hpp>
#include <gimgui/logic/absolutemap.hpp>

gim::Event randomColorEvent()
{
    return gim::Event([=] (gim::Element& element)
           {
               if(element.tags().count("random"))
               {
                    element.setAttribute("color", Color((uint8_t)(rand() % 256), (uint8_t)(rand() % 256u), (uint8_t)(rand() % 256u)));
               }
           });
};

gim::Event mouseHoverEvent(const Vec2& currentPosition, const Vec2& lastPosition)
{
    return gim::Event([=] (gim::Element& element)
    {
        gim::AbsoluteMap<Vec2> absoluteMap("position");
        const Vec2& position = absoluteMap.getAbsoluteOf(element);
        const Vec2& size = element.getAttribute<Vec2>("size");

        bool currentOverlaps = currentPosition.x > position.x &&
                               currentPosition.x < position.x + size.x &&
                               currentPosition.y > position.y &&
                               currentPosition.y < position.y + size.y;
        bool lastOverlaps = lastPosition.x > position.x &&
                            lastPosition.x < position.x + size.x &&
                            lastPosition.y > position.y &&
                            lastPosition.y < position.y + size.y;
                            
        if(currentOverlaps && !lastOverlaps)
        {//got hovered
        element.setAttribute("color", Color(250, 200, 150));
        }
        else if(lastOverlaps && !currentOverlaps)
        {//got un-hovered
        element.setAttribute("color", Color(150, 150, 150));
        }
    });
}
