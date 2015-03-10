#include "events.hpp"
#include <gimgui/data/element.hpp>
#include <helpers/color.hpp>

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

gim::Event nonFocusAllEvent()
{
    return gim::Event([=] (gim::Element& element)
    {
        element.setAttribute("color", Color(150, 150, 150));
    });
}

gim::Event focusEvent(int32_t x, int32_t y)
{
    return gim::Event([=] (gim::Element& element)
    {
        element.setAttribute("color", Color(200, 200, 200));
    });
}
