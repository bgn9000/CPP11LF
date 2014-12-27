#include "publisher.h"
#include "listener.h"

#include <vector>
#include <deque>

template<size_t _BufferCapacity, class _Tp, class _Alloc = std::allocator<_Tp>, template<typename, typename> class _BufferType = std::vector>
void testOneListenerThread(Listener<_BufferCapacity, _Tp, _Alloc,  _BufferType>& listener)
{
std::cout << "testOneListenerThread started" << std::endl;
    listener.emptyPublisherBuffer();
}

void testOnePublisherOneListener()
{
    std::cout << "Start testOnePublisherOneListener" << std::endl;
    constexpr int capacity = 1000;
    // Publisher fill buffer then listener empty it
    {
        Publisher<capacity+1,int> publisher; // buffer is a vector
        Listener<capacity+1,int> listener(publisher);
        high_resolution_clock::time_point start = high_resolution_clock::now();
        for (int cpt = 0; cpt < capacity; ++cpt)
        {
            publisher.fill(cpt);
        }
        high_resolution_clock::time_point end = high_resolution_clock::now();
        nanoseconds time_span = duration_cast<nanoseconds>(end - start);
        
        start = high_resolution_clock::now();
        listener.emptyPublisherBuffer();
        end = high_resolution_clock::now();
        nanoseconds time_span2 = duration_cast<nanoseconds>(end - start);
        std::cout << "Basic fill (vector) took [" << time_span.count() << "] ns then empty took [" << time_span2.count() << "] ns" << std::endl;
        
        Publisher<capacity+1,int, std::allocator<int>, std::deque> publisher2; // buffer is a deque
        Listener<capacity+1,int, std::allocator<int>, std::deque> listener2(publisher2);
        start = high_resolution_clock::now();
        for (int cpt = 0; cpt < capacity; ++cpt)
        {
            publisher2.fill(cpt);
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
        Publisher<capacity+1,int> publisher; // buffer is a vector
        Listener<capacity+1,int> listener(publisher);
        std::cout << "Spawn testOneListenerThread" << std::endl;
        std::thread testOneListenerThreaded(testOneListenerThread<capacity+1,int>, std::ref(listener));
//std::this_thread::sleep_for(seconds(20));
std::cout << "Parallel fill started" << std::endl;
        high_resolution_clock::time_point start = high_resolution_clock::now();
        for (int cpt = 0; cpt < capacity; ++cpt)
        {
            publisher.fill(cpt);
        }
        high_resolution_clock::time_point end = high_resolution_clock::now();
        nanoseconds time_span = duration_cast<nanoseconds>(end - start);
        testOneListenerThreaded.join();
        std::cout << "Parallel fill (vector) took [" << time_span.count() /*<< "] ns then empty took [" << time_span2.count()*/ << "] ns" << std::endl;
    }
    
//    std::this_thread::sleep_for(milliseconds(100));
}

