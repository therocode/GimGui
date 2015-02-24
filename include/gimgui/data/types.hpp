#pragma once
#include <set>
#include <deque>
#include <string>
#include <memory>

namespace gim
{
    class Element;

    using ElementList = std::deque<std::unique_ptr<Element>>;
    using ElementPtrList = std::deque<Element*>;
    using ElementConstPtrList = std::deque<const Element*>;
    using TagSet = std::set<std::string>;
}
