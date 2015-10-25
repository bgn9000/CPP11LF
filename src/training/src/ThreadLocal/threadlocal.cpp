#include <iostream>
#include <thread>
#include <chrono>

class Local
{
public:
    Local()
    {
//        std::cout << "Create Local: " << this << std::endl;
    }
    ~Local()
    {
//        std::cout << "Destroy Local: " << this << std::endl;
    }
    void display(int n)
    {
        std::cout << "display Local: " << n << "(" << this << ")" << std::endl;
    }
};

thread_local Local local;

void threaded_function(int n)
{
    local.display(n);
}

int main()
{
    // non concurrent threads : same local objet
    std::cout << "non concurrent threads..." << std::endl;
    std::thread t1(threaded_function, 1);
    t1.join();
    std::thread t2(threaded_function, 2);
    t2.join();

    // concurrent threads : different local objets
    std::cout << "concurrent threads..." << std::endl;
    std::thread t3(threaded_function, 3);
    std::thread t4(threaded_function, 4);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    t3.join();
    t4.join();

    return 0;
}
