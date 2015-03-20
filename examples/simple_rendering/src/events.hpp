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

//goes to the element which was actually clicked
void clicked(gim::Element& element, const Vec2& position);
//goes to all elements, notifying that a mouse button was released
void mouseRelease(gim::Element& element, const Vec2& position);
void moveMouse(gim::Element& element, const Vec2& currentPosition, const Vec2& lastPosition);
