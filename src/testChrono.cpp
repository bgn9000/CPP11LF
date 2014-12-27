#include <iostream>

#include <chrono>

using namespace std::chrono;

typedef unsigned long long ticks;
static __inline__ ticks getticks(void)
{
#if defined (__amd64__) || defined (__x86_64__)
    // Read the high res tick counter into 32 bit int variables "eax" and
    // "edx", and then combine them into 64 bit int "now"
    unsigned int eax, edx;
    asm volatile ("rdtsc" : "=a" (eax), "=d" (edx) : : "memory");
    return (((ACE_UINT64) eax) | (((ACE_UINT64) edx) << 32));
#else
    // Read the high-res tick counter directly into memory variable "now".
    // The A constraint signifies a 64-bit int.
    ticks now;
    asm volatile ("rdtsc" : "=A" (now) : : "memory");
    return now;
#endif
}

void testChrono()
{
    std::cout << "================== testChrono =================" << std::endl;
    
    ticks start, end;
    double latency = 0.0;
    int loop = 1000000;
    
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        getticks();
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "getticks with latency [" << latency << "] (ticks)" << std::endl;
    
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        struct timespec ct;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ct);
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "CLOCK_PROCESS_CPUTIME_ID clock_gettime with latency [" << latency << "] (ticks)" << std::endl;
    
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        struct timespec ct;
        clock_gettime(CLOCK_MONOTONIC, &ct);
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "MONOTONIC clock_gettime with latency [" << latency << "] (ticks)" << std::endl;
    
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        system_clock::time_point stc = system_clock::now();
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "chrono system_clock with latency [" << latency << "] (ticks)" << std::endl;
    
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        struct timespec ct;
        clock_gettime(CLOCK_REALTIME, &ct);
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "REALTIME clock_gettime with latency [" << latency << "] (ticks)" << std::endl;
    
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        steady_clock::time_point stc = steady_clock::now();
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "chrono steady_clock with latency [" << latency << "] (ticks)" << std::endl;
    
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        high_resolution_clock::time_point hr = high_resolution_clock::now();
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "chrono high_resolution_clock with latency [" << latency << "] (ticks)" << std::endl;
    
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    high_resolution_clock::time_point t2;
    nanoseconds time_span;
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        t2 = high_resolution_clock::now();
        time_span = duration_cast<nanoseconds>(t2 - t1);
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "chrono high_resolution_clock then duration calculation with latency [" << latency << "] (ticks)" << std::endl;
}


