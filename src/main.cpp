#include <iostream>

#include <thread>
#include <chrono>

#include <vector>
#include <list>
#include <deque>

#include <atomic>

using namespace std::chrono;

void testThreadedFunction()
{
//    std::cout << "Start First" << std::endl;
//    std::this_thread::sleep_for(milliseconds(100));
}

//std::vector<high_resolution_clock::time_point> buffer(1000);

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
/*if (atomic_is_lock_free(std::atomic<int>())
{
}*/ 
    }
    
    void emptyPublisherBuffer()
    {
        while (pos_ < publisher_.getPos() || publisher_.getPos() < _BufferCapacity - 1)
        {
            time_span_ = duration_cast<nanoseconds>(high_resolution_clock::now() - publisher_.getBuffer()[pos_]);
            ++pos_;
//std::this_thread::sleep_for(nanoseconds(200));
//std::cout << "Listener [" << pos_ << "] duration [" << time_span_.count() << "] ns" << std::endl;
        }
    }
        
private:
    Publisher<_BufferCapacity, _Tp, _Alloc,  _BufferType>& publisher_;
    size_t pos_;
    nanoseconds time_span_;
};

constexpr int capacity = 100000;

template<size_t _BufferCapacity, class _Tp, class _Alloc = std::allocator<_Tp>, template<typename, typename> class _BufferType = std::vector>
void testOneListenerThread(Listener<_BufferCapacity, _Tp, _Alloc,  _BufferType>& listener)
{
}

void testOnePublisherOneListener()
{
    std::cout << "Start testOnePublisherOneListener" << std::endl;
    // Publisher fill buffer then listener empty it
    {
        Publisher<capacity+1,high_resolution_clock::time_point> publisher; // buffer is a vector
        Listener<capacity+1,high_resolution_clock::time_point> listener(publisher);
        high_resolution_clock::time_point start = high_resolution_clock::now();
        for (int cpt = 0; cpt < capacity; ++cpt)
        {
            publisher.fill(high_resolution_clock::now());
        }
        high_resolution_clock::time_point end = high_resolution_clock::now();
        nanoseconds time_span = duration_cast<nanoseconds>(end - start);
        
        start = high_resolution_clock::now();
        listener.emptyPublisherBuffer();
        end = high_resolution_clock::now();
        nanoseconds time_span2 = duration_cast<nanoseconds>(end - start);
        std::cout << "Basic fill (vector) took [" << time_span.count() << "] ns then empty took [" << time_span2.count() << "] ns" << std::endl;
        
        Publisher<capacity+1,high_resolution_clock::time_point, std::allocator<high_resolution_clock::time_point>, std::deque> publisher2; // buffer is a deque
        Listener<capacity+1,high_resolution_clock::time_point, std::allocator<high_resolution_clock::time_point>, std::deque> listener2(publisher2);
        start = high_resolution_clock::now();
        for (int cpt = 0; cpt < capacity; ++cpt)
        {
            publisher2.fill(high_resolution_clock::now());
        }
        end = high_resolution_clock::now();
        time_span = duration_cast<nanoseconds>(end - start);
        start = high_resolution_clock::now();
        listener2.emptyPublisherBuffer();
        end = high_resolution_clock::now();
        time_span2 = duration_cast<nanoseconds>(end - start);
        std::cout << "Basic fill (deque) took [" << time_span.count() << "] ns then empty took [" << time_span2.count() << "] ns" << std::endl;
    }
    
    // Parallel fill and empty
    {
        Publisher<capacity+1,high_resolution_clock::time_point> publisher; // buffer is a vector
        Listener<capacity+1,high_resolution_clock::time_point> listener(publisher);
        std::thread testOneListenerThreaded(testOneListenerThread<capacity+1,high_resolution_clock::time_point>, std::ref(listener));
        testOneListenerThreaded.join();
    }
    
//    std::this_thread::sleep_for(milliseconds(100));
}

int main(int argc, char* argv[])
{
    std::cout << "Start" << std::endl;
        
    high_resolution_clock::time_point start = high_resolution_clock::now();
    std::thread firstThread(testThreadedFunction);
    firstThread.join();
    high_resolution_clock::time_point end = high_resolution_clock::now();
    nanoseconds time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "Spawn thread function took [" << time_span.count() << "] ns" << std::endl;
    
    start = high_resolution_clock::now();
    std::thread secondThread(testThreadedFunction);
    secondThread.detach();
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "Spawn detach thread function took [" << time_span.count() << "] ns" << std::endl;
    
    std::thread testOnePublisherOneListenerThread(testOnePublisherOneListener);
    testOnePublisherOneListenerThread.join();
    
    std::cout << "End" << std::endl;
    return 0;
}

