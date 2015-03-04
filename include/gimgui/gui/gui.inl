#pragma once

template <typename Propagator>
void Gui::sendEvent(const Event& event)
{
    Propagator propagator(mRoot);

    gim::Element* element;

    while((element = propagator.next()))
        event(*element);
}
