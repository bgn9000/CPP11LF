#include <iostream>
#include <type_traits>

#include <chrono>
using namespace std::chrono;

#include <typeinfo>
#include <cxxabi.h>

// Templates are a way to generate code when compiling (generic programming), here I intend to describe template usage that produce no code executed at runtime.
// This implies a constant time expectation whereas traditional code could be O(N).
// This is Template Meta programming purpose.

// example 1

template<int i> class UnrollSum
{
public:
    static inline constexpr int exec()
    {
// uncomment below to see recursive behavior
//        std::cout << i << ' ';
        constexpr auto ret = UnrollSum<i-1>::exec();
//        std::cout << i << ' ';
        return ret;
    }
};

// specialization for terminal case
template<> class UnrollSum<0>
{
public:
    static inline constexpr int exec()
    {
// uncomment below to see recursive behavior
//        std::cout << "0 ";
//        std::cout << "\n"; 
//        std::cout << "0 ";
        return 0;
    }
};

// example 3

template<bool Condition>
class If {
public:
    static inline bool exec()
    {
        return true;
    }
};

template <> class If<false> {
public:
    static inline bool exec()
    {
        return false;
    }
};

template<bool Condition, class Then, class Else>
struct Conditional
{ 
    typedef Then type; 
};
 
template<class Then, class Else>
struct Conditional<false, Then, Else>
{ 
    typedef Else type; 
};

int main()
{
    // example 1 : unroll loop : with the limit of compiler settings (added -ftemplate-depth=1500 in maxefile)
    // classic sum (runs 2000 increments and 1000 comparisons)
    high_resolution_clock::time_point begin = high_resolution_clock::now();
    for (auto sum = 0, i = 0; i < 1000; ++i)
    {
         sum += i;
    }
    high_resolution_clock::time_point end = high_resolution_clock::now();
    std::cout << "classic sum took=" << duration_cast<nanoseconds>(end - begin).count() << " ns" << std::endl;
    
    // unrolled sum (runs only an assignement of a constant value
    begin = high_resolution_clock::now();
    constexpr auto sum2 = UnrollSum<1000>::exec();
    end = high_resolution_clock::now();
    std::cout << "Unrolled sum took=" << duration_cast<nanoseconds>(end - begin).count() << " ns" << std::endl;
    
    // example 2 : std::conditional (depending on an compile time choice, we want either 64 bits integers or 32 bits)
        auto status = 0;
    constexpr auto choice = 4;
    std::conditional<(choice > 4), int64_t, int32_t>::type val = 0;
    std::cout << "choice=" << choice << " type=" << abi::__cxa_demangle(typeid(val).name(), 0, 0, &status) << std::endl;
    
    constexpr auto secondchoice = 8;
    std::conditional<(secondchoice > 4), int64_t, int32_t>::type val2 = 0;
    std::cout << "choice=" << secondchoice << " type=" << abi::__cxa_demangle(typeid(val2).name(), 0, 0, &status) << std::endl;
    
    // example 3 : conditional implementation
    std::cout << "If<true>::exec()=" << std::boolalpha << If<true>::exec() << std::endl;
    std::cout << "If<false>::exec()=" << If<false>::exec() << std::endl;
    
    Conditional<(choice > 4), int64_t, int32_t>::type myval = 0;
    std::cout << "choice=" << choice << " type=" << abi::__cxa_demangle(typeid(myval).name(), 0, 0, &status) << std::endl;
    
    Conditional<(secondchoice > 4), int64_t, int32_t>::type myval2 = 0;
    std::cout << "choice=" << choice << " type=" << abi::__cxa_demangle(typeid(myval2).name(), 0, 0, &status) << std::endl;
    
    return 0;
}
