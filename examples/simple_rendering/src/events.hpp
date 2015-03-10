#pragma once
#include <gimgui/logic/event.hpp>


gim::Event randomColorEvent();
gim::Event nonFocusAllEvent();
gim::Event focusEvent(int32_t x, int32_t y);
