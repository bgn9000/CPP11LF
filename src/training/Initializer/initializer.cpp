#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cstdint>
#include <initializer_list>
#include <type_traits> // make_signed & is_null_pointer since c++14

void printNums(const std::initializer_list<int>& seq)
{
    std::cout << "printNums: " << seq.size() << ": [";
    for (auto it = seq.begin(); it != seq.end(); ++it)
    {
        if (it == seq.begin()) std::cout << *it;
        else std::cout << " " << *it;
    }
    std::cout << "]" << std::endl;
}

void printPairs(const std::initializer_list<std::pair<int,int>>& seq)
{
    std::cout << "printPairs: " << seq.size() << ": [";
    for (auto it = seq.begin(); it != seq.end(); ++it)
    {
        if (it == seq.begin()) std::cout << "(" << it->first << ":" << it->second << ")";
        else std::cout << " (" << it->first << ":" << it->second << ")";
    }
    std::cout << "]" << std::endl;
}

template<class T>
void printAllVect(const T& v)
{
    std::cout << "printAllVect: " << v.size() << ": [";
    // classic way
    /*for (auto it = v.begin(); it != v.end(); ++it)
    {
        if (it == v.begin()) std::cout << *it;
        else std::cout << " " << *it;
    }*/
    // range-for c++ way (not able to distinguish first loop to next ones)
    for (const auto data : v)
    {
        std::cout << " " << data;
    }
    std::cout << "]" << std::endl;
}

template<class T>
void printAllMap(const T& m)
{
    std::cout << "printAllMap: " << m.size() << ": [";
    // classic way
    /*for (auto it = m.begin(); it != m.end(); ++it)
    {
        if (it == m.begin()) std::cout << "(" << it->first << ":" << it->second << ")";
        else std::cout << " (" << it->first << ":" << it->second << ")";
    }*/
    // range-for c++ way (not able to distinguish first loop to next ones)
    for (const auto& p : m)
    {
        std::cout << " (" << p.first << ":" << p.second << ")";
    }
    std::cout << "]" << std::endl;
}

class Timer
{
public:
    Timer() = default;
    Timer(const Timer&) = delete;
    Timer& operator= (const Timer&) = delete;
};

class TimeKeeper
{
public:
    TimeKeeper(const Timer& t) {}
    int get_time() { return 0; }
};

template<class T> typename std::make_signed<T>::type as_signed(T t) { return typename std::make_signed<T>::type(t); }

void testnull(int n)
{
    std::cout << "testnull int: " << n << std::endl;
}

void testnull(char *p)
{
    if (p == nullptr)
    {
        std::cout << "testnull char*: null" << std::endl;
    }
    else
    {
        std::cout << "testnull char*: not null" << std::endl;
    }
}

void testnull(std::nullptr_t p)
{
    std::cout << "testnull nullptr: null_pointer=" << std::is_null_pointer<decltype(p)>::value << std::endl;
}

int main()
{
    printNums( {10,20,30} );
    printNums( {} );
    printNums( {40} );
    printPairs( { {1,1}, {2,4}, {3,6}, {4,16} } );
    
    int A[] = {1,2,3,4,5};
    
    std::vector<int> Vect = {1,2,3,4,5};
    printAllVect<>(Vect);
    std::vector<double> Vect2 = {1.1,2.2,3.3,4.4,5.5};
    printAllVect<>(Vect2);
    
    std::pair<int,int> pair = {1,1};
    std::map<int,int> squares = { {1,1}, {2,4}, {3,6}, {4,16} };
    printAllMap<>(squares);
    std::cout << "square=" << squares[3] << std::endl;
    
    // uniform init & prevent narrowing
    Timer t1;
    // error copy constructor deleted: Timer t2(t1);
    // error copy constructor deleted: Timer t2 = t1;
    // error operator= delete: Timer t2; t2 = t1;
    TimeKeeper tq1(t1);
    tq1.get_time();
    TimeKeeper tq2{Timer{}}; // TimeKeeper tq(Timer()); : ambiguous declaration (variable or function)
    tq2.get_time();
    //int narrow(3.14); // error with -Wconversion or 3.14 becomes 3
    double narrow{3.14};
    std::cout << "narrow=" << narrow << std::endl;
    
    // Herb sutter : prefer to declare local variables "auto x = type{ expr };" when you want explicitly commit to a type
    // won't allow an accidental implicit narrowing conversion, otherwise use () instead of {}
    // it is a way to self document weither or not the code is requesting a conversion
    std::string str = "hello";
    auto calc1 = std::string::size_type{str.size() * 2};
    auto calc2 = uint32_t(str.size() * 2);
    // auto calc3 = uint32_t{str.size() * 4e6}; : narrowing not allowed from double to uint32_t
    // auto calc3 = uint32_t(str.size() * 4e6); // error with -Wconversion
    
    // signed / unsigned traps
    uint32_t x = 42;
    int32_t y = 43;
    auto diff = x - y;
    std::cout << "diff=" << diff << std::endl;
    // see upper as_signed declaration
    auto diff2 = as_signed(x - y);
    std::cout << "diff2=" << diff2 << std::endl;
    
    // auto = { expr } => std::initializer_list
    auto a(1);
    auto b = {2};
    auto sum = a + *(b.begin());
    std::cout << "sum=" << sum << std::endl;
    
    testnull(0);
    // testnull(NULL); //=> ambiguous between int and char* : use nullptr

    char* strnull = NULL; 
    testnull(strnull);

    strnull = new char();
    testnull(strnull);
    delete strnull;

    strnull = nullptr;
    testnull(strnull);

    testnull(nullptr);
    return 0;
}
