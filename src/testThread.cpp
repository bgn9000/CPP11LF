#include <iostream>

#include <thread>
#include <chrono>

using namespace std::chrono;

void testThreadedFunction()
{
//    std::cout << "Start Thread" << std::endl;
//    std::this_thread::sleep_for(milliseconds(100));
}

void testThread()
{
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
}
