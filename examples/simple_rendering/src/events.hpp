#pragma once
#include <gimgui/logic/event.hpp>
#include <gimgui/data/element.hpp>
#include <helpers/vec2.hpp>
#include <vector>
#include <functional>

using Parameters = std::unordered_map<std::string, gim::Variant>;

using Callback = std::function<void(gim::Element&, const Parameters&)>;
using CallbackList = std::vector<Callback>;

class CallbackExecutor
{
    public:
        CallbackExecutor(const std::string& attributeName):
            mAttributeName(attributeName)
        {
        }
        void execute(gim::Element& element, const Parameters& parameters)
        {
            if(auto* callback = element.findAttribute<Callback>(mAttributeName))
            {
                (*callback)(element, parameters);
            }
            else if(auto* callbackList = element.findAttribute<CallbackList>(mAttributeName))
            {
                for(auto callback : *callbackList)
                {
                    callback(element, parameters);
                }
            }
        }
    private:
        std::string mAttributeName;
};

enum MouseButton { UNKNOWN = 0, LEFT = 1, RIGHT = 2, MIDDLE = 4};

//goes to the element which was actually clicked
void mouseClicked(gim::Element& element, const Vec2& position, MouseButton button);
//goes to all elements, notifying that a mouse button was released
void mouseReleased(gim::Element& element, const Vec2& position, MouseButton button);
void moveMouse(gim::Element& element, const Vec2& currentPosition, const Vec2& lastPosition);
