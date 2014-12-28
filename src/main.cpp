#include "common.h"

double FREQUENCY_CPU = 0.0;
int FREQUENCY_CPU_GETTICKS2TIME = 0;

void computeCpuFreq()
{
    ticks start, end;
    double latency1 = 0.0, latency2 = 0.0;
    nanoseconds time_span;
    for (int cpt = 0; cpt < 100; ++cpt)
    {
        high_resolution_clock::time_point hr1 = high_resolution_clock::now();
	    start = getticks();
        std::this_thread::sleep_for(microseconds(100));
        high_resolution_clock::time_point hr2 = high_resolution_clock::now();
    	end = getticks();
    	time_span = duration_cast<nanoseconds>(hr2-hr1);
        latency1 += time_span.count();
        latency2 += ((ticks)(end-start));
    }
    latency1 /= 100.0;
    latency2 /= 100.0;
    FREQUENCY_CPU = (double)(latency2/latency1) * 1000000000.0;
    FREQUENCY_CPU_GETTICKS2TIME = (unsigned int)((FREQUENCY_CPU/1000000)+0.5);

    std::cout << "FREQUENCY_CPU [" << FREQUENCY_CPU 
              << "] FREQUENCY_CPU_GETTICKS2TIME [" << FREQUENCY_CPU_GETTICKS2TIME
              << "]" << std::endl;
}

int main(int argc, char* argv[])
{
#if defined (__amd64__) || defined (__x86_64__)
    std::cout << "Start on arch64" << std::endl;
#else
    std::cout << "Start on arch32" << std::endl;
#endif

    computeCpuFreq();
    
    if (doTestThread) testThread();
    if (doTestChrono) testChrono();
    if (doTestOnePublisherOneListenerThread)
    {
        std::thread testOnePublisherOneListenerThread(testOnePublisherOneListener);
        testOnePublisherOneListenerThread.join();
    }
    
    std::cout << "End" << std::endl;
    return 0;
}

