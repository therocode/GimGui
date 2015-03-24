template <typename Vec2>
RectanglePacker<Vec2>::RectanglePacker(int32_t size):
    mInitialSize(size),
    mPackers({GuillotineBinPack(size, size)})
{
}

template <typename Vec2>
Rectangle<Vec2> RectanglePacker<Vec2>::insert(const Vec2& size)
{
    Rectangle<Vec2> packed = pack(size);

    if(packed.size.y == 0)
    {//the packer is full, we need to make a new one
        int32_t newIndex = mPackers.size();
        int32_t newOrder = indexToOrder(newIndex);
        int32_t newSize = orderToSize(newOrder);
        mPackers.push_back(GuillotineBinPack(newSize, newSize));
        packed = pack(size);
    }

    GIM_ASSERT(packed.size.y != 0, "could not pack this rectangle. this should not happen");
    return packed;
}

template <typename Vec2>
Rectangle<Vec2> RectanglePacker<Vec2>::pack(const Vec2& size)
{
    GuillotineBinPack::Rect packed = mPackers.back().Insert(size.x, size.y, true, GuillotineBinPack::RectBestAreaFit, GuillotineBinPack::SplitShorterLeftoverAxis);

    Vec2 offset = indexToPosition(mPackers.size() - 1);

    return Rectangle<Vec2>(Vec2({packed.x + offset.x, packed.y + offset.y}), Vec2({packed.width, packed.height}));
}

template <typename Vec2>
int32_t RectanglePacker<Vec2>::indexToOrder(int32_t index)
{
    GIM_ASSERT(index >= 0, "index cannot be negative");
    return (index - 1) / 3 + 1;
}

template <typename Vec2>
Vec2 RectanglePacker<Vec2>::indexToPosition(int32_t index)
{
    GIM_ASSERT(index >= 0, "index cannot be negative");

    if(index == 0)
        return Vec2({0, 0});

    int32_t order = indexToOrder(index);
    int32_t orderIndex = (index - 1) % 3;

    int32_t orderOffset = orderToSize(order);

    Vec2 position({0, 0});

    if(orderIndex == 0)
    {
        position.x = orderOffset;
    }
    else if(orderIndex == 1)
    {
        position.y = orderOffset;
    }
    else if(orderIndex == 2)
    {
        position.x = orderOffset;
        position.y = orderOffset;
    }

    return position;
}

template <typename Vec2>
int32_t RectanglePacker<Vec2>::orderToSize(int32_t order)
{
    GIM_ASSERT(order >= 0, "order cannot be negative");
    return mInitialSize * std::pow(2, order - 1);
}
