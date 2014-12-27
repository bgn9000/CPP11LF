#include <iostream>
#include <thread>

extern void testThread();
extern void testChrono();
extern void testOnePublisherOneListener();

int main(int argc, char* argv[])
{
    std::cout << "Start" << std::endl;
        
    testThread();
    
    testChrono();
    
    std::thread testOnePublisherOneListenerThread(testOnePublisherOneListener);
    testOnePublisherOneListenerThread.join();
    
    std::cout << "End" << std::endl;
    return 0;
}

