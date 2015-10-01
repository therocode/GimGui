#include <gimgui/logic/rectanglepacker.hpp>

namespace gim
{
    namespace internal
    {
        RectanglePacker::RectanglePacker(int32_t size):
            mInitialSize(size),
            mPackers({GuillotineBinPack(size, size)})
        {
        }
        
        Rectangle RectanglePacker::insert(const IVec2& size)
        {
            GIM_ASSERT(size.x > 0 && size.y > 0, "size cannot be zero");
            Rectangle packed = pack(size);
        
            while(packed.size.y == 0)
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
        
        Rectangle RectanglePacker::pack(const IVec2& size)
        {
            GuillotineBinPack::Rect packed = mPackers.back().Insert(size.x, size.y, true, GuillotineBinPack::RectBestAreaFit, GuillotineBinPack::SplitShorterLeftoverAxis);
        
            IVec2 offset = indexToPosition(mPackers.size() - 1);
        
            return Rectangle{{packed.x + offset.x, packed.y + offset.y}, {packed.width, packed.height}};
        }
        
        int32_t RectanglePacker::indexToOrder(int32_t index)
        {
            GIM_ASSERT(index >= 0, "index cannot be negative");
            return (index - 1) / 3 + 1;
        }
        
        IVec2 RectanglePacker::indexToPosition(int32_t index)
        {
            GIM_ASSERT(index >= 0, "index cannot be negative");
        
            if(index == 0)
                return IVec2{ 0, 0 };
        
            int32_t order = indexToOrder(index);
            int32_t orderIndex = (index - 1) % 3;
        
            int32_t orderOffset = orderToSize(order);
        
            IVec2 position{ 0, 0 };
        
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
        
        int32_t RectanglePacker::orderToSize(int32_t order)
        {
            GIM_ASSERT(order >= 0, "order cannot be negative");
            return mInitialSize * std::pow(2, order - 1);
        }
    }
}
