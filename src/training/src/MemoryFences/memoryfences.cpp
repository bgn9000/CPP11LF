#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>

#include <chrono>
using namespace std::chrono;

#include <cassert>

std::string thread_siblings_list()
{
    std::ifstream infile("/sys/devices/system/cpu/cpu0/topology/thread_siblings_list", std::ios::in);
    std::string line;
    std::getline(infile, line);
    return line;
}

struct Data
{
    int nb = 0;
    bool stop = false;
    std::atomic<int> start{0};
    
    int x = 0;
    int y = 0;
    int z = 0;
    std::atomic<int> flag{0};
    
    double time_span = 0.0;
};

using millis = duration<double, std::milli>;

void threaded_function1(Data& data, int max)
{
    ++data.start;
    while (data.start < 2) ;
    high_resolution_clock::time_point start = high_resolution_clock::now();
    while (1)
    {
        assert(data.x == 0);
        assert(data.y == 0);
        assert(data.z == 0);
        data.x = 1;
        data.y = 2;
        data.z = 3;
        data.flag.store(1, std::memory_order_relaxed);
        while (data.flag.load(std::memory_order_relaxed) == 1) ;
        if (++data.nb > max) { data.stop = true; break; }
    }
    high_resolution_clock::time_point end = high_resolution_clock::now();
    data.time_span = duration_cast<millis>(end - start).count();
    
    data.x = 1;
    data.y = 2;
    data.z = 3;
    data.flag.store(1, std::memory_order_relaxed);
}

void threaded_function2(Data& data)
{
    ++data.start;
    while (!data.stop)
    {
        while (data.flag.load(std::memory_order_relaxed) == 0) ;
        std::atomic_thread_fence(std::memory_order_acquire);
        assert(data.x == 1);
        assert(data.y == 2);
        assert(data.z == 3);
        data.x = data.y = data.z = 0;
        std::atomic_thread_fence(std::memory_order_release);
        data.flag.store(0, std::memory_order_relaxed);
    }
}

void threaded_function1b(Data& data, int max)
{
    ++data.start;
    while (data.start < 2) ;
    high_resolution_clock::time_point start = high_resolution_clock::now();
    while (1)
    {
        assert(data.x == 0);
        assert(data.y == 0);
        assert(data.z == 0);
        data.x = 1;
        data.y = 2;
        data.z = 3;
        data.flag = 1;
        while (data.flag == 1) ;
        if (++data.nb > max) { data.stop = true; break; }
    }
    high_resolution_clock::time_point end = high_resolution_clock::now();
    data.time_span = duration_cast<millis>(end - start).count();
    
    data.x = 1;
    data.y = 2;
    data.z = 3;
    data.flag = 1;
}

void threaded_function2b(Data& data)
{
    ++data.start;
    while (!data.stop)
    {
        while (data.flag == 0) ;
        assert(data.x == 1);
        assert(data.y == 2);
        assert(data.z == 3);
        data.x = data.y = data.z = 0;
        data.flag = 0;
    }
    
}

int main()
{
    std::cout.sync_with_stdio(false);
    unsigned num_cpus = std::thread::hardware_concurrency();
    if (num_cpus < 4) { std::cerr << "need at least 4 cpu/core" << std::endl; return -1; }
    
    std::string str(thread_siblings_list());
    auto pos = str.find(',');
    auto core1 = std::stoi(str.substr(0, pos));
    auto core2 = std::stoi(str.substr(pos+1, str.length()));
    std::cout << "cpu with [" << core1 << '|' << core2 << ']' << std::endl;
    
    auto rc = 0;
    cpu_set_t cpuset;
    
    Data data1;
    std::thread t1(threaded_function1, std::ref(data1), 30'000'000);
    CPU_ZERO(&cpuset);
    CPU_SET(core1, &cpuset);
    rc = pthread_setaffinity_np(t1.native_handle(), sizeof(cpu_set_t), &cpuset);
    std::thread t2(threaded_function2, std::ref(data1));
    CPU_ZERO(&cpuset);
    CPU_SET(core2, &cpuset);
    rc = pthread_setaffinity_np(t2.native_handle(), sizeof(cpu_set_t), &cpuset);
    t1.join();
    t2.join();
    
    // cooling cpu
    std::this_thread::sleep_for(std::chrono::seconds(10));
    
    Data data2;
//    std::thread t1b(threaded_function1b, std::ref(data2), 30'000'000);
    std::thread t1b(threaded_function1, std::ref(data2), 30'000'000);
    CPU_ZERO(&cpuset);
    CPU_SET(core1, &cpuset);
    rc = pthread_setaffinity_np(t1b.native_handle(), sizeof(cpu_set_t), &cpuset);
//    std::thread t2b(threaded_function2b, std::ref(data2));
    std::thread t2b(threaded_function2, std::ref(data2));
    CPU_ZERO(&cpuset);
    CPU_SET(core2, &cpuset);
    rc = pthread_setaffinity_np(t2b.native_handle(), sizeof(cpu_set_t), &cpuset);
    t1b.join();
    t2b.join();
    
    if (data1.time_span < data2.time_span)
        std::cout << "1 (" << data1.time_span << ") faster than 2 (" << data2.time_span << ')' << std::endl;
    else
        std::cout << "2 (" << data2.time_span << ") faster than 1 (" << data1.time_span << ')' << std::endl;
    return 0;
}
