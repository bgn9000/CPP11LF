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

    std::cout << "FREQUENCY_CPU [" << FREQUENCY_CPU << "]" << std::endl;
}

bool doTestThread = true;
bool doTestThread10ms = false;
bool doTestAlloc = true;
bool doTestChrono = true;
bool doTestAtomic = true;
bool doTestOnePublisherOneListenerThread = true;
bool doTestOnePublisherOneListenerThreadBasic = true;
bool doTestOnePublisherOneListenerThreadParallel = true;

int main(int argc, char* argv[])
{
#if defined (__amd64__) || defined (__x86_64__)
    std::cout << "Start on arch64" << std::endl;
#else
    std::cout << "Start on arch32" << std::endl;
#endif

    computeCpuFreq();
    
    // parse arguments
    for (int cpt = 0; cpt < argc; ++cpt)
    {
        if (argv[cpt][0] == '-')
        {
            switch(argv[cpt][1])
            {
                case '1': doTestThread = false; break;
                case '3': doTestAlloc = false; break;
                case '4': doTestChrono = false; break;
                case '5': doTestAtomic = false; break;
                case '6': doTestOnePublisherOneListenerThread = false; break;
                case '7': doTestOnePublisherOneListenerThreadBasic = false; break;
                case '8': doTestOnePublisherOneListenerThreadParallel = false; break;
                default: 
                    std::cerr << "!!!! bad argument !!!!" << std::endl;
            }
        }
        else if (argv[cpt][1] == '+')
        {
            switch(argv[cpt][1])
            {
                case '2': doTestThread10ms = true; break;
                default: 
                    std::cerr << "!!!! bad argument !!!!" << std::endl;
            }
        }
    }
    
    if (doTestThread) testThread();
    if (doTestAlloc) testAlloc();
    if (doTestChrono) testChrono();
    if (doTestAtomic) testAtomic();
    if (doTestOnePublisherOneListenerThread)
    {
        std::thread testOnePublisherOneListenerThread(testOnePublisherOneListener);
        testOnePublisherOneListenerThread.join();
    }
    
    std::cout << "End" << std::endl;
    return 0;
}

