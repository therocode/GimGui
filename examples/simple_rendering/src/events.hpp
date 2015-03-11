#pragma once
#include <gimgui/logic/event.hpp>
#include <helpers/vec2.hpp>

gim::Event randomColorEvent();
gim::Event mouseHoverEvent(const Vec2& currentPosition, const Vec2& lastPosition);
