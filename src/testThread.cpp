#include "common.h"

#include <future>

void testThreadedFunction()
{
//    std::cout << "Start Thread" << std::endl;
//    std::this_thread::sleep_for(milliseconds(100));
}

int testAsyncFunction()
{
    return 42;
}

void testThread()
{
    std::cout << "================== testThread =================" << std::endl;
    
    high_resolution_clock::time_point start = high_resolution_clock::now();
    std::thread firstThread(testThreadedFunction);
    high_resolution_clock::time_point end = high_resolution_clock::now();
    nanoseconds time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "Spawn thread function took [" << time_span.count() << "] ns" << std::endl;
    firstThread.join();
    
    start = high_resolution_clock::now();
    std::thread secondThread(testThreadedFunction);
    secondThread.detach();
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "Spawn detach thread function took [" << time_span.count() << "] ns" << std::endl;
    
    start = high_resolution_clock::now();
    auto ret = std::async(std::launch::async, testAsyncFunction);
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "Async function took [" << time_span.count() << "] ns" << std::endl;
    ret.get();
    
    int nbNano = 1000;
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < nbNano; ++cpt)
    {
        timespec t;
        t.tv_sec = 0;
        t.tv_nsec = 0;
        nanosleep(&t, NULL);
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "nanosleep 0 ns took [" << time_span.count()/nbNano << "] ns" << std::endl;
    
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < nbNano; ++cpt)
    {
        std::this_thread::sleep_for(nanoseconds(0));
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "this_thread::sleep_for 0 ns took [" << time_span.count()/nbNano << "] ns" << std::endl;
    
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < nbNano; ++cpt)
    {
        timespec t;
        t.tv_sec = 0;
        t.tv_nsec = 10000;
        nanosleep(&t, NULL);
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "nanosleep 10 us took [" << time_span.count()/nbNano << "] ns" << std::endl;
    
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < nbNano; ++cpt)
    {
        std::this_thread::sleep_for(microseconds(10));
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "this_thread::sleep_for 10 us took [" << time_span.count()/nbNano << "] ns" << std::endl;
    
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < nbNano; ++cpt)
    {
        timespec t;
        t.tv_sec = 0;
        t.tv_nsec = 1000000;
        nanosleep(&t, NULL);
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "nanosleep 1 ms took [" << time_span.count()/nbNano << "] ns" << std::endl;
    
    start = high_resolution_clock::now();
    for (int cpt=0; cpt < nbNano; ++cpt)
    {
        std::this_thread::sleep_for(milliseconds(1));
    }
    end = high_resolution_clock::now();
    time_span = duration_cast<nanoseconds>(end - start);
    std::cout << "this_thread::sleep_for 1 ms took [" << time_span.count()/nbNano << "] ns" << std::endl;
    
    if (0)
    {
        start = high_resolution_clock::now();
        for (int cpt=0; cpt < nbNano; ++cpt)
        {
            timespec t;
            t.tv_sec = 0;
            t.tv_nsec = 10000000;
            nanosleep(&t, NULL);
        }
        end = high_resolution_clock::now();
        time_span = duration_cast<nanoseconds>(end - start);
        std::cout << "nanosleep 10 ms took [" << time_span.count()/nbNano << "] ns" << std::endl;
        
        start = high_resolution_clock::now();
        for (int cpt=0; cpt < nbNano; ++cpt)
        {
            std::this_thread::sleep_for(milliseconds(10));
        }
        end = high_resolution_clock::now();
        time_span = duration_cast<nanoseconds>(end - start);
        std::cout << "this_thread::sleep_for 10 ms took [" << time_span.count()/nbNano << "] ns" << std::endl;
    }
}
