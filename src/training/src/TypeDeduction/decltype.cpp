#include <iostream>

#include <typeinfo>
#include <cxxabi.h>

double onefunction();
double secondfunction(double x) { return x*x; }
class toto
{
};

template<class F, class X>
auto wrapper(F func, X x) -> decltype(func(x))
{
    std::cout << "Func called" << std::endl;
    return func(x);
}

int main()
{
    auto status = 0;
    
    // auto is more convenient than decltype (used to get return type if not statically known)
    decltype(onefunction()) f1;
    std::cout << "type is " << abi::__cxa_demangle(typeid(f1).name(), 0, 0, &status) << std::endl;
    
    decltype(onefunction) f1b;
    std::cout << "type is " << abi::__cxa_demangle(typeid(f1b).name(), 0, 0, &status) << std::endl;
    
    decltype(secondfunction) f2;
    std::cout << "type is " << abi::__cxa_demangle(typeid(f2).name(), 0, 0, &status) << std::endl;
    
    std::cout << "type is " << abi::__cxa_demangle(typeid(toto).name(), 0, 0, &status) << std::endl;
    
    // return type
    auto res = wrapper(secondfunction, 2);
    std::cout << "type is " << abi::__cxa_demangle(typeid(res).name(), 0, 0, &status) << " and res=" << res << std::endl;
    
    return 0;
}

