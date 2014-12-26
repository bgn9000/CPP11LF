#include <iostream>

#include <thread>
#include <chrono>

#include <vector>
#include <list>
#include <deque>

using namespace std::chrono;

std::vector<high_resolution_clock::time_point> buffer(1000);

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

template<size_t _BufferCapacity, class _Tp, class _Alloc = std::allocator<_Tp>, template<typename, typename> class _BufferType = std::vector>
class Listener
{
public:
    Listener(Publisher<_BufferCapacity, _Tp, _Alloc,  _BufferType>& publisher) 
        : publisher_(publisher), pos_(0)
    {
    }
    
    void emptyPublisherBuffer()
    {
        while (pos_ < publisher_.getPos())
        {
            time_span_ = duration_cast<microseconds>(high_resolution_clock::now() - publisher_.getBuffer()[pos_]);
            ++pos_;
std::cout << "Listener [" << pos_ << "] duration [" << time_span_.count() << "] us" << std::endl;
        }
    }
        
private:
    Publisher<_BufferCapacity, _Tp, _Alloc,  _BufferType>& publisher_;
    size_t pos_;
    microseconds time_span_;
};

void testThreadedFunction()
{
//    std::cout << "Start First" << std::endl;
//    std::this_thread::sleep_for(milliseconds(100));
}

void testOnePublisherOneListener()
{
    std::cout << "Start testOnePublisherOneListener" << std::endl;
    constexpr int capacity = 10;
    Publisher<capacity+1,high_resolution_clock::time_point> publisher; // buffer is a vector
//    Publisher<capacity+1,high_resolution_clock::time_point, std::allocator<high_resolution_clock::time_point>, std::list> publisher2; // buffer is a list
//    Publisher<capacity+1,high_resolution_clock::time_point, std::allocator<high_resolution_clock::time_point>, std::deque> publisher3; // buffer is a deque   
    Listener<capacity+1,high_resolution_clock::time_point> listener(publisher);
    
    // Publisher fill buffer then listener empty it
    high_resolution_clock::time_point start = high_resolution_clock::now();
    for (int cpt = 0; cpt < capacity; ++cpt)
    {
        publisher.fill(high_resolution_clock::now());
    }
    high_resolution_clock::time_point end = high_resolution_clock::now();
    microseconds time_span = duration_cast<microseconds>(end - start);
    
    start = high_resolution_clock::now();
    listener.emptyPublisherBuffer();
    end = high_resolution_clock::now();
    microseconds time_span2 = duration_cast<microseconds>(end - start);
    std::cout << "Basic fill took [" << time_span.count() << "] us then empty took [" << time_span2.count() << "] us" << std::endl;
    
//    std::this_thread::sleep_for(milliseconds(100));
}

int main(int argc, char* argv[])
{
    std::cout << "Start" << std::endl;
    
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    
    std::thread firstThread(testThreadedFunction);
    
    firstThread.join();
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    microseconds time_span = duration_cast<microseconds>(t2 - t1);
    std::cout << "Spawn thread function took [" << time_span.count() << "] us" << std::endl;
    
    std::thread testOnePublisherOneListenerThread(testOnePublisherOneListener);
    testOnePublisherOneListenerThread.join();
    
    std::cout << "End" << std::endl;
    return 0;
}

