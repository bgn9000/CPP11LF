#include "common.h"

void testAlloc()
{
    std::cout << "================== testAlloc =================" << std::endl;
    
    ticks start, end;
    double latency = 0.0;
    constexpr int loop = 1000000;
    
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        std::string* mystring = new std::string();
        delete mystring;
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "empty stdstring new/delete with latency [" << latency << "] (ticks)" << std::endl;
    
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        char* mycharp = new char();
        delete mycharp;
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "char new/delete with latency [" << latency << "] (ticks)" << std::endl;
    
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        std::string* mystring = new std::string();
        mystring->resize(64);
        delete mystring;
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "std::string(64) new/delete with latency [" << latency << "] (ticks)" << std::endl;
    
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        char* mycharp = new char[64];
        delete [] mycharp;
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "char[64] new/delete with latency [" << latency << "] (ticks)" << std::endl;
    
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        std::string* mystring = new std::string();
        mystring->resize(1024);
        delete mystring;
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "std::string(1024) new/delete with latency [" << latency << "] (ticks)" << std::endl;
     
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        char* mycharp = new char[1024];
        delete [] mycharp;
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "char[1024] new/delete with latency [" << latency << "] (ticks)" << std::endl;
    
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        std::string* mystring = new std::string();
        mystring->resize(70000);
        delete mystring;
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "std::string(70000) new/delete with latency [" << latency << "] (ticks)" << std::endl;
     
    start = getticks();
    for (int cpt=0; cpt < loop; ++cpt)
    {
        char* mycharp = new char[70000];
        delete [] mycharp;
    }
    end = getticks();
    latency = (double)((ticks)(end-start))/loop;
    std::cout << "char[70000] new/delete with latency [" << latency << "] (ticks)" << std::endl;
}
