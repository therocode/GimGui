#include "events.hpp"
#include <gimgui/data/element.hpp>
#include <helpers/color.hpp>

gim::Event randomColorEvent()
{
    return gim::Event([=] (gim::Element& element)
           {
               if(element.tags().count("random"))
               {
                    element.setAttribute("color", Color({(uint8_t)(rand() % 256), (uint8_t)(rand() % 256u), (uint8_t)(rand() % 256u)}));
               }
           });
};

