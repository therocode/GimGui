#pragma once
#include <set>
#include <deque>
#include <string>

namespace gim
{
    class Element;

    using ElementList = std::deque<Element>;
    using ElementPtrList = std::deque<Element*>;
    using TagSet = std::set<std::string>;
}
