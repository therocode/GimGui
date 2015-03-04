#pragma once
#include <functional>

namespace gim
{
    class Element;

    using Event = std::function<void(gim::Element&)>;
    //class Event
    //{
    //    public:
    //        Event(std::function<void(gim::Element&)> function);
    //    private:
    //        
    //};
}
