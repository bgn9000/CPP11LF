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
    for (int cpt = 0; cpt < 5; ++cpt)
    {
        v1.push_back(cpt);
    }
    auto v2 = v1;
    printAll(v1);
    printAll(v2);
    return 0;
}

