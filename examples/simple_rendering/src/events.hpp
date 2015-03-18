#pragma once
#include <gimgui/logic/event.hpp>
#include <gimgui/data/element.hpp>
#include <helpers/vec2.hpp>
#include <vector>
#include <functional>

template<typename... Parameters>
using Callback = std::function<void(gim::Element&, const Parameters&...)>;

template<typename... Parameters>
using CallbackList = std::vector<Callback<Parameters...>>;

template<typename... Parameters>
class CallbackExecutor
{
    public:
        CallbackExecutor(const std::string& attributeName):
            mAttributeName(attributeName)
        {
        }
        void execute(gim::Element& element, const Parameters&... parameters)
        {
            if(auto* callback = element.findAttribute<Callback<Parameters...>>(mAttributeName))
            {
                (*callback)(element, parameters...);
            }
            else if(auto* callbackList = element.findAttribute<CallbackList<Parameters...>>(mAttributeName))
            {
                for(auto callback : *callbackList)
                {
                    callback(element, parameters...);
                }
            }
        }
    private:
        std::string mAttributeName;
};

void click(gim::Element& element, const Vec2& position);
void mouseRelease(gim::Element& element, const Vec2& position);
void moveMouse(gim::Element& element, const Vec2& currentPosition, const Vec2& lastPosition);
