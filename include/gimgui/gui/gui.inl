#pragma once

template <typename Propagator>
void Gui::sendEvent(const Event& event, Propagator& propagator)
{
    gim::Element* element;

    while((element = propagator.next()))
        event(*element);
}
