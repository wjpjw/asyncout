#pragma once

#include <atomic>
#include <string.h>
#include <memory>

namespace wjp
{

template <uint32_t BUFFER_SIZE>
class membuf{
public:
    //无锁线程安全的写buffer方法：唯一的潜在缺陷是buffer要满时一次失败的写导致pos增加了，这时另一次原本可能成功的写可能由于pos增加的值未及时减掉而失败。此时buffer已经接近写满，这种情况损失的内存很少，而且出现概率很低。相比无锁实现的性能，这个缺陷是可以接受的tradeoff。
    bool                        append(const char* what, uint32_t size) noexcept
    {
        auto before=pos.fetch_add(size);
        if(before+size>=BUFFER_SIZE)
        {
            pos.fetch_sub(size);
            return false;
        }
        memcpy(meta_->buf+before, what, size);
        return true;
    }
    uint32_t                    length() const noexcept
    {
        return meta_->pos.load()+1;
    }
    const char*                 data() const noexcept 
    {
        return meta_->buf;
    }
private:
    struct meta{
        char                    buf[BUFFER_SIZE];
        std::atomic<uint32_t>   pos;  // 4G
    };
    std::shared_ptr<meta> meta_;
};


}