#pragma once

#include <cassert>
#include <iostream>

#include <thread>
#include <chrono>

#include <vector>

#include <atomic>

using namespace std::chrono;

template<size_t _BufferCapacity, class _Tp, class _Alloc = std::allocator<_Tp>, template<typename, typename> class _BufferType = std::vector>
class Listener
{
public:
    Listener(Publisher<_BufferCapacity, _Tp, _Alloc,  _BufferType>& publisher) 
        : publisher_(publisher), pos_(0), ready_(false)
    {
    }
    
    Listener(const Listener<_BufferCapacity, _Tp, _Alloc,  _BufferType>&) = delete;
    Listener<_BufferCapacity, _Tp, _Alloc,  _BufferType>& operator=(const Listener<_BufferCapacity, _Tp, _Alloc,  _BufferType>&) = delete;
    
    void emptyPublisherBuffer()
    {
        ready_=true;
        while (1)
        {
            if (pos_ < publisher_.getPos())
            {
                assert(pos_ == (unsigned int)publisher_.getBuffer()[pos_]);
//if (pos_ % 10 == 0) std::cout << "Listener [" << pos_ << "] publisher [" << publisher_.getBuffer()[pos_] << ":" << publisher_.getPos() << "]" << std::endl;
//std::this_thread::sleep_for(milliseconds(30));
                ++pos_;
            }
            else if (publisher_.isFinish())
            {
                break;
            }
        }
    }
    
    bool isReady() { return ready_.load(); }
        
private:
    Publisher<_BufferCapacity, _Tp, _Alloc,  _BufferType>& publisher_;
    size_t pos_;
    std::atomic<bool> ready_;
};

