#include <vector>
#include <iostream>

void printAll(const std::vector<int>& v)
{
    for (auto it = v.begin(); it != v.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
}

int main()
{
    auto v1 = std::vector<int>();
    for (auto cpt = 0; cpt < 5; ++cpt)
    {
        v1.push_back(cpt);
    }
    auto v2 = v1;
    printAll(v1);
    printAll(v2);
    
    int i = 0;
    int& j = i;
    auto k = j; // auto is an int not a int&
    std::cout << "i(int)=" << i << " j(int &i)=" << j << " k(auto?)=" << k << std::endl;
    ++i;
    std::cout << "i(int)=" << i << " j(int &i)=" << j << " k(auto?)=" << k << std::endl;
    return 0;
}

