#include "common.h"

extern void testThread();
extern void testChrono();
extern void testOnePublisherOneListener();

int main(int argc, char* argv[])
{
    std::cout << "Start" << std::endl;
        
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

