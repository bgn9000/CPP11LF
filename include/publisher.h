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
        : buffer_(_BufferCapacity), pos_(0)
    {
    }
    
    _BufferType<_Tp, _Alloc>& getBuffer() { return buffer_; }
    size_t getPos() { return pos_; }
    
    void fill(const _Tp& data)
    {
        buffer_[pos_]=data;
        ++pos_;
        if (pos_ == _BufferCapacity) pos_ = 0;
//std::cout << "Publisher [" << pos_ << "]" << std::endl;
    }
        
private:
    _BufferType<_Tp, _Alloc> buffer_;
    size_t pos_;
};

#endif

