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
        : buffer_(_BufferCapacity), pos_(0), finish_(false)
    {
        if (pos_.is_lock_free())
        {
            std::cout << "Publisher pos is lock free atomic" << std::endl;
        }
        else
        {
            std::cout << "!!!!! Publisher pos is not lock free atomic !!!!" << std::endl;
        }
        
        if (finish_.is_lock_free())
        {
            std::cout << "Publisher finish is lock free atomic" << std::endl;
        }
        else
        {
            std::cout << "!!!!! Publisher finish is not lock free atomic !!!!" << std::endl;
        }
    }
    
    _BufferType<_Tp, _Alloc>& getBuffer() { return buffer_; }
    size_t getPos() { return pos_.load(); }
    bool isFinish() { return finish_.load(); }
    
    void fill(const _Tp& data)
    {
//if (pos_ % 10 == 0) std::cout << "Publisher [" << pos_ << "]=[" << data << "]" << std::endl;
        int pos = pos_.load();
        buffer_[pos]=data;
        ++pos;
        if (pos == _BufferCapacity) pos = 0;
        pos_.store(pos);
    }
    
    void finish() { finish_ = true; }
        
private:
    _BufferType<_Tp, _Alloc> buffer_;
    std::atomic<size_t> pos_;
    std::atomic<bool> finish_;
};

#endif

