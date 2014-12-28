#include "publisher.h"
#include "listener.h"

#include "common.h"

#include <future>
#include <deque>

template<size_t _BufferCapacity, class _Tp, class _Alloc = std::allocator<_Tp>, template<typename, typename> class _BufferType = std::vector>
unsigned long long testOneListenerAsync(Listener<_BufferCapacity, _Tp, _Alloc,  _BufferType>& listener)
{
std::cout << "Parallel empty started" << std::endl;
    high_resolution_clock::time_point start = high_resolution_clock::now();
    listener.emptyPublisherBuffer();
    high_resolution_clock::time_point end = high_resolution_clock::now();
    return (unsigned long long) duration_cast<nanoseconds>(end - start).count();
}

void testOnePublisherOneListener()
{
    std::cout << "================== testOnePublisherOneListener =================" << std::endl;
    constexpr int capacity = 1000;
    constexpr int loop = 100;
    // Publisher fill buffer then listener empty it
    if (doTestOnePublisherOneListenerThreadBasic)
    {
        Publisher<capacity,int> publisher; // buffer is a vector
        Listener<capacity,int> listener(publisher);
        high_resolution_clock::time_point start = high_resolution_clock::now();
        for (int cpt = 0; cpt < loop; ++cpt)
        {
            publisher.fill(cpt);
        }
        publisher.stop();
        high_resolution_clock::time_point end = high_resolution_clock::now();
        nanoseconds time_span = duration_cast<nanoseconds>(end - start);
        
        start = high_resolution_clock::now();
        listener.emptyPublisherBuffer();
        end = high_resolution_clock::now();
        nanoseconds time_span2 = duration_cast<nanoseconds>(end - start);
        std::cout << "Basic fill (vector) took [" << time_span.count() << "] ns then empty took [" << time_span2.count() << "] ns" << std::endl;
        
        Publisher<capacity,int, std::allocator<int>, std::deque> publisher2; // buffer is a deque
        Listener<capacity,int, std::allocator<int>, std::deque> listener2(publisher2);
        start = high_resolution_clock::now();
        for (int cpt = 0; cpt < loop; ++cpt)
        {
            publisher2.fill(cpt);
        }
        publisher.stop();
        end = high_resolution_clock::now();
        time_span = duration_cast<nanoseconds>(end - start);
        start = high_resolution_clock::now();
        listener2.emptyPublisherBuffer();
        end = high_resolution_clock::now();
        time_span2 = duration_cast<nanoseconds>(end - start);
        std::cout << "Basic fill (deque) took [" << time_span.count() << "] ns then empty took [" << time_span2.count() << "] ns" << std::endl;
    }
    
    // Parallel fill and empty
    if (doTestOnePublisherOneListenerThreadParallel) 
    {
        Publisher<capacity,int> publisher; // buffer is a vector
        Listener<capacity,int> listener(publisher);
        std::cout << "Async testOneListenerAsync" << std::endl;
        auto time_span2 = std::async(std::launch::async, testOneListenerAsync<capacity,int>, std::ref(listener));
std::this_thread::sleep_for(milliseconds(100));
std::cout << "Parallel fill started" << std::endl;
        high_resolution_clock::time_point start = high_resolution_clock::now();
        for (int cpt = 0; cpt < loop; ++cpt)
        {
            publisher.fill(cpt);
        }
        publisher.stop();
        high_resolution_clock::time_point end = high_resolution_clock::now();
        nanoseconds time_span = duration_cast<nanoseconds>(end - start);
        std::cout << "Parallel fill (vector) took [" << time_span.count() << "] ns then empty took [" << time_span2.get() << "] ns" << std::endl;
    }
}

