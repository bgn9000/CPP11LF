#ifndef __PUBLISHER_H__
#define __PUBLISHER_H__

#include <iostream>

#include <thread>
#include <chrono>

#include <vector>

#include <atomic>

using namespace std::chrono;

template<size_t _BufferCapacity, class _Tp, class _Alloc = std::allocator<_Tp>, template<typename, typename> class _BufferType = std::vector>
class Publisher
{  
public:
    Publisher() 
        : buffer_(_BufferCapacity), pos_(0), stop_(false)
    {
        if (pos_.is_lock_free())
        {
            std::cout << "Publisher pos is lock free atomic" << std::endl;
        }
        else
        {
            std::cout << "!!!!! Publisher pos is not lock free atomic !!!!" << std::endl;
        }
        
        if (stop_.is_lock_free())
        {
            std::cout << "Publisher stop is lock free atomic" << std::endl;
        }
        else
        {
            std::cout << "!!!!! Publisher stop is not lock free atomic !!!!" << std::endl;
        }
    }
    
    _BufferType<_Tp, _Alloc>& getBuffer() { return buffer_; }
    size_t getPos() { return pos_.load(); }
    bool isStop() { return stop_.load(); }
    
    void fill(const _Tp& data)
    {
//if (pos_ % 100 == 0) std::cout << "Publisher [" << pos_ << "]=[" << data << "]" << std::endl;
        buffer_[pos_]=data;
        ++pos_;
        if (pos_ == _BufferCapacity) pos_ = 0;
    }
    
    void stop() { stop_ = true; }
        
private:
    _BufferType<_Tp, _Alloc> buffer_;
    std::atomic<size_t> pos_;
    std::atomic<bool> stop_;
};

#endif

